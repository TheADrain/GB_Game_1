#include "player.h"
#include "main.h"
#include "levels.h"
#include "collision.h"

UINT16 player_position_x = 48*SUBPIXELS;
UINT16 player_position_y = 64*SUBPIXELS;

UINT16 player_world_x = 48;
UINT16 player_world_y = 64;

UINT16 prev_player_world_x = 48;
UINT16 prev_player_world_y = 64;

INT8 player_move_x = 0;
INT8 player_move_y = 0;

INT8 player_intertia_y = 0;
UINT8 player_grounded = 0U;

UINT8 jump_buffer = 0U;
UINT8 jump_last_pressed = 0U;
UINT8 player_fell = 0U;

INT8 player_speed_x = 0;

UINT8 player_scr_x = 0U;
UINT8 player_scr_y = 0U;

UINT16 camera_x = 0U;
UINT16 camera_y = 0U;

UINT8 player_sub_x = 0U;
UINT8 player_sub_y = 0U;

extern UINT8 player_sprite_num = 0U;		/* which sprite number the player occupies, should always be 0 */
extern UINT8 player_sprite_tile = 0U; /* the tile data for player graphic */

UINT8 u8Temp1 = 0U;
UINT8 u8Temp2 = 0U;
INT8 i8Temp1 = 0;
INT8 i8Temp2 = 0;
UINT16 u16Temp1 = 0;
UINT16 u16Temp2 = 0;

void init_player_sprite()
{
	set_sprite_tile(player_sprite_num, player_sprite_tile);

	player_world_x = levels[CUR_LEVEL].PlayerSpawnX;
	player_world_y = levels[CUR_LEVEL].PlayerSpawnY;
}

void init_game_camera()
{
	update_camera();
}

void update_player() 
{
#ifdef DEBUG_MOVEMENT

	/* DEBUG_MOVEMENT */
	if(JOY_DOWN(BTN_LEFT))
	{
		player_world_x -= 1;
	}
	else if(JOY_DOWN(BTN_RIGHT))
	{
		player_world_x += 1;
	}

	if(JOY_DOWN(BTN_UP))
	{
		player_world_y -= 1;
	}
	else if(JOY_DOWN(BTN_DOWN))
	{
		player_world_y += 1;
	}

#else /* DEBUG_MOVEMENT */

	/* find movement vector */
	if(JOY_DOWN(BTN_LEFT))
	{
		player_speed_x -= PLAYER_ACCEL_X;
		if(player_speed_x < -PLAYER_MAX_SPEED_H)
		{
			player_speed_x = -PLAYER_MAX_SPEED_H;
		}
	}
	else if(JOY_DOWN(BTN_RIGHT))
	{
		player_speed_x += PLAYER_ACCEL_X;
		if(player_speed_x > PLAYER_MAX_SPEED_H)
		{
			player_speed_x = PLAYER_MAX_SPEED_H;
		}
	}
	else
	{
		if(player_speed_x > 0)
		{
			player_speed_x -= PLAYER_DECEL_X;
			if(player_speed_x < 0)
			{
				player_speed_x = 0;
			}
		}
		else if(player_speed_x < 0)
		{
			player_speed_x += PLAYER_DECEL_X;
			if(player_speed_x > 0)
			{
				player_speed_x = 0;
			}
		}
	}

	player_move_x += player_speed_x;
	
	if(player_grounded == NOT_GROUNDED)
	{
		/* gravity */
		if(player_intertia_y < 0)
		{
			player_intertia_y += RISING_GRAVITY_CONSTANT;
		}
		else
		{
			player_intertia_y += FALLING_GRAVITY_CONSTANT;

			if(player_intertia_y > PLAYER_TERMINAL_VEL)
			{
				player_intertia_y = PLAYER_TERMINAL_VEL;
			}
		}
		
		player_move_y += player_intertia_y;

		/* test to see if we need to jump late */
		if(jump_buffer > 0)
		{
			jump_buffer--;
			if(JOY_PRESSED(BTN_A))
			{
				player_intertia_y = -PLAYER_JUMP_INERTIA;
				player_move_y += player_intertia_y;
				player_grounded = 0U;
			}
		}

		/* test to see if jump was pressed early */
		if(jump_last_pressed == 0)
		{
			if(JOY_PRESSED(BTN_A))
			{
				jump_last_pressed = 1;
			}
		}
		else
		{
			jump_last_pressed++;
			if(jump_last_pressed > REJUMP_BUFFER)
			{
				jump_last_pressed = 0U;
			}
		}
	}
	else
	{
		TestGrounded();

		if(player_grounded == GROUNDED)
		{
			if(JOY_PRESSED(BTN_A))
			{
				player_intertia_y = -PLAYER_JUMP_INERTIA;
				player_move_y += player_intertia_y;
				player_grounded = 0U;
			}
		}
	}

	prev_player_world_x = player_world_x;
	prev_player_world_y = player_world_y;

	/* resolve horizontal collision first */
	player_world_x += player_move_x / SUBPIXELS;
	HandleCollisionHorizontal();

	player_world_y += player_move_y / SUBPIXELS;
	HandleCollisionVertical();

	/* subtract whole pixels from the move vector only after collision has been handled */
	player_move_x = player_move_x % SUBPIXELS;
	player_move_y = player_move_y % SUBPIXELS;

#endif /* DEBUG_MOVEMENT */
}

