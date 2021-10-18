#include "levels.h"

/* Level Collision Data holds both Collision Tiles and Object Spawn data */
/* 0x00 to 0x7F is a Collision Tile */
/* 0x80 to 0x FF is an Object Type */

const struct LEVEL_DEF levels[TOTAL_LEVELS] =
{
	/* Tower 01 */
	{
		1U, /* num chunks */
		0U  /* first chunk */
	},
	/* Tower 02 */
	{
		1U, /* num chunks */
		1U  /* first chunk */
	},
	/* Forest 01 */
	{
		1U, /* num chunks */
		2U  /* first chunk */
	},


	/* Tower 03 - 3 parts */
	{
		5U, /* num chunks */
		3U  /* first chunk */
	},


	/* Debug Level - Last in the array */
	{
		1U, /* num chunks */
		8U  /* first chunk */
	}
};

const struct LEVEL_DATA level_datas[TOTAL_LEVEL_DATAS] =
{
	{ 
		MAP_HORIZONTAL,
		map_tower01Width, 
		map_tower01Height, 
		BANK_MAPDATA_01, 
		SPAWN_LEFT,
		EXIT_EXIT_TILES_ONLY,
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
		EXIT_EXIT_TILES_ONLY,
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
		EXIT_EXIT_TILES_ONLY,
		20, /* spawn x */
		96, /* spawn y */
		tset_forestLength,
		(unsigned char*)&tset_forest,
		(unsigned char*)&map_forest01_tilemap, 
		(unsigned char*)&map_forest01_collision 
	},

	/* first multiple-chunk map - tower03_ 01 to 03 */
	{
		MAP_HORIZONTAL,
		map_tower03_01Width,
		map_tower03_01Height,
		BANK_MAPDATA_03,
		SPAWN_LEFT,
		EXIT_RIGHT_BOUNDS,
		30, /* spawn x */
		40, /* spawn y */
		tset_towerLength,
		(unsigned char*)&tset_tower,
		(unsigned char*)&map_tower03_01_tilemap,
		(unsigned char*)&map_tower03_01_collision
	},
	{
		MAP_VERTICAL,
		map_tower03_02Width,
		map_tower03_02Height,
		BANK_MAPDATA_03,
		SPAWN_BOTTOM,
		EXIT_SMOOTH_TO_NEXT_CHUNK,
		40, /* spawn x */
		264, /* spawn y */
		tset_towerLength,
		(unsigned char*)&tset_tower,
		(unsigned char*)&map_tower03_02_tilemap,
		(unsigned char*)&map_tower03_02_collision
	},
	{
		MAP_HORIZONTAL,
		map_tower03_03Width,
		map_tower03_03Height,
		BANK_MAPDATA_03,
		SPAWN_LEFT,
		EXIT_SMOOTH_TO_NEXT_CHUNK,
		0, /* spawn x */
		0, /* spawn y */
		tset_towerLength,
		(unsigned char*)&tset_tower,
		(unsigned char*)&map_tower03_03_tilemap,
		(unsigned char*)&map_tower03_03_collision
	},
	{
		MAP_VERTICAL,
		map_tower03_04Width,
		map_tower03_04Height,
		BANK_MAPDATA_03,
		SPAWN_TOP,
		EXIT_EXIT_TILES_ONLY,
		0, /* spawn x */
		0, /* spawn y */
		tset_towerLength,
		(unsigned char*)&tset_tower,
		(unsigned char*)&map_tower03_04_tilemap,
		(unsigned char*)&map_tower03_04_collision
	},
	{
		MAP_HORIZONTAL,
		map_tower03_05Width,
		map_tower03_05Height,
		BANK_MAPDATA_03,
		SPAWN_LEFT,
		EXIT_RIGHT_BOUNDS,
		0, /* spawn x */
		0, /* spawn y */
		tset_towerLength,
		(unsigned char*)&tset_tower,
		(unsigned char*)&map_tower03_05_tilemap,
		(unsigned char*)&map_tower03_05_collision
	},


	/* last map is a debug map */
	{
		MAP_HORIZONTAL,
		map_dbglvl01Width,
		map_dbglvl01Height,
		BANK_MAPDATA_02,
		SPAWN_LEFT,
		EXIT_EXIT_TILES_ONLY,
		20, /* spawn x */
		220, /* spawn y */
		tset_towerLength,
		(unsigned char*)&tset_tower,
		(unsigned char*)&map_dbglvl01_tilemap,
		(unsigned char*)&map_dbglvl01_collision
	}
};
