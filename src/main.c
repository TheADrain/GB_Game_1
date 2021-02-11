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

/* ---- temp test stuff ---- */
struct SHAPE * shape_ptr;
struct SHAPE * shape_ptr2;
struct SHAPE * shape_ptr3;
struct SHAPE * shape_ptr4;
struct SHAPE * shape_ptr5;
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

	/* see if we have a stored command to load tiles into background vram */
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

	/* sprites are rendered at the position -8 in x and -16 in y so that 0,0 equates to off-screen */
	/* always bottom right of the player hitbox */
	UINT16 pos_x = (player_world_x - SCX_REG) + 8;
	UINT16 pos_y = (player_world_y - SCY_REG) + 16;
	move_sprite(player_sprite_num, pos_x-4, pos_y-24);
	move_sprite(player_sprite_num+1U, pos_x+8, pos_y-24);
	move_sprite(player_sprite_num+2U, pos_x, pos_y-16);
	move_sprite(player_sprite_num+3U, pos_x+8, pos_y-16);
	move_sprite(player_sprite_num+4U, pos_x, pos_y-8);
	move_sprite(player_sprite_num+5U, pos_x+8, pos_y-8);
	/*update animated tiles*/

	

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
  
	/* ensure we're in sprite 8x8 mode and init sprite systems */
	init_sprite_manager();
	init_shape_manager();
	SPRITES_8x8;
	SHOW_SPRITES;

	/* put some temp sprites over the nintendo ones */
	SWITCH_ROM_MBC1(BANK_GRAPHICS_DATA_1);
	set_sprite_data(0x00, player_spritesLength, player_sprites);

	if(create_shape(SHAPE_2x2) == SHAPE_AVAILABLE)
	{
		shape_ptr = created_shape_ptr;
		shape_ptr->PositionX = 50;
		shape_ptr->PositionY = 80;

		set_sprite_tile((*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[0], 2);
		set_sprite_tile((*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[1], 3);
		set_sprite_tile((*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[2], 18);
		set_sprite_tile((*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[3], 19);

		shape_ptr->Place(shape_ptr);
	}	

	if(create_shape(SHAPE_1x2) == SHAPE_AVAILABLE)
	{
		shape_ptr2 = created_shape_ptr;
		shape_ptr2->PositionX = 120;
		shape_ptr2->PositionY = 100;

		set_sprite_tile((*shapes_array[shape_ptr2->ShapeIndex].SpriteArrayPtr)[0], 20);
		set_sprite_tile((*shapes_array[shape_ptr2->ShapeIndex].SpriteArrayPtr)[1], 21);

		shape_ptr2->Place(shape_ptr2);
	}	

	if(create_shape(SHAPE_2x1) == SHAPE_AVAILABLE)
	{
		shape_ptr3 = created_shape_ptr;
		shape_ptr3->PositionX = 60;
		shape_ptr3->PositionY = 120;

		set_sprite_tile((*shapes_array[shape_ptr3->ShapeIndex].SpriteArrayPtr)[0], 16);
		set_sprite_tile((*shapes_array[shape_ptr3->ShapeIndex].SpriteArrayPtr)[1], 17);

		shape_ptr3->Place(shape_ptr3);
	}	

	if(create_shape(SHAPE_1x1) == SHAPE_AVAILABLE)
	{
		shape_ptr4 = created_shape_ptr;
		shape_ptr4->PositionX = 20;
		shape_ptr4->PositionY = 50;

		set_sprite_tile((*shapes_array[shape_ptr4->ShapeIndex].SpriteArrayPtr)[0U], 17);

		shape_ptr4->Place(shape_ptr4);
	}	

	if(create_shape(SHAPE_2x3) == SHAPE_AVAILABLE)
	{
		shape_ptr5 = created_shape_ptr;
		shape_ptr5->PositionX = 20;
		shape_ptr5->PositionY = 30;

		set_sprite_tile((*shapes_array[shape_ptr5->ShapeIndex].SpriteArrayPtr)[0U], 0);
		set_sprite_tile((*shapes_array[shape_ptr5->ShapeIndex].SpriteArrayPtr)[1U], 1);
		set_sprite_tile((*shapes_array[shape_ptr5->ShapeIndex].SpriteArrayPtr)[2U], 16);
		set_sprite_tile((*shapes_array[shape_ptr5->ShapeIndex].SpriteArrayPtr)[3U], 17);
		set_sprite_tile((*shapes_array[shape_ptr5->ShapeIndex].SpriteArrayPtr)[4U], 32);
		set_sprite_tile((*shapes_array[shape_ptr5->ShapeIndex].SpriteArrayPtr)[5U], 33);

		shape_ptr5->Place(shape_ptr5);
	}	

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
	shape_ptr->PositionX = shape_ptr->PositionX + 1;
	shape_ptr->Place(shape_ptr);

	shape_ptr2->PositionX = shape_ptr2->PositionX - 1;
	shape_ptr2->Place(shape_ptr2);

	shape_ptr3->PositionX = shape_ptr3->PositionX + 1;
	shape_ptr3->Place(shape_ptr3);

	shape_ptr4->PositionX = shape_ptr4->PositionX + 1;
	shape_ptr4->PositionY = shape_ptr4->PositionY + 1;
	shape_ptr4->Place(shape_ptr4);

	shape_ptr5->PositionY = shape_ptr5->PositionY + 1;
	shape_ptr5->Place(shape_ptr5);

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

