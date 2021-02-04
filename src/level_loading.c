#include "level_loading.h"
#include "main.h"
#include "bankdefs.h"
#include "levels.h"
#include "utils_1.h"
#include "input.h"
#include "player.h"

UINT8 numerals_offset = 0;
UINT8 tileDataCtr = 0x00;

#define LEVELNUM_X 12
#define LEVELNUM_y 10

void levelcard_init()
{
	FadeToBlack(4U);

	disable_interrupts();
	/* Initialize the VRAM data */
	SWITCH_ROM_MBC1(BANK_TITLE);
	/* load tiles for the title screen */
	tileDataCtr = 0x00;
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

	SHOW_BKG;
	enable_interrupts();

	FadeFromBlack(4U);
}

void levelcard_update()
{
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

	level_tilemap_data = levels[CUR_LEVEL].MapTileData;
	level_collision_data = levels[CUR_LEVEL].CollisionMap;
	CUR_MAP_WIDTH = levels[CUR_LEVEL].Width;
	CUR_MAP_HEIGHT = levels[CUR_LEVEL].Height;

	enable_interrupts();
}

void load_current_level_graphics()
{
	disable_interrupts();

	/* ensure we're in sprite 8x16 mode */
	SPRITES_8x16;

	/* load the sprite tiles (for now this is same for all levels) */
	SWITCH_ROM_MBC1(BANK_GRAPHICS_DATA_1);
	set_sprite_data(0x00, spritesLength, sprites);

	/* load the background tiles for this map */
	set_bkg_data(0x00, levels[CUR_LEVEL].tileDataLength, levels[CUR_LEVEL].tileDataPtr);

	enable_interrupts();
}

void load_current_level_map()
{
	disable_interrupts();
	
	/* Initialize the title map data */
	SWITCH_ROM_MBC1(levels[CUR_LEVEL].RomBank);

	/* currently assuming column-first map */
	UINT8 i = 0;
	unsigned char* dataPtr = levels[CUR_LEVEL].MapTileData;
		
	
	/* draw left to right for a horizontal level */
	if(levels[CUR_LEVEL].MapType == MAP_HORIZONTAL)
	{
		for(i = 0; i < 32; i++)
		{
			set_bkg_tiles(i, 0, 1, CUR_MAP_HEIGHT, dataPtr);
			dataPtr += CUR_MAP_HEIGHT;
		}
	}
	/* and top to bottom for a vertical one */
	else
	{
		for(i = 0; i < 32; i++)
		{
			set_bkg_tiles(0, i, CUR_MAP_WIDTH, 1, dataPtr);
			dataPtr += CUR_MAP_WIDTH;
		}
	}
	
	enable_interrupts();
}

void start_level()
{
	init_game_camera();
	init_player_sprite();

	SHOW_BKG;
	SHOW_SPRITES;

	gbt_enable_channels(0x00);
	gbt_play(song_Data, 2, 7);
	gbt_loop(1);

	/* spawn and position the player */

	FadeFromWhite(4U);
}