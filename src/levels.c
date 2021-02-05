#include "levels.h"

const struct LEVEL_DATA levels[3] =
{
	{ 
		MAP_HORIZONTAL,
		map01Width, 
		map01Height, 
		BANK_MAPDATA_01, 
		SPAWN_LEFT,
		20, /* spawn x */
		220, /* spawn y */
		tower_tiledataLength,
		(unsigned char*)&tower_tiledata,
		(unsigned char*)&map01_tilemap, 
		(unsigned char*)&map01_collision
	},

	{ 
		MAP_VERTICAL,
		map02Width, 
		map02Height, 
		BANK_MAPDATA_01, 
		SPAWN_BOTTOM,
		20, /* spawn x */
		716, /* spawn y */
		tower_tiledataLength,
		(unsigned char*)&tower_tiledata,
		(unsigned char*)&map02_tilemap, 
		(unsigned char*)&map02_collision 
	},

	{ 
		MAP_HORIZONTAL,
		map_forest01Width, 
		map_forest01Height, 
		BANK_MAPDATA_01, 
		SPAWN_LEFT,
		20, /* spawn x */
		96, /* spawn y */
		forest_tiledataLength,
		(unsigned char*)&forest_tiledata,
		(unsigned char*)&map_forest01_tilemap, 
		(unsigned char*)&map_forest01_collision 
	},
};