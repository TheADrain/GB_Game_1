#include "main.h"
#include <gb/gb.h>
#include "bankdefs.h"
#include "sprite_manager.h"
#include "levels.h"
#include "input.h"
#include "utils_1.h"
#include "player.h"
#include "scrolling.h"
#include "level_loading.h"

/* ---------------------GLOBAL VARS-------------------------- */
UINT8 GAME_FLOW_STATE = GAMEFLOW_BOOT;

UINT8 global_frame_count = 0U;

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
	if(GAME_FLOW_STATE == GAMEFLOW_GAME)
	{
		DoGraphicsUpdate();
	}
}

void DoGraphicsUpdate()
{
	disable_interrupts();
	/* do as much of our graphics update as possible in one place */	
	move_bkg(camera_x, camera_y);

	/* see if we have a stored commane to load tiles into background vram */
	if(stored_tile_load_command == 1U)
	{
		stored_tile_load_command = 0U;

		SWITCH_ROM_MBC1(levels[CUR_LEVEL].RomBank);
		set_bkg_tiles(stored_tile_load_bkg_x, 
			stored_tile_load_bkg_y, 
			stored_tile_load_bkg_w, 
			stored_tile_load_bkg_h, 
			stored_scrl_dat_ptr);
	}

	/* sprites are rendered at the position -8 in x and -8 in y so that 0,0 equates to off-screen */
	/* always bottom right of the player hitbox */
	UINT16 pos_x = (player_world_x - SCX_REG) + 4;
	UINT16 pos_y = (player_world_y - SCY_REG) + 8;
	move_sprite(player_sprite_num, pos_x-4, pos_y-16);
	move_sprite(player_sprite_num+1U, pos_x+4, pos_y-16);
	move_sprite(player_sprite_num+2U, pos_x-4, pos_y-8);
	move_sprite(player_sprite_num+3U, pos_x+4, pos_y-8);
	move_sprite(player_sprite_num+4U, pos_x-4, pos_y);
	move_sprite(player_sprite_num+5U, pos_x+4, pos_y);
	/*update animated tiles*/

	enable_interrupts(); 
}

void main()
{
	boot_init();  
 
  /* main loop */
  while(1) {

	  poll_input(); 
	  global_frame_count = global_frame_count + 1;
	  wait_vbl_done();

	  switch(GAME_FLOW_STATE)
	  {
			case GAMEFLOW_BOOT:
				boot_update();
				break;

	  		case GAMEFLOW_SPLASH:
				splash_update();
				break;

			case GAMEFLOW_TITLE:
				title_update();
				break;

			case GAMEFLOW_LEVELCARD:
				levelcard_update();
				break;

			case GAMEFLOW_GAME:

				if(JOY_PRESSED(BTN_START))
				{
					GAME_FLOW_STATE = GAMEFLOW_LEVELCARD;
					end_level();

					CUR_LEVEL = CUR_LEVEL + 1;
					set_current_level(CUR_LEVEL);
					levelcard_init();	
					 
					break;
				}

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

	TDT_MODE_0;
	/* startup */
	DISPLAY_ON;
  
	/* ensure we're in sprite 8x8 mode */
	init_sprite_manager();
	SPRITES_8x8;
	SHOW_SPRITES;

	enable_interrupts(); 

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
		set_current_level(FIRST_LVL);
		levelcard_init();	
		GAME_FLOW_STATE = GAMEFLOW_LEVELCARD;
	}
}

