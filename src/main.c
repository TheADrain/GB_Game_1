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
				SWITCH_ROM_MBC1(levels[CUR_LEVEL].RomBank);
				update_player();
				SWITCH_ROM_MBC1(levels[CUR_LEVEL].RomBank);
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
		FadeToWhite(4U);

		set_current_level(1U);
		load_current_level_graphics();
		load_current_level_map();	

		start_level();
		

		GAME_FLOW_STATE = GAMEFLOW_GAME;
	}
}

void set_current_level(UINT8 newLevel)
{
	disable_interrupts();

	CUR_LEVEL = newLevel;

	enable_interrupts();
}

void load_current_level_graphics()
{
	disable_interrupts();

	/* ensure we're in sprite 8x16 mode */
	SPRITES_8x16;

	/* load the sprite tiles (for now this is same for all levels) */
	SWITCH_ROM_MBC1(BANK_GRAPHICS_DATA_1);
	set_sprite_data(0x00, spritesLength, sprites);

	/* load the background tiles for this map */
	set_bkg_data(0x00, levels[CUR_LEVEL].tileDataLength, levels[CUR_LEVEL].tileDataPtr);

	level_tilemap_data = levels[CUR_LEVEL].MapTileData;
	level_collision_data = levels[CUR_LEVEL].CollisionMap;
	CUR_MAP_WIDTH = levels[CUR_LEVEL].Width;
	CUR_MAP_HEIGHT = levels[CUR_LEVEL].Height;

	enable_interrupts();
}

void load_current_level_map()
{
	disable_interrupts();
	
	/* Initialize the title map data */
	SWITCH_ROM_MBC1(levels[CUR_LEVEL].RomBank);

	/* currently assuming column-first map */
	UINT8 i = 0;
	unsigned char* dataPtr = levels[CUR_LEVEL].MapTileData;
		
	
	/* draw left to right for a horizontal level */
	if(levels[CUR_LEVEL].MapType == MAP_HORIZONTAL)
	{
		for(i = 0; i < 32; i++)
		{
			set_bkg_tiles(i, 0, 1, CUR_MAP_HEIGHT, dataPtr);
			dataPtr += CUR_MAP_HEIGHT;
		}
	}
	/* and top to bottom for a vertical one */
	else
	{
		for(i = 0; i < 32; i++)
		{
			set_bkg_tiles(0, i, CUR_MAP_WIDTH, 1, dataPtr);
			dataPtr += CUR_MAP_WIDTH;
		}
	}
	
	enable_interrupts();
}

void start_level()
{
	init_game_camera();
	init_player_sprite();

	SHOW_BKG;
	SHOW_SPRITES;

	gbt_enable_channels(0x00);
	gbt_play(song_Data, 2, 7);
	gbt_loop(1);

	/* spawn and position the player */

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