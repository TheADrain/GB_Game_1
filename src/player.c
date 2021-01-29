#include "player.h"
#include "main.h"
#include "levels.h"

UINT16 player_position_x = 48*SUBPIXELS;
UINT16 player_position_y = 64*SUBPIXELS;

UINT16 player_world_x = 48;
UINT16 player_world_y = 64;

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
}

void init_game_camera()
{
	update_camera();
}

void update_player() 
{
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

	/* resolve horizontal collision first */
	player_world_x += player_move_x / SUBPIXELS;
	HandleCollisionHorizontal();

	player_world_y += player_move_y / SUBPIXELS;
	HandleCollisionVertical();

	/* subtract whole pixels from the move vector only after collision has been handled */
	player_move_x = player_move_x % SUBPIXELS;
	player_move_y = player_move_y % SUBPIXELS;
}

void HandleCollisionHorizontal()
{
/* TODO: SIMPLIFY THIS CODE DOWN TO SOME!!!!

all these offsets and shit could be done ONCE (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH)
*/
	if(player_move_x < 0)
	{
		/* test top left point */
		collision_grid_test_x = (player_world_x + (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH));
		collision_grid_test_y = (player_world_y);

		TestCollisionAtWorldPosition();
		if(collision_grid_test_result == COLLISION_FOUND)
		{
			player_world_x += (TILE_SIZE-((player_world_x + (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH)) % TILE_SIZE));
			return;
		}

		/* test bottom left point */
		collision_grid_test_x = (player_world_x + (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH));
		collision_grid_test_y = (player_world_y + PLAYER_HEIGHT-1);

		TestCollisionAtWorldPosition();
		if(collision_grid_test_result == COLLISION_FOUND)
		{
			player_world_x += (TILE_SIZE - ((player_world_x + (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH)) % TILE_SIZE));
			return;
		}

		/* test mid left point */
		collision_grid_test_x = (player_world_x + (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH));
		collision_grid_test_y = (player_world_y + PLAYER_HALF_HEIGHT);

		TestCollisionAtWorldPosition();
		if(collision_grid_test_result == COLLISION_FOUND)
		{
			player_world_x += (TILE_SIZE-((player_world_x + (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH)) % TILE_SIZE));
			return;
		}
	}
	else if(player_move_x > 0)
	{
		/* test top right point */
		collision_grid_test_x = (player_world_x + (PLAYER_SPRITE_WIDTH - (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH))-1);
		collision_grid_test_y = (player_world_y);	

		TestCollisionAtWorldPosition();
		if(collision_grid_test_result == COLLISION_FOUND)
		{
			player_world_x -= (((player_world_x + (PLAYER_SPRITE_WIDTH - (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH))) % TILE_SIZE));
			return;
		}

		/* test bottom right point */
		collision_grid_test_x = (player_world_x + (PLAYER_SPRITE_WIDTH - (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH))-1);
		collision_grid_test_y = (player_world_y + PLAYER_HEIGHT-1);

		TestCollisionAtWorldPosition();
		if(collision_grid_test_result == COLLISION_FOUND)
		{
			player_world_x -= (((player_world_x + (PLAYER_SPRITE_WIDTH - (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH))) % TILE_SIZE));
			return;
		}

		/* test mid right point */
		collision_grid_test_x = (player_world_x + (PLAYER_SPRITE_WIDTH - (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH))-1);
		collision_grid_test_y = (player_world_y + PLAYER_HALF_HEIGHT);

		TestCollisionAtWorldPosition();
		if(collision_grid_test_result == COLLISION_FOUND)
		{
			player_world_x -= (((player_world_x + (PLAYER_SPRITE_WIDTH - (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH))) % TILE_SIZE));
			return;
		}
	}
}

void HandleCollisionVertical()
{
	if(player_move_y < 0)
	{
		/* test top left point */
		collision_grid_test_x = (UINT16)(player_world_x + (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH));
		collision_grid_test_y = (UINT16)(player_world_y);

		TestCollisionAtWorldPosition();
		if(collision_grid_test_result == COLLISION_FOUND)
		{
			player_world_y += (TILE_SIZE - (player_world_y % TILE_SIZE));
			player_intertia_y = 0;
			return;
		}

		/* test top right point */
		collision_grid_test_x = (UINT16)(player_world_x + (PLAYER_SPRITE_WIDTH - (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH))-1);
		collision_grid_test_y = (player_world_y);	

		TestCollisionAtWorldPosition();
		if(collision_grid_test_result == COLLISION_FOUND)
		{
			player_world_y += (TILE_SIZE - (player_world_y % TILE_SIZE));
			player_intertia_y = 0;
			return;
		}
	}
	else if(player_move_y > 0)
	{
		/* test bottom left point */
		collision_grid_test_x = (UINT16)(player_world_x + (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH));
		collision_grid_test_y = (UINT16)(player_world_y + PLAYER_HEIGHT);

		TestCollisionAtWorldPosition();
		if(collision_grid_test_result == COLLISION_FOUND)
		{
			/* zero out the motion for now, in future we'll want a motion-aware calculation */
			player_grounded = GROUNDED;
			player_intertia_y = 0;
			player_fell = 0U;
			jump_buffer = 0U;
			player_world_y -= (((player_world_y + PLAYER_HEIGHT) % TILE_SIZE));
			return;
		}

		/* test bottom right point */
		collision_grid_test_x = (UINT16)(player_world_x + (PLAYER_SPRITE_WIDTH - (PLAYER_SPRITE_WIDTH - PLAYER_WIDTH))-1);
		collision_grid_test_y = (player_world_y + PLAYER_HEIGHT);

		TestCollisionAtWorldPosition();
		if(collision_grid_test_result == COLLISION_FOUND)
		{
			/* zero out the motion for now, in future we'll want a motion-aware calculation */
			player_grounded = GROUNDED;
			player_intertia_y = 0;
			player_fell = 0U;
			jump_buffer = 0U;
			player_world_y -= (((player_world_y + PLAYER_HEIGHT) % TILE_SIZE));
			return;
		}
	}
}

