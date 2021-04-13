#include "levels.h"

/* Level Collision Data holds both Collision Tiles and Object Spawn data */
/* 0x00 to 0x7F is a Collision Tile */
/* 0x80 to 0x FF is an Object Type */

const struct LEVEL_DATA levels[3] =
{
	{ 
		MAP_HORIZONTAL,
		map_tower01Width, 
		map_tower01Height, 
		BANK_MAPDATA_01, 
		SPAWN_LEFT,
		20, /* spawn x */
		220, /* spawn y */
		tset_towerLength,
		(unsigned char*)&tset_tower,
		(unsigned char*)&map_tower01_tilemap, 
		(unsigned char*)&map_tower01_collision
	},

	{ 
		MAP_VERTICAL,
		map_tower02Width, 
		map_tower02Height, 
		BANK_MAPDATA_01, 
		SPAWN_BOTTOM,
		20, /* spawn x */
		50, /* spawn y */
		tset_towerLength,
		(unsigned char*)&tset_tower,
		(unsigned char*)&map_tower02_tilemap, 
		(unsigned char*)&map_tower02_collision 
	},

	{ 
		MAP_HORIZONTAL,
		map_forest01Width, 
		map_forest01Height, 
		BANK_MAPDATA_01, 
		SPAWN_LEFT,
		20, /* spawn x */
		96, /* spawn y */
		tset_forestLength,
		(unsigned char*)&tset_forest,
		(unsigned char*)&map_forest01_tilemap, 
		(unsigned char*)&map_forest01_collision 
	},
};