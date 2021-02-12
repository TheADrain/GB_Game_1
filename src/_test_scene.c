#include "anims/anims_forest.h"

UINT8 _test_scene_initialized = 0U;


struct ACTOR * actor1 = 0U;
struct ACTOR * actor2 = 0U;

UINT8 _i = 0U;

void Initialize_TestActor1(struct ACTOR* a);
void UpdateTestActor1(struct ACTOR* a);

void _init_test_scene()
{
	_test_scene_initialized = 1U;

	disable_interrupts();

	SHOW_SPRITES;
	SHOW_BKG;
	
	/* test music 
	gbt_enable_channels(0x00);
	gbt_play(song_Data, 2, 7);
    gbt_loop(0);
    set_interrupts(VBL_IFLAG);
	*/

	/* put some temp sprites over the nintendo ones */
	SWITCH_ROM_MBC1(BANK_GRAPHICS_DATA_1);
	set_sprite_data(VRAM_START, spr_gameplayLength, spr_gameplay);
	/* TODO: define these VRAM offsets */
	set_sprite_data(VRAM_ACTORS, spr_actors_forestLength, spr_actors_forest);

	

	/* ------test code here----------- */

	if(create_actor(ACTOR_ENEMY_1) == ACTOR_AVAILABLE)
	{
		actor1 = created_actor_ptr;
		
		/* replace init/update functions to test */
		actor1->Initialize = &Initialize_TestActor1;
		actor1->Update = &UpdateTestActor1;

		actor1->Initialize(actor1);

		fill_bkg_rect(1, 1, 1, 1, UI_NUM_1);
	}
	else
	{
		fill_bkg_rect(1, 1, 1, 1, UI_NUM_0);
	}

	if(create_actor(ACTOR_ENEMY_1) == ACTOR_AVAILABLE)
	{
		actor2 = created_actor_ptr;
		
		/* replace init/update functions to test */
		actor2->Initialize = &Initialize_TestActor1;
		actor2->Update = &UpdateTestActor1;

		actor2->Initialize(actor2);

		fill_bkg_rect(2, 1, 1, 1, UI_NUM_1);
	}
	else
	{
		fill_bkg_rect(2, 1, 1, 1, UI_NUM_0);
	}

	actor1->PositionX = 50U;
	actor1->PositionY = 100U;
	actor1->CurAnimFramePtr = frames_crow_idle[0U];
	actor1->AnimTimer = actor1->CurAnimFramePtr->FrameDuration;

	actor2->PositionX = 80U;
	actor2->PositionY = 100U;
	actor2->CurAnimFramePtr = frames_crow_idle[0U];
	actor2->AnimTimer = actor2->CurAnimFramePtr->FrameDuration - 60;

	/* ----------------*/

	enable_interrupts();

	FadeFromBlack(4U);
}


void _test_scene_update()
{
	if(_test_scene_initialized == 0U)
	{
		_init_test_scene();
	}

	if(actor1 != 0U)
	{
		actor1->Update(actor1);
	}

	if(actor2 != 0U)
	{
		actor2->Update(actor2);
	}
}

void Initialize_TestActor1(struct ACTOR* a)
{
	/* Set the tiles for the first frame */
	UINT8 frame_spritecount = 0U;
	frame_spritecount = a->CurAnimFramePtr->NumTiles;

	_i = 0U;
	for(_i = 0U; _i < a->SpritesAllocated; _i = _i+1)
	{
		if(_i < frame_spritecount)
		{
			set_sprite_tile(a->SpriteIndexes[_i], VRAM_ACTORS + a->CurAnimFramePtr->AnimTiles[_i].Tile);
		}
		else
		{
			/* hide unused sprites */
			move_sprite(a->SpriteIndexes[_i], 0, 0);
		}
	}
}

void UpdateTestActor1(struct ACTOR* a)
{
	_i = 0U;
	UINT16 py;
	UINT16 px;

	UINT8 frame_spritecount = 0U;
	frame_spritecount = a->CurAnimFramePtr->NumTiles;

	/* Position the tiles in relation to the actor origin */
	for(_i = 0U; _i < a->SpritesAllocated; _i = _i+1)
	{
		if(_i < frame_spritecount)
		{
			py = a->PositionY;
			px = a->PositionX;

			py = py + a->CurAnimFramePtr->AnimTiles[_i].YOffset;
			px = px + a->CurAnimFramePtr->AnimTiles[_i].XOffset;

			move_sprite(a->SpriteIndexes[_i], px, py);

			if(a->UpdateSprites == 1U)
			{
				set_sprite_tile(a->SpriteIndexes[_i], VRAM_ACTORS + a->CurAnimFramePtr->AnimTiles[_i].Tile);
			}
		}
		else
		{
			/* hide unused sprites */
			move_sprite(a->SpriteIndexes[_i], 0, 0);
		}
	}


	a->UpdateSprites = 0U;
	a->AnimTimer = a->AnimTimer - 1;
	if(a->AnimTimer == 0U)
	{
		a->CurAnimFrameIndex = a-> CurAnimFrameIndex + 1U;
		if(a->CurAnimFrameIndex > 1U)
		{
			a->CurAnimFrameIndex = 0U;
		}

		a->CurAnimFramePtr = frames_crow_idle[a->CurAnimFrameIndex];

		a->AnimTimer = a->CurAnimFramePtr->FrameDuration;

		a->UpdateSprites = 1U;
	}
}
