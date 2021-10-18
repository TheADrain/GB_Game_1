#include "player.h"
#include "main.h"
#include "levels.h"
#include "collision.h"
#include "actors.h"
#include "anims/anims_player.h"
#include "actors.h"

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

UINT8 player_state = 0U;

UINT8 dash_timer = 0U;

extern UINT8 player_sprite_num = 0U;		/* which sprite number the player occupies, should always be 0 */
extern UINT8 player_sprite_tile = 0U; /* the tile data for player graphic */

UINT8 u8Temp1 = 0U;
UINT8 u8Temp2 = 0U;
INT8 i8Temp1 = 0;
INT8 i8Temp2 = 0;
UINT16 u16Temp1 = 0;
UINT16 u16Temp2 = 0;

extern struct ACTOR * player_actor = 0U;

extern UINT8 player_lvl_complete_check_timer = 0U;

void init_player_sprite()
{
	put_player_at_spawn_point();
	
	/* create the player sprite */
	if(create_actor(ACTOR_PLAYER) == ACTOR_AVAILABLE)
	{
		player_actor = created_actor_ptr;
		player_actor->Initialize = &InitializePlayerActor;
		player_actor->Update = &UpdatePlayerActor;

		player_actor->Initialize(player_actor);
	}

	player_state = PLAYER_STATE_IDLE;
}

void put_player_at_spawn_point()
{
	player_world_x = level_datas[CUR_LEVEL_DATA_IDX].PlayerSpawnX;
	player_world_y = level_datas[CUR_LEVEL_DATA_IDX].PlayerSpawnY;
}

void cleanup_player_sprite()
{
	player_world_x = 0U;
	player_world_y = 0U;
	release_actor(player_actor);
}

void InitializePlayerActor(struct ACTOR* a)
{
	/* Set the tiles for the first frame */
	SWITCH_ROM_MBC1(BANK_GRAPHICS_DATA_1);
	SetActorAnim(a, &anim_player_idle);
} 

void UpdatePlayerActor(struct ACTOR* a)
{
	/*player_actor->PositionX = (player_world_x - SCX_REG);
	player_actor->PositionY = (player_world_y - SCY_REG);
	
	SWITCH_ROM_MBC1(BANK_GRAPHICS_DATA_1);
	UpdateActorAnim(a, &anim_player_idle);*/
}

void manual_update_player_sprite()
{
	player_actor->PositionX = (player_world_x - camera_x) + PLAYER_SPRITE_X_OFFSET;
	player_actor->PositionY = (player_world_y - camera_y);
	SWITCH_ROM_MBC1(BANK_GRAPHICS_DATA_1);
	UpdateActorAnim(player_actor, &anim_player_idle);
}

void update_player() 
{
#ifdef DEBUG_MOVEMENT

	if (JOY_DOWN(BTN_LEFT))
	{
			player_world_x = player_world_x - 1;
	}
	else if (JOY_DOWN(BTN_RIGHT))
	{
			player_world_x = player_world_x + 1;
	}

	if (JOY_DOWN(BTN_UP))
	{
		player_world_y = player_world_y - 1;
	}
	else if (JOY_DOWN(BTN_DOWN))
	{
		player_world_y = player_world_y + 1;
	}

#else

	switch(player_state)
	{
		case PLAYER_STATE_IDLE:
			HandlePlayerStateIdle();
			break;

		case PLAYER_STATE_JUMPING:
			HandlePlayerStateJumping();
			break;

		case PLAYER_STATE_FALLING:
			HandlePlayerStateFalling();
			break;

		case PLAYER_STATE_DASHING:
			HandlePlayerDashing();
			break;

		case PLAYER_STATE_DROPPING_FROM_PLATFORM:
			HandlePlayerDroppingFromPlatform();
			break;

		case PLAYER_STATE_HANGING_FROM_PLATFORM:
			HandlePlayerHangingFromPlatform();
			break;
	}

#endif

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

	/* handle actor flipping x */
	if (player_speed_x > 0)
	{
		player_actor->FlipX = 0U;
	}
	else if (player_speed_x < 0)
	{
		player_actor->FlipX = 1U;
	}
}

void HandlePlayerStateIdle()
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
		player_state = PLAYER_STATE_FALLING;
		jump_buffer = JUMP_GRACE_BUFFER;
		return;
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

				player_state = PLAYER_STATE_JUMPING;
				return;
			}
		}
	}
}

void HandlePlayerStateJumping()
{
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

	/* gravity */
	if(player_intertia_y < 0)
	{
		player_intertia_y += RISING_GRAVITY_CONSTANT;
	}
	else
	{
		player_state = PLAYER_STATE_FALLING;
		return;
	}
	
	player_move_y += player_intertia_y;
}

