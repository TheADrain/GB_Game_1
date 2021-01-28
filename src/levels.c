#include "levels.h"

const struct LEVEL_DATA levels[2] =
{
	{ map01Width, map01Height, BANK_MAPDATA_01, (unsigned char*)&map01_tilemap, (unsigned char*)&map01_collision },
	{ map02Width, map01Height, BANK_MAPDATA_01, (unsigned char*)&map02_tilemap, (unsigned char*)&map02_collision }
};