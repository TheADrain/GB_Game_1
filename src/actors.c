#include "actors.h"
#include "sprite_manager.h"

/* an actor is essentially a collection of the following
	- a rectangle (hitbox)
	- a position (bottom center of the hitbox)
	- a SHAPE struct pointer (for sprite rendering)
	- the Initialize and Update functions
		that determine behaviour for this object
*/
struct ACTOR actors_array[MAX_ACTORS];
UINT8 actors_in_use[MAX_ACTORS];
struct ACTOR * created_actor_ptr = 0U;

void init_actor_manager()
{
	UINT8 i = 0U;
	for(i = 0; i < MAX_ACTORS; i = i + 1)
	{
		actors_array[i].ActorType = 0U;
		actors_array[i].ActorIndex = i;
		actors_array[i].Width = 0U;
		actors_array[i].Height = 0U;
		actors_array[i].PositionX = 0U;
		actors_array[i].PositionY = 0U;

		actors_array[i].shape_ptr = 0U;
		actors_array[i].Initialize = 0U;
		actors_array[i].Update = 0U;
	}

	i = 0;
	for(i = 0; i < MAX_ACTORS; i = i + 1)
	{
		actors_in_use[i] = ACTOR_FREE;
	}
}

UINT8 create_actor(UINT8 actor_type)
{
	if(actor_type >= NUM_DEFINED_ACTORS)
	{
		return ACTOR_NOT_AVAILABLE;
	}

	/* init a new actor if we can, if not return a failure result */
	UINT8 i = 0U;
	UINT8 actor_index = 0U;
	for(i = 0; i < MAX_ACTORS; i = i + 1)
	{
		if(actors_in_use[i] == ACTOR_FREE)
		{
			actor_index = i;
			break;
		}
	}

	UINT8 shape_type = actor_defs[actor_type].SpriteShapeType;

	/* init a shape if we can */
	if(create_shape(shape_type))
	{
		actors_in_use[actor_index] = ACTOR_USED;

		actors_array[actor_index].ActorType = actor_type;

		actors_array[actor_index].Width = actor_defs[actor_type].Width;
		actors_array[actor_index].Height = actor_defs[actor_type].Height;

		actors_array[actor_index].PositionX = 0U;
		actors_array[actor_index].PositionY = 0U;

		actors_array[actor_index].SpriteShapeType = shape_type;
		actors_array[actor_index].shape_ptr = created_shape_ptr;

		actors_array[actor_index].Initialize = actor_defs[actor_type].Initialize;
		actors_array[actor_index].Update = actor_defs[actor_type].Update;

		created_actor_ptr = &(actors_array[actor_index]);

		return ACTOR_AVAILABLE;
	}

	return ACTOR_NOT_AVAILABLE;
}

void release_actor(struct ACTOR * actor_ptr)
{
	UINT8 actor_index = actor_ptr->ActorIndex;

	actors_array[actor_index].ActorType = 0U;
	actors_array[actor_index].Width = 0U;
	actors_array[actor_index].Height = 0U;
	actors_array[actor_index].PositionX = 0U;
	actors_array[actor_index].PositionY = 0U;

	actors_array[actor_index].Initialize = 0U;
	actors_array[actor_index].Update = 0U;

	/* check shape is valid incase we already released it */
	if(actors_array[actor_index].shape_ptr != 0U)
	{
		release_shape(actors_array[actor_index].shape_ptr);
		actors_array[actor_index].shape_ptr = 0U;
	}

	actors_in_use[actor_index] = ACTOR_FREE;
}

/* ACTOR DEFINITIONS */
const struct ACTOR_DEFINITION actor_defs[NUM_DEFINED_ACTORS] = {
	{
		ACTOR_EMPTY,
		1U, /* width */
		1U, /* height */

		SHAPE_1x1, /* sprite shape to use */

		/* function pointers */
		&Initialize_EmptyActor,
		&UpdateEmptyActor
	},
	{ 
		ACTOR_ENEMY_1,
		1U, /* width */
		1U, /* height */

		SHAPE_2x2, /* sprite shape to use */

		/* function pointers */
		&Initialize_EmptyActor,
		&UpdateEmptyActor
	}
};

void Initialize_EmptyActor(struct ACTOR* actor_ptr)
{
}

void UpdateEmptyActor(struct ACTOR* actor_ptr)
{
}