void HandlePlayerStateFalling()
{
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

	/* gravity */
	player_intertia_y += FALLING_GRAVITY_CONSTANT;

	if(player_intertia_y > PLAYER_TERMINAL_VEL)
	{
		player_intertia_y = PLAYER_TERMINAL_VEL;
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

			player_state = PLAYER_STATE_JUMPING;
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

	TestGrounded();

	if(player_grounded == GROUNDED)
	{
		if(jump_last_pressed > 0U && jump_last_pressed <= REJUMP_BUFFER)
		{
			player_intertia_y = -PLAYER_JUMP_INERTIA;
			player_move_y += player_intertia_y;
			player_grounded = 0U;

			player_state = PLAYER_STATE_JUMPING;
			return;
		}
		else
		{
			player_state = PLAYER_STATE_IDLE;
		}
	}
}

void HandlePlayerDashing()
{

}

void HandlePlayerDroppingFromPlatform()
{

}

void HandlePlayerHangingFromPlatform()
{

}

void HandleCollisionHorizontal()
{
	UINT8 collision_type = 0U;

	if(player_move_x < 0)
	{
		/* test high left point */
		UINT16 test_x = player_world_x - PLAYER_HALF_WIDTH;
		UINT16 test_y = (player_world_y) - PLAYER_HEIGHT_CHECK_1;
		collision_grid_test_x = test_x;
		collision_grid_test_y = test_y;

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingLeft(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			player_world_x += (TILE_SIZE-((test_x) % TILE_SIZE));
			return;
		}
		else if(collision_type == COLLISION_RESULT_SOLID_NON_GRID_ALIGNED)
		{
			player_world_x = prev_player_world_x;
			return;
		}

		/* test low left point */
		collision_grid_test_x = test_x;
		test_y = player_world_y - PLAYER_HEIGHT_CHECK_2;
		collision_grid_test_y = test_y;

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingLeft(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			player_world_x += (TILE_SIZE-(test_x % TILE_SIZE));
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
		/* test high right point */
		UINT16 test_x = (player_world_x + PLAYER_HALF_WIDTH);
		UINT16 test_y = (player_world_y - PLAYER_HEIGHT_CHECK_1);	
		collision_grid_test_x = test_x;
		collision_grid_test_y = test_y;

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingRight(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			player_world_x -= (test_x % TILE_SIZE);
			return;
		}
		else if(collision_type == COLLISION_RESULT_SOLID_NON_GRID_ALIGNED)
		{
			player_world_x = prev_player_world_x;
			return;
		}

		/* test low right point */
		collision_grid_test_x = test_x;
		test_y = (player_world_y - PLAYER_HEIGHT_CHECK_2);
		collision_grid_test_y = test_y;

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingRight(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			player_world_x -= (test_x % TILE_SIZE);
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
		UINT16 test_x = player_world_x - (PLAYER_HALF_WIDTH-1);
		UINT16 test_y = player_world_y - PLAYER_HEIGHT;
		collision_grid_test_x = test_x;
		collision_grid_test_y = test_y;

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingUp(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			player_world_y += (TILE_SIZE - (test_y % TILE_SIZE));
			player_intertia_y = 0;
			return;
		}
		else if(collision_type == COLLISION_RESULT_SOLID_NON_GRID_ALIGNED)
		{
			player_world_y = prev_player_world_y;
			return;
		}

		/* test top right point */
		test_x = player_world_x + (PLAYER_HALF_WIDTH-1);
		collision_grid_test_x = test_x;
		collision_grid_test_y = test_y;	

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingUp(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			player_world_y += (TILE_SIZE - (test_y % TILE_SIZE));
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
		UINT16 test_x = player_world_x - (PLAYER_HALF_WIDTH-1);
		UINT16 test_y = player_world_y;
		collision_grid_test_x = test_x;
		collision_grid_test_y = test_y;

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingDown(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			/* zero out the motion for now, in future we'll want a motion-aware calculation */
			player_grounded = GROUNDED;
			player_intertia_y = 0;
			player_fell = 0U;
			jump_buffer = 0U;
			player_world_y -= (((test_y) % TILE_SIZE));
			return;
		}
		else if(collision_type == COLLISION_RESULT_SOLID_NON_GRID_ALIGNED)
		{
			player_world_y = prev_player_world_y;
			return;
		}

		/* test bottom right point */
		test_x = player_world_x + (PLAYER_HALF_WIDTH-1);
		collision_grid_test_x = test_x;
		collision_grid_test_y = test_y;

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingDown(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_SOLID)
		{
			/* zero out the motion for now, in future we'll want a motion-aware calculation */
			player_grounded = GROUNDED;
			player_intertia_y = 0;
			player_fell = 0U;
			jump_buffer = 0U;
			player_world_y -= (((test_y) % TILE_SIZE));
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
	collision_grid_test_x = (UINT16)(player_world_x - (PLAYER_HALF_WIDTH-1));
	collision_grid_test_y = (UINT16)(player_world_y);

	UINT8 collision_type = 0U;

	TestCollisionAtWorldPosition();
	collision_type = TestCollisionType_MovingDown(collision_grid_test_result);
	if(collision_type == COLLISION_RESULT_NONE)
	{
		/* test bottom right point */
		collision_grid_test_x = (player_world_x + (PLAYER_HALF_WIDTH-1));
		collision_grid_test_y = (player_world_y);

		TestCollisionAtWorldPosition();
		collision_type = TestCollisionType_MovingDown(collision_grid_test_result);
		if(collision_type == COLLISION_RESULT_NONE)
		{
			/* we are no longer grounded */
			player_grounded = NOT_GROUNDED;
			
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
	u16Temp1 = (UINT16)collision_grid_test_y * (UINT16)level_datas[CUR_LEVEL_DATA_IDX].Width;
	u16Temp1 += (UINT16)collision_grid_test_x;

	/* bank switch before testing this position! */
	SWITCH_ROM_MBC1(level_datas[CUR_LEVEL_DATA_IDX].RomBank);
	collision_grid_test_result = level_datas[CUR_LEVEL_DATA_IDX].CollisionMap[u16Temp1];

}

