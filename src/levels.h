#ifndef LEVELS_H
#define LEVELS_H
#include <gb/gb.h>
#include "bankdefs.h"

/* Level Data structure */
typedef struct LEVEL_DATA {
	UINT16 Width;
	UINT16 Height;
	UINT8 RomBank;
	/* pointer to the tile graphics data for this map here? */
	unsigned char* MapTileData;
	unsigned char* CollisionMap;
	unsigned char* ThingsMap;
};

/* ---------------------   LEVELS    -------------------------- */

/* no way to do this automatically huh? ok then :< */
extern const struct LEVEL_DATA levels[3];

#endif