void HandleCollisionHorizontal()
{
/* TODO: SIMPLIFY THIS CODE DOWN TO SOME!!!!*/
	UINT8 collision_type = 0U;

	if(player_move_x < 0)
	{
		/* test top left point */
		collision_grid_test_x = (player_world_x + (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH));
		collision_grid_test_y = (player_world_y);

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingLeft(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			player_world_x += (TILE_SIZE-((player_world_x + (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH)) % TILE_SIZE));
			return;
		}
		else if(collision_type == COLLISION_RESULT_SOLID_NON_GRID_ALIGNED)
		{
			player_world_x = prev_player_world_x;
			return;
		}

		/* test bottom left point */
		collision_grid_test_x = (player_world_x + (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH));
		collision_grid_test_y = (player_world_y + PLAYER_HEIGHT-1);

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingLeft(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			player_world_x += (TILE_SIZE - ((player_world_x + (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH)) % TILE_SIZE));
			return;
		}
		else if(collision_type == COLLISION_RESULT_SOLID_NON_GRID_ALIGNED)
		{
			player_world_x = prev_player_world_x;
			return;
		}

		/* test mid left point */
		collision_grid_test_x = (player_world_x + (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH));
		collision_grid_test_y = (player_world_y + PLAYER_HALF_HEIGHT);

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingLeft(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			player_world_x += (TILE_SIZE-((player_world_x + (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH)) % TILE_SIZE));
			return;
		}
		else if(collision_type == COLLISION_RESULT_SOLID_NON_GRID_ALIGNED)
		{
			player_world_x = prev_player_world_x;
			return;
		}
	}
	else if(player_move_x > 0)
	{
		/* test top right point */
		collision_grid_test_x = (player_world_x + (PLAYER_SPRITE_WIDTH - (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH))-1);
		collision_grid_test_y = (player_world_y);	

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingRight(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			player_world_x -= (((player_world_x + (PLAYER_SPRITE_WIDTH - (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH))) % TILE_SIZE));
			return;
		}
		else if(collision_type == COLLISION_RESULT_SOLID_NON_GRID_ALIGNED)
		{
			player_world_x = prev_player_world_x;
			return;
		}

		/* test bottom right point */
		collision_grid_test_x = (player_world_x + (PLAYER_SPRITE_WIDTH - (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH))-1);
		collision_grid_test_y = (player_world_y + PLAYER_HEIGHT-1);

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingRight(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			player_world_x -= (((player_world_x + (PLAYER_SPRITE_WIDTH - (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH))) % TILE_SIZE));
			return;
		}
		else if(collision_type == COLLISION_RESULT_SOLID_NON_GRID_ALIGNED)
		{
			player_world_x = prev_player_world_x;
			return;
		}

		/* test mid right point */
		collision_grid_test_x = (player_world_x + (PLAYER_SPRITE_WIDTH - (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH))-1);
		collision_grid_test_y = (player_world_y + PLAYER_HALF_HEIGHT);

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingRight(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			player_world_x -= (((player_world_x + (PLAYER_SPRITE_WIDTH - (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH))) % TILE_SIZE));
			return;
		}
		else if(collision_type == COLLISION_RESULT_SOLID_NON_GRID_ALIGNED)
		{
			player_world_x = prev_player_world_x;
			return;
		}
	}
}

