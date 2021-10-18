#ifndef LEVELS_H
#define LEVELS_H
#include <gb/gb.h>
#include "bankdefs.h"

/* ------- Notes on Level Format ---------

	No compression is used (goal was to not use any SRAM)

	Level (or at least chunk) maximum size is 256x32 as that will fit
	in a single bank. Could potentially get 512x32 by 
	splitting tile/collision into two banks.

	Tile data is simply 0-256 (should never really go over 128, max 160)
	however it is layed out either in columns or rows to enable copying to
	VRAM in a single operation for horizontally or vertically scrolling levels.
		
		Horizontal levels are arranged in columns.
		Vertical levels are arranged in rows.
	
	Collision and Object Data are combined in one array.
	0-127 = Collision tile of the type indicated by that number,
				no modification needed to get the collision type

	128-256 = The object index from 128-256
				flip the top bit to subtract 128 and you have
				your object index of 0-127 use this index to lookup
				the object for that index in the associated object table
				for the level to find the actual object type and properties.

	The 'levels' array is populated by hand (for now) and contains 
		meta-data for each level, including its scroll type, 
			dimensions and object data table pointer.

*/

#define MAP_HORIZONTAL 0U
#define MAP_VERTICAL 1U

#define SPAWN_LEFT 0U
#define SPAWN_BOTTOM 1U
#define SPAWN_TOP 2U

#define EXIT_EXIT_TILES_ONLY 0U
#define EXIT_RIGHT_BOUNDS 1U
#define EXIT_SMOOTH_TO_NEXT_CHUNK 2U

typedef struct LEVEL_DEF
{
	/* string level name ... etc */

	UINT8 NumChunks;
	UINT8 FirstChunk; /* The LevelData which represents the first chunk of level data */
};

#define TOTAL_LEVELS 5
extern const struct LEVEL_DEF levels[TOTAL_LEVELS];

/* Level Data structure */
typedef struct LEVEL_DATA {

	/* map info */
	UINT8 MapType;
	UINT16 Width;
	UINT16 Height;
	UINT8 RomBank;

	/* other properties */
	/* todo: either implement or get rid of this! 
	 it's supposed to automatically find the player spawn location for the level but it doesnt */
	UINT8 SpawnType;

	UINT8 ExitType;

	/* player spawn x and y */
	UINT16 PlayerSpawnX;
	UINT16 PlayerSpawnY;

	/* where to get tile data from */
	UINT8 tileDataLength;
	unsigned char* tileDataPtr;
	
	/* pointer to the tile graphics data for this map here? */
	unsigned char* MapTileData;
	unsigned char* CollisionMap;
	/*unsigned char* ThingsMap;*/
};

/* ---------------------   LEVELS    -------------------------- */

/* no way to do this automatically huh? ok then :< */
#define TOTAL_LEVEL_DATAS 9
extern const struct LEVEL_DATA level_datas[TOTAL_LEVEL_DATAS];

#endif