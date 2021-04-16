#include "actors.h"
#include "sprite_manager.h"
#include "actors/skelton.h"

/* an actor is essentially a collection of the following
	- a rectangle (hitbox)
	- a position (bottom center of the hitbox)
	- a SHAPE struct pointer (for sprite rendering)
	- the Initialize and Update functions
		that determine behaviour for this object
*/
struct ACTOR actors_array[MAX_ACTORS];
UINT8 actors_in_use[MAX_ACTORS];
UINT8 num_actors_in_use = 0U;
struct ACTOR * created_actor_ptr = 0U;



void init_actor_manager()
{
	UINT8 i = 0U;
	UINT8 j = 0U;
	for(i = 0; i < MAX_ACTORS; i = i + 1)
	{
		actors_array[i].ActorType = 0U;
		actors_array[i].ActorIndex = i;
		actors_array[i].Width = 0U;
		actors_array[i].Height = 0U;
		actors_array[i].PositionX = 0U;
		actors_array[i].PositionY = 0U;

		actors_array[i].FlipX = 0U;
		actors_array[i].FlipY = 0U;

		actors_array[i].SpritesAllocated = 0U;
		
		actors_array[i].Update = EMPTY_UPDATE;
		actors_array[i].Initialize = EMPTY_INIT;
		
		j = 0U;
		for(j = 0; j < MAX_SPRITES_PER_ACTOR; j = j + 1)
		{
			actors_array[i].SpriteIndexes[j] = 0U;
		}

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

	UINT8 sprite_allocation = actor_defs[actor_type].SpriteAllocation;

	/* init a shape if we can */
	if(grab_sprites(sprite_allocation))
	// todo: reserve the sprites insted
	{
		actors_in_use[actor_index] = ACTOR_USED;

		actors_array[actor_index].ActorType = actor_type;

		actors_array[actor_index].SpritesAllocated = sprite_allocation;

		actors_array[actor_index].Width = actor_defs[actor_type].Width;
		actors_array[actor_index].Height = actor_defs[actor_type].Height;

		actors_array[actor_index].PositionX = 0U;
		actors_array[actor_index].PositionY = 0U;

		actors_array[actor_index].CurAnimFramePtr = 0U;
		actors_array[actor_index].CurAnimFrameIndex = 0U;
		actors_array[actor_index].AnimTimer = 0U;

		actors_array[actor_index].FlipX = 0U;
		actors_array[actor_index].FlipY = 0U;

		actors_array[actor_index].State = 0U;

		actors_array[actor_index].Initialize = actor_defs[actor_type].Initialize;
		actors_array[actor_index].Update = actor_defs[actor_type].Update;

		/* allocate the sprites we grabbed */
		i = 0U;
		for(i = 0; i < sprite_allocation; i = i + 1)
		{
			actors_array[actor_index].SpriteIndexes[i] = grabbed_sprites_buffer[i];
		}

		created_actor_ptr = &(actors_array[actor_index]);
		num_actors_in_use = num_actors_in_use + 1;

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

	actors_array[actor_index].FlipX = 0U;
	actors_array[actor_index].FlipY = 0U;

	actors_array[actor_index].Initialize = 0U;
	actors_array[actor_index].Update = 0U;

	actors_array[actor_index].State = 0U;

	UINT8 i = 0U;
	UINT8 sprite_allocation = actors_array[actor_index].SpritesAllocated;
	for(i = 0; i < sprite_allocation; i = i + 1)
	{
		release_sprite(actors_array[actor_index].SpriteIndexes[i]);
	}

	actors_in_use[actor_index] = ACTOR_FREE;
	num_actors_in_use = num_actors_in_use - 1;
}

/* ACTOR DEFINITIONS */
const struct ACTOR_DEFINITION actor_defs[NUM_DEFINED_ACTORS] = {
	{
		ACTOR_EMPTY,
		1U, /* width */
		1U, /* height */

		1U, /* HOW MANY SPRITES TO RESERVE */

		/* function pointers */
		&Initialize_EmptyActor,
		&UpdateEmptyActor
	},
	{
		ACTOR_PLAYER,
		16U, /* width */
		16U, /* height */

		8U, /* HOW MANY SPRITES TO RESERVE */

		/* function pointers */
		EMPTY_INIT,
		EMPTY_UPDATE
	},
	{ 
		ACTOR_ENEMY_1,

		1U, /* width */
		1U, /* height */

		4U, /* HOW MANY SPRITES TO RESERVE */

		/* function pointers */
		&Initialize_EmptyActor,
		&UpdateEmptyActor
	},
	{
		ACTOR_SKELTON,

		2U, /* width */
		2U, /* height */

		5U, /* HOW MANY SPRITES TO RESERVE */

		/* function pointers */
		&Initialize_ActorSkelton,
		&Update_ActorSkelton
	}
};

void Initialize_EmptyActor(struct ACTOR* actor_ptr)
{
}

void UpdateEmptyActor(struct ACTOR* actor_ptr)
{
}

/* main actor update function */

UINT8 cur_update_actor_idx = 0U;
UINT8 actr_i = 1U;
UINT8 actr_upd_lim = 0U;
void UpdateActors()
{
	/* always update actor 0 every frame, it should always be the player */
	actors_array[0U].Update(&actors_array[0U]);

	actr_upd_lim = actr_i + ACTORS_PER_UPDATE;
	if(actr_upd_lim > MAX_ACTORS)
	{
		actr_upd_lim = MAX_ACTORS;
	}

	for(actr_i; actr_i < actr_upd_lim; actr_i = actr_i + 1)
	{
		if(actors_in_use[actr_i] == ACTOR_USED && actors_array[actr_i].Update != EMPTY_UPDATE)
		{
			actors_array[actr_i].Update(&actors_array[actr_i]);
		}
	}

	if(actr_i >= MAX_ACTORS)
	{
		actr_i = 1U;
	}
}

void SetActorAnim(struct ACTOR * a, struct ANIMATION * anim)
{
	a->CurAnimFramePtr = (struct ANIM_FRAME *)(anim->Frames[0U]);
	frame_spritecount = a->CurAnimFramePtr->NumTiles;

	_i = 0U;
	sprites_allocated_count = a->SpritesAllocated;
	for(_i = 0U; _i < sprites_allocated_count; _i = _i+1)
	{
		if(_i < frame_spritecount)
		{
			set_sprite_tile(a->SpriteIndexes[_i], VRAM_PLAYER + a->CurAnimFramePtr->AnimTiles[_i].Tile);
		}
		else
		{
			move_sprite(a->SpriteIndexes[_i], 0, 0);
		}
	}

	for(_i = 0U; _i < frame_spritecount; _i = _i+1)
	{
		move_sprite(a->SpriteIndexes[_i], a->PositionX + a->CurAnimFramePtr->AnimTiles[_i].XOffset, a->PositionY + a->CurAnimFramePtr->AnimTiles[_i].YOffset);
	}
}

UINT8 tmp_spr_props = 0U;
UINT8 tmp_offset_x = 0U;
UINT8 tmp_offset_y = 0U;

void UpdateActorAnim(struct ACTOR * a, struct ANIMATION * anim)
{
	tmp_spr_props = 0U;

	frame_spritecount = a->CurAnimFramePtr->NumTiles;
	sprites_allocated_count = a->SpritesAllocated;

	anim_tmr = a->AnimTimer - ACTOR_ANIM_TIMESTEP;

	if(anim_tmr <= 0U)
	{
		a->CurAnimFrameIndex = a->CurAnimFrameIndex + 1U;
		if(a->CurAnimFrameIndex >= anim->NumFrames)
		{
			a->CurAnimFrameIndex = 0U;
		}

		a->CurAnimFramePtr = (struct ANIM_FRAME *)(anim->Frames[a->CurAnimFrameIndex]);

		anim_tmr = a->CurAnimFramePtr->FrameDuration;

		for(_i = 0U; _i < sprites_allocated_count; _i = _i+1)
		{
			if(_i >= frame_spritecount)
			{
				move_sprite(a->SpriteIndexes[_i], 0, 0);
			}
			else
			{
				set_sprite_tile(a->SpriteIndexes[_i], VRAM_PLAYER + a->CurAnimFramePtr->AnimTiles[_i].Tile);
			}
		}
	}

	a->AnimTimer = anim_tmr;
	
	for(_i = 0U; _i < frame_spritecount; _i = _i+1)
	{
		tmp_spr_props = get_sprite_prop(a->SpriteIndexes[_i]);
		tmp_offset_x = 0U;
		tmp_offset_y = 0U;

		if (a->FlipX == 0U)
		{
			/* turn flip x off */
			tmp_spr_props = tmp_spr_props & ~S_FLIPX;

			/* adjust x position */
			tmp_offset_x = a->PositionX + a->CurAnimFramePtr->AnimTiles[_i].XOffset;
		}
		else
		{
			/* turn flip x on */
			tmp_spr_props = tmp_spr_props | S_FLIPX;

			/* adjust x position */
			tmp_offset_x = a->PositionX - a->CurAnimFramePtr->AnimTiles[_i].XOffset;
		}

		/* only support flipx for now on actors I think... */

		set_sprite_prop(a->SpriteIndexes[_i], tmp_spr_props);
		move_sprite(a->SpriteIndexes[_i], tmp_offset_x, a->PositionY + a->CurAnimFramePtr->AnimTiles[_i].YOffset);
	}
}