void HandleCollisionVertical()
{
	UINT8 collision_type = 0U;

	if(player_move_y < 0)
	{
		/* test top left point */
		collision_grid_test_x = (UINT16)(player_world_x + (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH));
		collision_grid_test_y = (UINT16)(player_world_y);

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingUp(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			player_world_y += (TILE_SIZE - (player_world_y % TILE_SIZE));
			player_intertia_y = 0;
			return;
		}
		else if(collision_type == COLLISION_RESULT_SOLID_NON_GRID_ALIGNED)
		{
			player_world_y = prev_player_world_y;
			return;
		}

		/* test top right point */
		collision_grid_test_x = (UINT16)(player_world_x + (PLAYER_SPRITE_WIDTH - (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH))-1);
		collision_grid_test_y = (player_world_y);	

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingUp(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			player_world_y += (TILE_SIZE - (player_world_y % TILE_SIZE));
			player_intertia_y = 0;
			return;
		}
		else if(collision_type == COLLISION_RESULT_SOLID_NON_GRID_ALIGNED)
		{
			player_world_y = prev_player_world_y;
			return;
		}
	}
	else if(player_move_y > 0)
	{
		/* test bottom left point */
		collision_grid_test_x = (UINT16)(player_world_x + (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH));
		collision_grid_test_y = (UINT16)(player_world_y + PLAYER_HEIGHT);

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingDown(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			/* zero out the motion for now, in future we'll want a motion-aware calculation */
			player_grounded = GROUNDED;
			player_intertia_y = 0;
			player_fell = 0U;
			jump_buffer = 0U;
			player_world_y -= (((player_world_y + PLAYER_HEIGHT) % TILE_SIZE));
			return;
		}
		else if(collision_type == COLLISION_RESULT_SOLID_NON_GRID_ALIGNED)
		{
			player_world_y = prev_player_world_y;
			return;
		}

		/* test bottom right point */
		collision_grid_test_x = (UINT16)(player_world_x + (PLAYER_SPRITE_WIDTH - (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH))-1);
		collision_grid_test_y = (player_world_y + PLAYER_HEIGHT);

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingDown(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			/* zero out the motion for now, in future we'll want a motion-aware calculation */
			player_grounded = GROUNDED;
			player_intertia_y = 0;
			player_fell = 0U;
			jump_buffer = 0U;
			player_world_y -= (((player_world_y + PLAYER_HEIGHT) % TILE_SIZE));
			return;
		}
		else if(collision_type == COLLISION_RESULT_SOLID_NON_GRID_ALIGNED)
		{
			player_world_y = prev_player_world_y;
			return;
		}
	}
}

void TestGrounded()
{
	/* test bottom left point */
	collision_grid_test_x = (UINT16)(player_world_x);
	collision_grid_test_y = (UINT16)(player_world_y + PLAYER_HEIGHT + 4);

	UINT8 collision_type = 0U;

	TestCollisionAtWorldPosition();
	collision_type = TestCollisionType_MovingDown(collision_grid_test_result);
	if(collision_type == COLLISION_RESULT_NONE)
	{
		/* test bottom right point */
		collision_grid_test_x = (player_world_x + PLAYER_WIDTH);
		collision_grid_test_y = (player_world_y + PLAYER_HEIGHT + 4);

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingDown(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_NONE)
		{
			/* we are no longer grounded */
			player_grounded = NOT_GROUNDED;
			jump_buffer = JUMP_GRACE_BUFFER;
		}
	}
}

UINT16 collision_grid_test_x = 0;
UINT16 collision_grid_test_y = 0;
UINT8 collision_grid_test_result = COL_EMPTY;

void TestCollisionAtWorldPosition()
{
	/* tile size is 8, so we can bit shift right by 3 to divide by 8 */
	collision_grid_test_x = collision_grid_test_x >> 3;// COLLIDER_TILE_SIZE; /* will this round down? */
	collision_grid_test_y = collision_grid_test_y >> 3;// COLLIDER_TILE_SIZE;

	TestCollisionAtGridPosition();
} 

void TestCollisionAtGridPosition()
{
	collision_grid_test_result = 0U;

	/* check level collision data at grid point for a collision tile */
	u16Temp1 = (UINT16)collision_grid_test_y * (UINT16)levels[CUR_LEVEL].Width;
	u16Temp1 += (UINT16)collision_grid_test_x;

	/* bank switch before testing this position! */
	SWITCH_ROM_MBC1(levels[CUR_LEVEL].RomBank);
	collision_grid_test_result = levels[CUR_LEVEL].CollisionMap[u16Temp1];

}

INT16 tempx;
INT16 tempy;
UINT16 widthOffset;
INT16 rightBound;
INT16 rightTileEdge = BKG_WIDTH;

INT16 topBound;
INT16 topTileEdge = BKG_HEIGHT;

INT16 bottomBound;

UINT8 cameraDelta = 128;

UINT16 scroll_x_temp = 0U;
UINT16 scroll_y_temp = 0U;
unsigned char * scrl_data_ptr;
UINT8 scrl_i = 0;

// store tile load commands until next vblank
UBYTE stored_tile_load_command = 0U;
UINT8 stored_tile_load_bkg_x = 0U;
UINT8 stored_tile_load_bkg_y = 0U;
UINT8 stored_tile_load_bkg_w = 0U;
UINT8 stored_tile_load_bkg_h = 0U;
unsigned char * stored_scrl_dat_ptr = 0U;

