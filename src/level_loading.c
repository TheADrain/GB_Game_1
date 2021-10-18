#include "level_loading.h"
#include "main.h"
#include "bankdefs.h"
#include "levels.h"
#include "utils_1.h"
#include "input.h"
#include "player.h"
#include "string.h"
#include "scrolling.h"

UINT8 numerals_offset = 0;
UINT8 tileDataCtr = 0x00;

#define LEVELNUM_X 12
#define LEVELNUM_y 10


void levelcard_init()
{
	FadeToBlack(4U);

	HIDE_SPRITES;
	HIDE_BKG;

	move_bkg(0U, 0U);

	DISPLAY_OFF;
	memset((UBYTE*)BG_MAP_1, 0U, 32*32);

	/* Initialize the VRAM data */
	SWITCH_ROM_MBC1(BANK_TITLE);
	/* load tiles for the title screen */
	tileDataCtr = 0x00;

	wait_vbl_done();
	disable_interrupts();

	set_bkg_data(tileDataCtr, levelcard_tilesLength, levelcard_tiles);
	/* Initialize the title map data */
	set_bkg_tiles(0, 0, levelcard_mapWidth, levelcard_mapHeight, levelcard_map);

	numerals_offset = levelcard_tilesLength;
	tileDataCtr += levelcard_tilesLength;

	set_bkg_data(tileDataCtr, numeralsLength, numerals);

	if(CUR_LEVEL < 10)
	{
		/* for single digit levels */
		fill_bkg_rect(LEVELNUM_X, LEVELNUM_y, 1, 1, (numerals_offset + CUR_LEVEL + 1));
	}
	else if(CUR_LEVEL > 9)
	{
		UINT8 ctr = CUR_LEVEL;
		UINT8 tens = 0;
		while(ctr > 9)
		{
			ctr -= 10;
			tens++;
		}

		fill_bkg_rect(LEVELNUM_X, LEVELNUM_y, 1, 1, (numerals_offset + tens));
		fill_bkg_rect(LEVELNUM_X+1, LEVELNUM_y, 1, 1, (numerals_offset + ctr + 1));
	}
	
	DISPLAY_ON;
	SHOW_BKG;
	enable_interrupts();

	FadeFromBlack(4U);
}

