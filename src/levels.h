#ifndef LEVELS_H
#define LEVELS_H
#include <gb/gb.h>
#include "bankdefs.h"

#define MAP_HORIZONTAL 0U
#define MAP_VERTICAL 1U

#define SPAWN_LEFT 0U
#define SPAWN_BOTTOM 1U

/* Level Data structure */
typedef struct LEVEL_DATA {

	/* map info */
	UINT8 MapType;
	UINT16 Width;
	UINT16 Height;
	UINT8 RomBank;

	/* other properties */
	UINT8 SpawnType;

	/* where to get tile data from */
	UINT8 tileDataLength;
	unsigned char* tileDataPtr;

	/* pointer to the tile graphics data for this map here? */
	unsigned char* MapTileData;
	unsigned char* CollisionMap;
	unsigned char* ThingsMap;
};

/* ---------------------   LEVELS    -------------------------- */

/* no way to do this automatically huh? ok then :< */
extern const struct LEVEL_DATA levels[3];

#endif