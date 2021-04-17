#include "scrolling.h"

INT16 tempx;
INT16 tempy;
UINT16 widthOffset;
INT16 camera_right_bound;
INT16 rightTileEdge = BKG_WIDTH;

INT16 camera_top_bound;
INT16 topTileEdge = BKG_HEIGHT;

INT16 camera_bottom_bound;

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
	camera_top_bound = 0U;
	camera_right_bound = (CUR_MAP_WIDTH * TILE_SIZE) - SCREEN_WIDTH;
	camera_bottom_bound = (CUR_MAP_HEIGHT * TILE_SIZE) - SCREEN_HEIGHT;

	cameraDelta = 128U;
	scroll_x_temp = 0U;
	scroll_y_temp = 0U;
	stored_tile_load_command = 0U;
	stored_tile_load_bkg_x = 0U;
	stored_tile_load_bkg_y = 0U;
	stored_tile_load_bkg_w = 0U;
	stored_tile_load_bkg_h = 0U;
	stored_scrl_dat_ptr = 0U;

	/* reset the camera to the players spawn position to prevent a large scroll delta on the first frame */
	tempx = player_world_x - CAMERA_OFFSET_X;
	tempy = player_world_y - CAMERA_OFFSET_Y;

	if(tempx < 0 )
	{
		tempx = 0;
	} else if (tempx >= camera_right_bound)
	{
		tempx = camera_right_bound;
	}

	if(tempy < 0)
	{
		tempy = 0;
	} else if (tempy >= camera_bottom_bound)
	{
		tempy = camera_bottom_bound;
	}

	camera_x = tempx;
	camera_y = tempy;
}

void update_camera() 
{
	tempx = player_world_x - CAMERA_OFFSET_X;
	tempy = player_world_y - CAMERA_OFFSET_Y;
	
	if(level_datas[CUR_LEVEL_DATA_IDX].MapType == MAP_HORIZONTAL)
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
	} else if (tempx >= camera_right_bound)
	{
		tempx = camera_right_bound;
	}

	if(tempy < 0)
	{
		tempy = 0;
	} else if (tempy >= camera_bottom_bound)
	{
		tempy = camera_bottom_bound;
	}

	cameraDelta += (tempx - camera_x);

	if(tempx > camera_x)
	{
		/* moving right */
		if(cameraDelta > (128+8))
		{
			cameraDelta -= 8;

			/* scy is the top scren position in the level, bit-shifted by 3 to get the VRAM position */
			/*  minus how many tiles ahead of the camera to draw */

			UINT16 scx = (tempx >> 3) + 24;
			scroll_x_temp = (scx)%BKG_WIDTH;
			scrl_data_ptr = level_datas[CUR_LEVEL_DATA_IDX].MapTileData;
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

			/* scy is the top scren position in the level, bit-shifted by 3 to get the VRAM position */
			/*  minus how many tiles ahead of the camera to draw */

			UINT16 scx = (tempx >> 3) - 4;
			scroll_x_temp = (scx)%BKG_WIDTH;
			scrl_data_ptr = level_datas[CUR_LEVEL_DATA_IDX].MapTileData;
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
	} else if (tempx >= camera_right_bound)
	{
		tempx = camera_right_bound;
	}

	if(tempy < 0)
	{
		tempy = 0;
	} else if (tempy >= camera_bottom_bound)
	{
		tempy = camera_bottom_bound;
	}

	cameraDelta += (tempy - camera_y);

	if(tempy > camera_y)
	{
		/* moving down */
		if(cameraDelta >= (128+8))
		{
			cameraDelta -= 8;

			/* scy is the top scren position in the level, bit-shifted by 3 to get the VRAM position */
			/*  minus how many tiles ahead of the camera to draw */

			UINT16 scy = (tempy >> 3) + 22;
			scy = scy - 1; /* draw a strip of 2 incase we are high enough velocity to skip one */
			scroll_y_temp = (scy)%BKG_HEIGHT;
			scrl_data_ptr = level_datas[CUR_LEVEL_DATA_IDX].MapTileData;
			scrl_data_ptr += (CUR_MAP_WIDTH * scy);

			stored_tile_load_command = 1U;
			stored_tile_load_bkg_x = 0U;
			stored_tile_load_bkg_y = scroll_y_temp;
			stored_tile_load_bkg_w = CUR_MAP_WIDTH;
			stored_tile_load_bkg_h = 2U;
			stored_scrl_dat_ptr = scrl_data_ptr;

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
			scy = scy - 1; /* draw a strip of 2 incase we are high enough velocity to skip one */
			scroll_y_temp = (scy)%BKG_HEIGHT;
			scrl_data_ptr = level_datas[CUR_LEVEL_DATA_IDX].MapTileData;
			scrl_data_ptr += (CUR_MAP_WIDTH * scy);

			stored_tile_load_command = 1U;
			stored_tile_load_bkg_x = 0U;
			stored_tile_load_bkg_y = scroll_y_temp;
			stored_tile_load_bkg_w = CUR_MAP_WIDTH;
			stored_tile_load_bkg_h = 2U;
			stored_scrl_dat_ptr = scrl_data_ptr;

			/* this gets set in main.c DoGraphics */
		}
	}
}