void levelcard_update()
{
	move_bkg(0U, 0U);
	if(JOY_PRESSED(BTN_START))
	{
		/* move to the next state */
		FadeToWhite(4U);
				
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
	CUR_SUBLEVEL = 0U;
	CUR_LEVEL_DATA_IDX = levels[CUR_LEVEL].FirstChunk;


	level_tilemap_data = level_datas[CUR_LEVEL_DATA_IDX].MapTileData;
	level_collision_data = level_datas[CUR_LEVEL_DATA_IDX].CollisionMap;
	CUR_MAP_WIDTH = level_datas[CUR_LEVEL_DATA_IDX].Width;
	CUR_MAP_HEIGHT = level_datas[CUR_LEVEL_DATA_IDX].Height;

	enable_interrupts();
}

void increment_sub_level_data()
{
	CUR_SUBLEVEL = CUR_SUBLEVEL + 1;
	CUR_LEVEL_DATA_IDX = levels[CUR_LEVEL].FirstChunk + CUR_SUBLEVEL;

	level_tilemap_data = level_datas[CUR_LEVEL_DATA_IDX].MapTileData;
	level_collision_data = level_datas[CUR_LEVEL_DATA_IDX].CollisionMap;
	CUR_MAP_WIDTH = level_datas[CUR_LEVEL_DATA_IDX].Width;
	CUR_MAP_HEIGHT = level_datas[CUR_LEVEL_DATA_IDX].Height;
}

void load_current_level_graphics()
{
	wait_vbl_done();
	disable_interrupts();

	/* load the gameplay sprite tiles (for now this is same for all levels) */
	SWITCH_ROM_MBC1(BANK_GRAPHICS_DATA_1);
	set_sprite_data(VRAM_START, spr_gameplayLength, spr_gameplay);

	/* load the actor tiles for this level type */
	/* TODO: put this in the level data */
	set_sprite_data(VRAM_ACTORS, spr_actors_forestLength, spr_actors_forest);

	/* load the background tiles for this map */
	set_bkg_data(0x00, level_datas[CUR_LEVEL_DATA_IDX].tileDataLength, level_datas[CUR_LEVEL_DATA_IDX].tileDataPtr);

	enable_interrupts();
}

void load_current_level_map()
{
	wait_vbl_done();
	disable_interrupts();
	
	/* Initialize the title map data */
	SWITCH_ROM_MBC1(level_datas[CUR_LEVEL_DATA_IDX].RomBank);

	/* currently assuming column-first map */
	UINT8 i = 0;
	unsigned char* dataPtr = level_datas[CUR_LEVEL_DATA_IDX].MapTileData;
	
	/* draw left to right for a horizontal level */
	if(level_datas[CUR_LEVEL_DATA_IDX].MapType == MAP_HORIZONTAL)
	{
		UINT8 width = 32;
		if (CUR_MAP_WIDTH < 32)
		{
			width = CUR_MAP_WIDTH;
		}

		for(i = 0; i < width; i++)
		{
			set_bkg_tiles(i, 0, 1, CUR_MAP_HEIGHT, dataPtr);
			dataPtr += CUR_MAP_HEIGHT;
		}
	}
	/* and top to bottom for a vertical one */
	else
	{
		UINT8 height = 32;
		if (CUR_MAP_HEIGHT < 32)
		{
			height = CUR_MAP_HEIGHT;
		}

		for(i = 0; i < height; i++)
		{
			set_bkg_tiles(0, i, CUR_MAP_WIDTH, 1, dataPtr);
			dataPtr += CUR_MAP_WIDTH;
		}
	}
	
	enable_interrupts();
}

INT8 chunk_h_offset = 0;
INT8 chunk_v_offset = 0;

void load_current_horizontal_level_map_chunk()
{
	wait_vbl_done();
	disable_interrupts();

	/* Initialize the title map data */
	SWITCH_ROM_MBC1(level_datas[CUR_LEVEL_DATA_IDX].RomBank);

	/* currently assuming column-first map */
	UINT8 i = 0;

	UINT8 width = 32;
	if (CUR_MAP_WIDTH < 32)
	{
		width = CUR_MAP_WIDTH;
	}
	
	unsigned char* dataPtr = level_datas[CUR_LEVEL_DATA_IDX].MapTileData;

	for (i = 0; i < width; i++)
	{
		set_bkg_tiles(i + chunk_h_offset, 0 + chunk_v_offset, 1, CUR_MAP_HEIGHT, dataPtr);
		dataPtr += CUR_MAP_HEIGHT;
	}

	enable_interrupts();
}

void load_current_vertical_level_map_chunk()
{
	wait_vbl_done();
	disable_interrupts();

	/* Initialize the title map data */
	SWITCH_ROM_MBC1(level_datas[CUR_LEVEL_DATA_IDX].RomBank);

	/* currently assuming column-first map */
	UINT8 i = 0;
	
	UINT8 height = 32;
	if (CUR_MAP_HEIGHT < 32)
	{
		height = CUR_MAP_HEIGHT;
	}

	unsigned char* dataPtr = level_datas[CUR_LEVEL_DATA_IDX].MapTileData;

	if (level_datas[CUR_LEVEL_DATA_IDX].SpawnType == SPAWN_BOTTOM)
	{
		dataPtr += (level_datas[CUR_LEVEL_DATA_IDX].Width * level_datas[CUR_LEVEL_DATA_IDX].Height);
		dataPtr -= CUR_MAP_WIDTH * height;
	}
	
	for (i = 0; i < height; i++)
	{
		set_bkg_tiles(0 + chunk_h_offset, i + chunk_v_offset, CUR_MAP_WIDTH, 1, dataPtr);
		dataPtr += CUR_MAP_WIDTH;
	}

	enable_interrupts();
}

void start_level()
{
	disable_interrupts();
	/* spawn and position the player */
	init_game_camera();
	init_player_sprite();

	SHOW_BKG;
	SHOW_SPRITES;

	gbt_enable_channels(0x00);
	gbt_play(song_Data, 2, 7);
	gbt_loop(1);

	add_VBL(vblint);
	enable_interrupts();

	FadeFromWhite(4U);
}

void end_level()
{
	disable_interrupts();

	remove_VBL(vblint);

	cleanup_player_sprite();

	enable_interrupts();
}