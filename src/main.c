#include "main.h"
#include <gb/gb.h>
#include "bankdefs.h"
#include "levels.h"
#include "input.h"
#include "utils_1.h"
#include "player.h"
#include "scrolling.h"

/* ---------------------GLOBAL VARS-------------------------- */

UINT8 GAME_FLOW_STATE = GAMEFLOW_BOOT;

/* INPUT VARS */
UINT8 pad_state_temp = 0U;

/* ---------------------LEVEL DATA-------------------------- */
UINT8 CUR_LEVEL = 0U;
UINT8 CUR_LEVEL_BANK = BANK_MAPDATA_01;
UINT8 CUR_MAP_WIDTH = 32;
UINT8 CUR_MAP_HEIGHT = 32;



unsigned char* level_tilemap_data;
unsigned char* level_collision_data;

void vblint()
{
	/* for now this routine is only used for gameplay */
	if(GAME_FLOW_STATE != GAMEFLOW_GAME)
	{
		return;
	}

	DoGraphicsUpdate();
}

void main()
{
  TDT_MODE_0;
  /* startup */
  DISPLAY_ON;
  enable_interrupts(); 
  boot_init();  

  add_VBL(vblint);

  /* main loop */
  while(1) {

	  poll_input();

	  switch(GAME_FLOW_STATE)
	  {
			case GAMEFLOW_BOOT:
				wait_vbl_done();
				boot_update();
				break;

	  		case GAMEFLOW_SPLASH:
				wait_vbl_done();
				splash_update();
				break;

			case GAMEFLOW_TITLE:
				wait_vbl_done();
				title_update();
				break;

			case GAMEFLOW_LEVELCARD:
				wait_vbl_done();
				levelcard_update();
				break;

			case GAMEFLOW_GAME:
				wait_vbl_done();
				update_player();
				update_camera();
				break;
	  }
  }
}

void boot_init()
{
	/* Startup sequence */
	/* Mostly unused, this is just setup space for anything that needs 
		to happen before the splash screen */

	reset_input();
	FadeOutNintendoLogo();

	/* immediately move on to splash state */
	splash_init();
	GAME_FLOW_STATE = GAMEFLOW_SPLASH;
}

void boot_update()
{
	
}

void splash_init()
{
	disable_interrupts();
	/* Initialize the VRAM data */
	SWITCH_ROM_MBC1(BANK_TITLE);
	/* load tiles for the title screen */
	set_bkg_data(0x00, splash_tilesLength, splash_tiles);
	/* Initialize the title map data */
	set_bkg_tiles(0, 0, splash_mapWidth, splash_mapHeight, splash_map);
	SHOW_BKG;
	
	/* test music */
	gbt_enable_channels(0x00);
	gbt_play(song_Data, 2, 7);
    gbt_loop(0);
    set_interrupts(VBL_IFLAG);
	/* test music */

	enable_interrupts();

	FadeFromBlack(4U);
}

void splash_update()
{
	if(JOY_PRESSED(BTN_A))
	{
		NR50_REG = 0x77;
		NR51_REG = 0xFF;
		NR52_REG = 0x80;
		/*NR41-44
		3A, A3, 33, 80*/
		NR41_REG = 0x3A;
		NR42_REG = 0xA3;
		NR43_REG = 0x33;
		NR44_REG = 0x80;
	}

	if(JOY_PRESSED(BTN_START))
	{
		/* move to the next state */
		gbt_stop();
		title_init();
		GAME_FLOW_STATE = GAMEFLOW_TITLE;
	}

	gbt_enable_channels(0x07);
	gbt_update();
}

void title_init()
{
	FadeToBlack(4U);

	disable_interrupts();
	/* Initialize the VRAM data */
	SWITCH_ROM_MBC1(BANK_TITLE);
	/* load tiles for the title screen */
	set_bkg_data(0x00, td_titleLength, td_title);
	/* Initialize the title map data */
	set_bkg_tiles(0, 0, tm_titleWidth, tm_titleHeight, tm_title);
	SHOW_BKG;
	enable_interrupts();

	FadeFromBlack(4U);
}

void title_update()
{
	if(JOY_PRESSED(BTN_START))
	{
		/* move to the next state */
		levelcard_init();	
		GAME_FLOW_STATE = GAMEFLOW_LEVELCARD;
	}
}

void levelcard_init()
{
	FadeToBlack(4U);

	disable_interrupts();
	/* Initialize the VRAM data */
	SWITCH_ROM_MBC1(BANK_TITLE);
	/* load tiles for the title screen */
	set_bkg_data(0x00, levelcard_tilesLength, levelcard_tiles);
	/* Initialize the title map data */
	set_bkg_tiles(0, 0, levelcard_mapWidth, levelcard_mapHeight, levelcard_map);
	SHOW_BKG;
	enable_interrupts();

	FadeFromBlack(4U);
}

void levelcard_update()
{
	if(JOY_PRESSED(BTN_START))
	{
		/* move to the next state */
		load_current_level();	
		GAME_FLOW_STATE = GAMEFLOW_GAME;
	}
}

void load_current_level()
{
	FadeToWhite(4U);
	/* load the level pointed to by CUR_LEVEL */
	/* todo: bank switching based on which level we want rather than just switching to bank 3 */
	disable_interrupts();

	/* Initialize the VRAM data */
	SWITCH_ROM_MBC1(BANK_GRAPHICS_DATA_1);
	/* load the sprite tiles */
	SPRITES_8x16;
	set_sprite_data(0x00, spritesLength, sprites);
	/* load tiles for the title screen */
	//set_bkg_data(0x00, tower_tiledataLength, tower_tiledata);
	set_bkg_data(0x00, forest_tiledataLength, forest_tiledata);
	CUR_LEVEL = 2;
	/* Initialize the title map data */
	SWITCH_ROM_MBC1(levels[CUR_LEVEL].RomBank);

	/*level_tilemap_data = levels[CUR_LEVEL].MapTileData;
	level_collision_data = levels[CUR_LEVEL].CollisionMap;
	CUR_MAP_WIDTH = levels[CUR_LEVEL].Width;*/

	level_tilemap_data = levels[CUR_LEVEL].MapTileData;
	level_collision_data = levels[CUR_LEVEL].CollisionMap;
	CUR_MAP_WIDTH = levels[CUR_LEVEL].Width;
	CUR_MAP_HEIGHT = levels[CUR_LEVEL].Height;

	UINT8 i = 0;
	for(i = 0; i < CUR_MAP_HEIGHT; i++)
	{
		set_bkg_tiles(0, i, BKG_WIDTH, 1, level_tilemap_data+(CUR_MAP_WIDTH*i));
	}

	//set_bkg_tiles(0, 0, levels[CUR_LEVEL].Width, levels[CUR_LEVEL].Height, level_tilemap_data);

	SHOW_BKG;
	SHOW_SPRITES;
	
	init_game_camera();
	init_player_sprite();

	enable_interrupts();
	FadeFromWhite(4U);
}

void DoGraphicsUpdate()
{
	/* do as much of our graphics update as possible in one place */	
	disable_interrupts();

	move_bkg(camera_x, camera_y);
	/* sprites are rendered at the position -8 in x and -16 in y so that 0,0 equates to off-screen */
	move_sprite(player_sprite_num, (player_world_x + PLAYER_SPRITE_WIDTH - SCX_REG), (player_world_y + PLAYER_SPRITE_HEIGHT - SCY_REG));

	enable_interrupts(); 
}