#include "scrolling.h"

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

void init_game_camera()
{
	rightBound = (CUR_MAP_WIDTH * TILE_SIZE) - SCREEN_WIDTH;
	bottomBound = (CUR_MAP_HEIGHT * TILE_SIZE) - SCREEN_HEIGHT;

	/* reset the camera to the players spawn position to prevent a large scroll delta on the first frame */
	tempx = player_world_x - CAMERA_OFFSET_X;
	tempy = player_world_y - CAMERA_OFFSET_Y;

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
	} else if (tempy >= bottomBound)
	{
		tempy = bottomBound;
	}

	camera_x = tempx;
	camera_y = tempy; 
}

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