#include "player.h"
#include "main.h"
#include "levels.h"

INT16 player_world_x = 48;
INT16 player_world_y = 230;

UINT8 player_scr_x = 0U;
UINT8 player_scr_y = 0U;

UINT8 camera_x = 0U;
UINT8 camera_y = 0U;

UINT8 player_sub_x = 0U;
UINT8 player_sub_y = 0U;

extern UINT8 player_sprite_num = 0U;		/* which sprite number the player occupies, should always be 0 */
extern UINT8 player_sprite_tile = 0U; /* the tile data for player graphic */

const UINT8 player_speed = 2U;

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
	if(JOY_DOWN(BTN_UP))
	{
		player_world_y -= player_speed;
	}
	else if(JOY_DOWN(BTN_DOWN))
	{
		player_world_y += player_speed;
	}

	if(JOY_DOWN(BTN_LEFT))
	{
		player_world_x -= player_speed;
	}
	else if(JOY_DOWN(BTN_RIGHT))
	{
		player_world_x += player_speed;
	}

	//player_scr_x = player_world_x - SCR_X;
	//player_scr_y = player_world_y - SCR_Y;  

	/* calculate our camera position from the new player position */

	/*
	if(JOY_DOWN(BTN_SELECT))
	{
		if(JOY_DOWN(BTN_UP))
		{
			tempSCRY = SCR_Y - player_speed;
			if(SCR_Y < 128U && tempSCRY > 128U) 
			{
				SCR_Y = 0;
			}
			else
			{
				SCR_Y = tempSCRY;
			}
		}
		else if(JOY_DOWN(BTN_DOWN))
		{
			tempSCRY = (SCR_Y + SCREEN_HEIGHT) + player_speed;
			if((SCR_Y + SCREEN_HEIGHT) > 128U && tempSCRY < 128U) 
			{
				SCR_Y = 255U - SCREEN_HEIGHT;
			}
			else
			{
				SCR_Y = (tempSCRY - SCREEN_HEIGHT);
			}
		}

		if(JOY_DOWN(BTN_LEFT)) 
		{
			tempSCRX = SCR_X - player_speed;
			if(SCR_X < 128U && tempSCRX > 128U) 
			{
				SCR_X = 0;
			}
			else
			{
				SCR_X -= player_speed;
			}
		}
		else if(JOY_DOWN(BTN_RIGHT))
		{
			tempSCRX = (SCR_X + SCREEN_WIDTH) + player_speed;
			if((SCR_X + SCREEN_WIDTH) > 128U && tempSCRX < 128U) 
			{
				SCR_X = 255U - SCREEN_WIDTH;
			}
			else
			{
				SCR_X += player_speed;
			}
		}
	}
	*/
}

INT16 tempx;/* is there a way to handle this with 8 bit? */
INT16 tempy;
void update_camera() 
{
	tempx = player_world_x - CAMERA_OFFSET_X;
	tempy = player_world_y - CAMERA_OFFSET_Y;

	/* clamp to the VRAM bounds for now, todo: level bounds */
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
	} else if (tempy >= CAMERA_BTM_BOUND)
	{
		tempy = CAMERA_BTM_BOUND;
	}

	camera_x = (UINT8)tempx;
	camera_y = (UINT8)tempy; // todo: fix this if X clamp works properly
}