void TestGrounded()
{
	/* test bottom left point */
	collision_grid_test_x = (UINT16)(player_world_x);
	collision_grid_test_y = (UINT16)(player_world_y + PLAYER_HEIGHT + 4);

	TestCollisionAtWorldPosition();
	if(collision_grid_test_result == NO_COLLISION_FOUND)
	{
		/* test bottom right point */
		collision_grid_test_x = (player_world_x + PLAYER_WIDTH);
		collision_grid_test_y = (player_world_y + PLAYER_HEIGHT + 4);

		TestCollisionAtWorldPosition();
		if(collision_grid_test_result == NO_COLLISION_FOUND)
		{
			/* we are no longer grounded */
			player_grounded = NOT_GROUNDED;
			jump_buffer = JUMP_GRACE_BUFFER;
		}
	}
}

UINT16 collision_grid_test_x = 0;
UINT16 collision_grid_test_y = 0;
UINT8 collision_grid_test_result = NO_COLLISION_FOUND;

void TestCollisionAtWorldPosition()
{
	/* decompose the test position into a world grid map point */
	//collision_grid_test_x = collision_grid_test_x / COLLIDER_TILE_SIZE; /* will this round down? */
	//collision_grid_test_y = collision_grid_test_y / COLLIDER_TILE_SIZE;

	/* tile size is 8, so we can bit shift right by 3 to divide by 8 */
	collision_grid_test_x = collision_grid_test_x >> 3;// COLLIDER_TILE_SIZE; /* will this round down? */
	collision_grid_test_y = collision_grid_test_y >> 3;// COLLIDER_TILE_SIZE;

	TestCollisionAtGridPosition();
} 

void TestCollisionAtGridPosition()
{
	collision_grid_test_result = NO_COLLISION_FOUND;

	/* check level collision data at grid point for a collision tile */
	u16Temp1 = (UINT16)collision_grid_test_y * (UINT16)levels[CUR_LEVEL].Width;
	u16Temp1 += (UINT16)collision_grid_test_x;

	/* bank switch before testing this position! */
	SWITCH_ROM_MBC1(levels[CUR_LEVEL].RomBank);

	if(levels[CUR_LEVEL].CollisionMap[u16Temp1] == TILE_SOLID)
	{
		collision_grid_test_result = COLLISION_FOUND;
		
	}
	else
	{
		collision_grid_test_result = NO_COLLISION_FOUND;
	}
}

INT16 tempx;/* is there a way to handle this with 8 bit? */
INT16 tempy;
INT16 rightBound;
INT16 rightTileEdge = BKG_WIDTH;
INT8 cameraDelta = 0;
void update_camera() 
{
	tempx = player_world_x - CAMERA_OFFSET_X;
	tempy = player_world_y - CAMERA_OFFSET_Y;
	rightBound = (CUR_MAP_WIDTH * TILE_SIZE) - SCREEN_WIDTH;

	cameraDelta += (tempx - camera_x);
	/* clamp to the level width bounds */
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

	if(tempx > camera_x)
	{
		/* moving right */
		if(cameraDelta >= TILE_SIZE)
		{
			cameraDelta -= 8;

			UINT8 i = 0;
			UINT16 scx = (tempx >> 3);
			for(i = 0; i < CUR_MAP_HEIGHT; i++)
			{
				set_bkg_tiles((scx+24)%BKG_WIDTH, i, 1, 1, level_tilemap_data+(i*CUR_MAP_WIDTH)+scx+24);
			}
		}
	}
	else
	{
		/* moving left */
		if(cameraDelta <= -TILE_SIZE)
		{
			cameraDelta += 8;

			UINT8 i = 0;
			UINT8 scx = (tempx >> 3);
			for(i = 0; i < CUR_MAP_HEIGHT; i++)
			{
				set_bkg_tiles((scx-4)%BKG_WIDTH, i, 1, 1, level_tilemap_data+(i*CUR_MAP_WIDTH)+(scx-4));
			}
		}
	}

	camera_x = tempx;
	camera_y = tempy; 
}