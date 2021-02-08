#ifndef COLLISION_H
#define COLLISION_H

#include <gb/gb.h>

/* direction helpers */
#define DIR_UP 0U
#define DIR_DOWN 1U
#define DIR_LEFT 2U
#define DIR_RIGHT 3U

/* Collision Table */
/* basic tile types */
#define COL_EMPTY			0U
#define COL_SOLID			1U

#define COL_DAMAGE			2U
#define COL_DEATH			3U

/* one-way tiles allow movement in the direction stated */
#define COL_ONEWAY_UP		4U
#define COL_ONEWAY_DOWN		5U
#define COL_ONEWAY_LEFT		6U
#define COL_ONEWAY_RIGHT	7U

/* what I really should do is have a tile props structure and feed in the travel direction 
	rather than having 4 separate movement related collision routines

	eg:
		tile data for COL_ONEWAY_UP would be entry 4U in a table and the data would be
			tile_props = %00000111 where the first (rightmost) 4 bits correspond to directions 
				they cause collisions with 

		then collision would be: moving in (0000 UDLR) compare with tile prop for that tile 
		compare moving up (1000) the blocking directions of the tile (0111) by 
		would this be more efficient?

		this might be something for the next game, this game is getting complex enough
*/

/* other */
#define COLLISION_RESULT_NONE 0U
#define COLLISION_RESULT_SOLID 1U
/* for one-way tiles, so we don't pop through them when moving against them */
#define COLLISION_RESULT_SOLID_NON_GRID_ALIGNED 2U 

UINT8 TestCollisionTile_WithDirection(UINT8 tile, UINT8 direction);
UINT8 TestCollisionType_MovingUp(UINT8 t);
UINT8 TestCollisionType_MovingDown(UINT8 t);
UINT8 TestCollisionType_MovingLeft(UINT8 t);
UINT8 TestCollisionType_MovingRight(UINT8 t);

#endif