void update_camera() 
{
	tempx = player_world_x - CAMERA_OFFSET_X;
	tempy = player_world_y - CAMERA_OFFSET_Y;
	
	if(levels[CUR_LEVEL].MapType == MAP_HORIZONTAL)
	{
		handle_scroll_horizontal();
	}
	else
	{
		handle_scroll_vertical();
	}

	camera_x = tempx;
	camera_y = tempy; 
}

void handle_scroll_horizontal()
{
	/* clamp to the level width bounds */
	rightBound = (CUR_MAP_WIDTH * TILE_SIZE) - SCREEN_WIDTH;

	if(tempx < 0 )
	{
		tempx = 0;
	} else if (tempx >= rightBound)
	{
		tempx = rightBound;
	}

	if(tempy < 0)
	{
		tempy = 0;
	} else if (tempy >= CAMERA_BTM_BOUND)
	{
		tempy = CAMERA_BTM_BOUND;
	}

	cameraDelta += (tempx - camera_x);

	if(tempx > camera_x)
	{
		/* moving right */
		if(cameraDelta > (128+8))
		{
			cameraDelta -= 8;

			UINT16 scx = (tempx >> 3) + 24;
			scroll_x_temp = (scx)%BKG_WIDTH;
			scrl_data_ptr = levels[CUR_LEVEL].MapTileData;
			scrl_data_ptr += (CUR_MAP_HEIGHT * scx);

			stored_tile_load_command = 1U;
			stored_tile_load_bkg_x = scroll_x_temp;
			stored_tile_load_bkg_y = 0U;
			stored_tile_load_bkg_w = 1U;
			stored_tile_load_bkg_h = CUR_MAP_HEIGHT;
			stored_scrl_dat_ptr = scrl_data_ptr;
			/* this gets set in main.c DoGraphics */
		}
	}
	else if (tempx < camera_x)
	{
		/* moving left */
		if(cameraDelta < (128-8))
		{
			cameraDelta += 8;

			UINT16 scx = (tempx >> 3) - 4;
			scroll_x_temp = (scx)%BKG_WIDTH;
			scrl_data_ptr = levels[CUR_LEVEL].MapTileData;
			scrl_data_ptr += (CUR_MAP_HEIGHT * scx);

			stored_tile_load_command = 1U;
			stored_tile_load_bkg_x = scroll_x_temp;
			stored_tile_load_bkg_y = 0U;
			stored_tile_load_bkg_w = 1U;
			stored_tile_load_bkg_h = CUR_MAP_HEIGHT;
			stored_scrl_dat_ptr = scrl_data_ptr;
			/* this gets set in main.c DoGraphics */
		}
	}
}

void handle_scroll_vertical()
{
	/* clamp to the level width bounds */
	bottomBound = (CUR_MAP_HEIGHT * TILE_SIZE) - SCREEN_HEIGHT;

	if(tempx < 0 )
	{
		tempx = 0;
	} else if (tempx >= CAMERA_RIGHT_BOUND)
	{
		tempx = CAMERA_RIGHT_BOUND;
	}

	if(tempy < 0)
	{
		tempy = 0;
	} else if (tempy >= bottomBound)
	{
		tempy = bottomBound;
	}

	cameraDelta += (tempy - camera_y);

	if(tempy > camera_y)
	{
		/* moving down */
		if(cameraDelta >= (128+8))
		{
			cameraDelta -= 8;

			UINT16 scy = (tempy >> 3) + 24;
			scroll_y_temp = (scy)%BKG_HEIGHT;
			unsigned char* mapData = levels[CUR_LEVEL].MapTileData;
			mapData += (CUR_MAP_WIDTH * scy);

			stored_tile_load_command = 1U;
			stored_tile_load_bkg_x = 0U;
			stored_tile_load_bkg_y = scroll_y_temp;
			stored_tile_load_bkg_w = CUR_MAP_WIDTH;
			stored_tile_load_bkg_h = 1U;
			stored_scrl_dat_ptr = mapData;

			/* this gets set in main.c DoGraphics */
		}
	}
	else if (tempy < camera_y)
	{
		/* moving up */
		if(cameraDelta <= (128-8))
		{
			cameraDelta += 8;

			UINT16 scy = (tempy >> 3) - 4;			
			scroll_y_temp = (scy)%BKG_HEIGHT;
			unsigned char* mapData = levels[CUR_LEVEL].MapTileData;
			mapData += (CUR_MAP_WIDTH * scy);

			stored_tile_load_command = 1U;
			stored_tile_load_bkg_x = 0U;
			stored_tile_load_bkg_y = scroll_y_temp;
			stored_tile_load_bkg_w = CUR_MAP_WIDTH;
			stored_tile_load_bkg_h = 1U;
			stored_scrl_dat_ptr = mapData;

			/* this gets set in main.c DoGraphics */
		}
	}
}