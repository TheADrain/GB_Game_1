#include <gb/gb.h>
#include "collision.h"

/* tests the tile we found when checking for collision with the direction our
	object is moving to see if that tile represents a collision in that direction */
UINT8 TestCollisionTile_WithDirection(UINT8 tile, UINT8 direction)
{
	if(direction == DIR_UP)
	{
		return TestCollisionType_MovingUp(tile);
	}
	else if(direction == DIR_DOWN)
	{
		return TestCollisionType_MovingDown(tile);
	}
	else if(direction == DIR_LEFT)
	{
		return TestCollisionType_MovingLeft(tile);
	}
	else if(direction == DIR_RIGHT)
	{
		return TestCollisionType_MovingRight(tile);
	}

	return COLLISION_RESULT_NONE;
}

UINT8 TestCollisionType_MovingUp(UINT8 t)
{
	if(t == COL_EMPTY)
	{
		return COLLISION_RESULT_NONE;
	}

	if(t == COL_SOLID)
	{
		return COLLISION_RESULT_SOLID;
	}

	if(t == COL_ONEWAY_DOWN)
	{
		/* 
		todo: this wants to be able
			to detect how far up into the tile we are
			so we can only pop up if we're halfway into it
		*/
		return COLLISION_RESULT_SOLID;
	}

	return COLLISION_RESULT_NONE;
}

UINT8 TestCollisionType_MovingDown(UINT8 t)
{
	if(t == COL_EMPTY)
	{
		return COLLISION_RESULT_NONE;
	}

	if(t == COL_SOLID)
	{
		return COLLISION_RESULT_SOLID;
	}

	if(t == COL_ONEWAY_UP)
	{
		/* 
		todo: this wants to be able
			to detect how far up into the tile we are
			so we can only pop up if we're halfway into it
		*/
		return COLLISION_RESULT_SOLID;
	}

	return COLLISION_RESULT_NONE;
}

UINT8 TestCollisionType_MovingLeft(UINT8 t)
{
	if(t == COL_EMPTY)
	{
		return COLLISION_RESULT_NONE;
	}

	if(t == COL_SOLID)
	{
		return COLLISION_RESULT_SOLID;
	}

	if(t == COL_ONEWAY_RIGHT)
	{
		return COLLISION_RESULT_SOLID_NON_GRID_ALIGNED;
	}

	return COLLISION_RESULT_NONE;
}

UINT8 TestCollisionType_MovingRight(UINT8 t)
{
	if(t == COL_EMPTY)
	{
		return COLLISION_RESULT_NONE;
	}

	if(t == COL_SOLID)
	{
		return COLLISION_RESULT_SOLID;
	}

	if(t == COL_ONEWAY_LEFT)
	{
		return COLLISION_RESULT_SOLID_NON_GRID_ALIGNED;
	}

	return COLLISION_RESULT_NONE;
}