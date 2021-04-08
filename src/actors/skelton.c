#include "skelton.h"
#include <gb/gb.h>
#include "../main.h"
#include "../actors.h"
#include "../sprite_manager.h"
#include "../anims/anims_forest.h"



void Initialize_ActorSkelton(struct ACTOR* a)
{
	/* Set the tiles for the first frame */
	
	frame_spritecount = a->CurAnimFramePtr->NumTiles;

	a->CurAnimFramePtr = (struct ANIM_FRAME *)frames_skelly_stand[0U];

	_i = 0U;
	sprites_allocated_count = a->SpritesAllocated;
	for(_i = 0U; _i < sprites_allocated_count; _i = _i+1)
	{
		if(_i < frame_spritecount)
		{
			set_sprite_tile(a->SpriteIndexes[_i], VRAM_ACTORS + a->CurAnimFramePtr->AnimTiles[_i].Tile);
		}
		else
		{
			/* hide unused sprites */
			move_sprite(a->SpriteIndexes[_i], 0, 0);
			set_sprite_prop(a->SpriteIndexes[_i], a->CurAnimFramePtr->AnimTiles[_i].Props);
			
		}
	}
}


void Update_ActorSkelton(struct ACTOR* a)
{
	frame_spritecount = a->CurAnimFramePtr->NumTiles;
	sprites_allocated_count = a->SpritesAllocated;

	anim_tmr = a->AnimTimer - ACTOR_ANIM_TIMESTEP;

	if(anim_tmr <= 0U)
	{
		a->CurAnimFrameIndex = a->CurAnimFrameIndex + 1U;
		if(a->CurAnimFrameIndex > 1U)
		{
			a->CurAnimFrameIndex = 0U;
		}

		a->CurAnimFramePtr = (struct ANIM_FRAME *)frames_skelly_stand[a->CurAnimFrameIndex];

		anim_tmr = a->CurAnimFramePtr->FrameDuration;

		for(_i = 0U; _i < sprites_allocated_count; _i = _i+1)
		{
			if(_i >= frame_spritecount)
			{
				move_sprite(a->SpriteIndexes[_i], 0, 0);
			}
			else
			{
				set_sprite_tile(a->SpriteIndexes[_i], VRAM_ACTORS + a->CurAnimFramePtr->AnimTiles[_i].Tile);
			}
		}
	}

	a->AnimTimer = anim_tmr;

	for(_i = 0U; _i < sprites_allocated_count; _i = _i+1)
	{
		move_sprite(a->SpriteIndexes[_i], a->PositionX + a->CurAnimFramePtr->AnimTiles[_i].XOffset, a->PositionY + a->CurAnimFramePtr->AnimTiles[_i].YOffset);
		set_sprite_prop(a->SpriteIndexes[_i], a->CurAnimFramePtr->AnimTiles[_i].Props | get_sprite_prop(a->SpriteIndexes[_i]));
	}
}