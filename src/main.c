#include "main.h"
#include <gb/gb.h>
#include "bankdefs.h"
#include "sprite_manager.h"
#include "actors.h"
#include "levels.h"
#include "input.h"
#include "utils_1.h"
#include "player.h"
#include "scrolling.h"
#include "level_loading.h"


#ifdef USE_TEST_SCENE
#include "_test_scene.c"
#endif

/* ---------------------GLOBAL VARS-------------------------- */
UINT8 GAME_FLOW_STATE = GAMEFLOW_BOOT;

UINT8 global_frame_count = 0U;

/* INPUT VARS */
UINT8 pad_state_temp = 0U;

/* HELPER VARS */
UINT8 v_i = 0U;
UINT8 v_j = 0U;
UINT8 v_k = 0U;

/* ---------------------LEVEL DATA-------------------------- */
UINT8 CUR_LEVEL = 0U;
UINT8 CUR_SUBLEVEL = 0U;
UINT8 CUR_LEVEL_DATA_IDX = 0U;
UINT8 CUR_LEVEL_BANK = BANK_MAPDATA_01;
UINT8 CUR_MAP_WIDTH = 32;
UINT8 CUR_MAP_HEIGHT = 32;

unsigned char* level_tilemap_data;
unsigned char* level_collision_data;

/* ---- temp test stuff ---- */

/* -------------------------- */


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
	manual_update_player_sprite();

	/* see if we have a stored command to load tiles into background vram */
	if(stored_tile_load_command == 1U)
	{
		stored_tile_load_command = 0U;

		SWITCH_ROM_MBC1(level_datas[CUR_LEVEL_DATA_IDX].RomBank);
		set_bkg_tiles(stored_tile_load_bkg_x, 
			stored_tile_load_bkg_y, 
			stored_tile_load_bkg_w, 
			stored_tile_load_bkg_h, 
			stored_scrl_dat_ptr);
	}

	enable_interrupts(); 
}

void main()
{
	wait_vbl_done();
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

#ifdef USE_TEST_SCENE
/* alternate loop for test scene only */
			default:
				_test_scene_update();
				break;
#else

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

				UpdateActors();

				update_camera(); 

				CheckLevelCompletion();

				break;
#endif
	  }
  }
}

void CheckLevelCompletion_ExitTiles()
{

	UINT16 test_x = player_world_x - PLAYER_HALF_WIDTH;
	/*one above player position, as grounded player position
		is actually in the top pixel of the floor tile */
	UINT16 test_y = (player_world_y)-2;
	collision_grid_test_x = test_x;
	collision_grid_test_y = test_y;

	TestCollisionAtWorldPosition();

	if (collision_grid_test_result == COL_LEVEL_COMPLETE)
	{
		/* we finished the level */
		/* todo: take control away from the player, and
			hook up some sort of animation */ 
		GAME_FLOW_STATE = GAMEFLOW_LEVELCARD;
		end_level();

		CUR_LEVEL = CUR_LEVEL + 1;
		set_current_level(CUR_LEVEL);
		levelcard_init();
	}
}

void CheckLevelCompletion_RightBounds() 
{
	/* check if we're one tile from the end of the level for now, because detecting the levels total data lenght in collision is a bit awkward */
	if ((player_world_x + (TILE_SIZE*2)) >= (level_datas[CUR_LEVEL_DATA_IDX].Width * TILE_SIZE))
	{
		HIDE_SPRITES;
		FadeToWhite(4U);
		
		wait_vbl_done();

		increment_sub_level_data();
		wait_vbl_done();
		load_current_level_graphics();
		wait_vbl_done();
		put_player_at_spawn_point();
		init_game_camera();

		chunk_v_offset = 0;
		chunk_h_offset = 0;

		if (level_datas[CUR_LEVEL_DATA_IDX].MapType == MAP_VERTICAL)
		{
			if (level_datas[CUR_LEVEL_DATA_IDX].SpawnType == SPAWN_BOTTOM)
			{
				chunk_v_offset = (level_datas[CUR_LEVEL_DATA_IDX].Height % 32);
				load_current_vertical_level_map_chunk();
			}
			else
			{
				load_current_level_map();
			}
		}
		else
		{
			load_current_level_map();
		}
		

		FadeFromWhite(4U);
		SHOW_SPRITES;
	}
}

void CheckLevelCompletion_SmoothToNextChunk() 
{
	if (level_datas[CUR_LEVEL_DATA_IDX].MapType == MAP_VERTICAL)
	{
		if (level_datas[CUR_LEVEL_DATA_IDX].SpawnType == SPAWN_BOTTOM)
		{
			if (camera_y == 0U)
			{
				/* next chunk */
				
				increment_sub_level_data();

				chunk_v_offset = 0;
				chunk_h_offset = 0;

				load_current_horizontal_level_map_chunk();

				init_game_camera();
			}
		}
		else
		{
			if (camera_y >= camera_bottom_bound)
			{
				/* next chunk */
				increment_sub_level_data();

				chunk_v_offset = 0;
				chunk_h_offset = 0;

				load_current_horizontal_level_map_chunk();

				init_game_camera();
			}
		}
	}
	else
	{
		if (camera_x >= camera_right_bound)
		{
			chunk_v_offset = 0;
			chunk_h_offset = (level_datas[CUR_LEVEL_DATA_IDX].Width % 32);

			increment_sub_level_data();

			if (level_datas[CUR_LEVEL_DATA_IDX].SpawnType == SPAWN_BOTTOM)
			{
				chunk_v_offset = (level_datas[CUR_LEVEL_DATA_IDX].Height % 32);
			}
			
			load_current_vertical_level_map_chunk();

			//init_game_camera();
		}
	}
}

void CheckLevelCompletion()
{
	player_lvl_complete_check_timer = player_lvl_complete_check_timer + 1;
	/* get the tile at the players feet and see if it is
	   a level complete tile */

	if(player_lvl_complete_check_timer >= CHECK_INTERVAL_LVL_COMPLETION)
	{
		player_lvl_complete_check_timer = 0U;

		switch (level_datas[CUR_LEVEL_DATA_IDX].ExitType)
		{
			case EXIT_EXIT_TILES_ONLY:
				CheckLevelCompletion_ExitTiles();
				break;

			case EXIT_RIGHT_BOUNDS:
				CheckLevelCompletion_RightBounds();
				break;

			case EXIT_SMOOTH_TO_NEXT_CHUNK:
				CheckLevelCompletion_SmoothToNextChunk();
				break;

			default:
				CheckLevelCompletion_ExitTiles();
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
  
	/* ensure we're in sprite 8x8 mode and init sprite systems */
	init_sprite_manager();
	init_actor_manager();
	SPRITES_8x8;
	SHOW_SPRITES;

	enable_interrupts(); 

#ifdef USE_TEST_SCENE
	GAME_FLOW_STATE = 0xFF;
	return;
#endif

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
#ifdef START_DEBUG_LEVEL_1
		/* move to the debug level */
		set_current_level(TOTAL_LEVELS-1);
		levelcard_init();
		GAME_FLOW_STATE = GAMEFLOW_LEVELCARD;
#else

		/* move to the next state */
		set_current_level(FIRST_LVL);
		levelcard_init();	
		GAME_FLOW_STATE = GAMEFLOW_LEVELCARD;
#endif
	}
}

