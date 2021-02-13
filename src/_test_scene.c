#include "anims/anims_forest.h"

UINT8 _test_scene_initialized = 0U;


struct ACTOR * actor1 = 0U;
struct ACTOR * actor2 = 0U;

struct ACTOR * actor3 = 0U;
struct ACTOR * actor4 = 0U;

struct ACTOR * actor5 = 0U;
struct ACTOR * actor6 = 0U;

struct ACTOR * test_actors[6U];

UINT8 a_i = 0U;
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
		test_actors[0U] = actor1;
		
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
		test_actors[1U] = actor2;
		
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

	if(create_actor(ACTOR_ENEMY_1) == ACTOR_AVAILABLE)
	{
		actor3 = created_actor_ptr;
		test_actors[2U] = actor3;
		
		/* replace init/update functions to test */
		actor3->Initialize = &Initialize_TestActor1;
		actor3->Update = &UpdateTestActor1;

		actor3->Initialize(actor3);

		fill_bkg_rect(3, 1, 1, 1, UI_NUM_1);
	}
	else
	{
		fill_bkg_rect(3, 1, 1, 1, UI_NUM_0);
	}

	if(create_actor(ACTOR_ENEMY_1) == ACTOR_AVAILABLE)
	{
		actor4 = created_actor_ptr;
		test_actors[3U] = actor4;
		
		/* replace init/update functions to test */
		actor4->Initialize = &Initialize_TestActor1;
		actor4->Update = &UpdateTestActor1;

		actor4->Initialize(actor4);

		fill_bkg_rect(4, 1, 1, 1, UI_NUM_1);
	}
	else
	{
		fill_bkg_rect(4, 1, 1, 1, UI_NUM_0);
	}

	if(create_actor(ACTOR_ENEMY_1) == ACTOR_AVAILABLE)
	{
		actor5 = created_actor_ptr;
		test_actors[4U] = actor5;
		
		/* replace init/update functions to test */
		actor5->Initialize = &Initialize_TestActor1;
		actor5->Update = &UpdateTestActor1;

		actor5->Initialize(actor5);

		fill_bkg_rect(5, 1, 1, 1, UI_NUM_1);
	}
	else
	{
		fill_bkg_rect(5, 1, 1, 1, UI_NUM_0);
	}

	if(create_actor(ACTOR_ENEMY_1) == ACTOR_AVAILABLE)
	{
		actor6 = created_actor_ptr;
		test_actors[5U] = actor6;

		/* replace init/update functions to test */
		actor6->Initialize = &Initialize_TestActor1;
		actor6->Update = &UpdateTestActor1;

		actor6->Initialize(actor6);

		fill_bkg_rect(6, 1, 1, 1, UI_NUM_1);
	}
	else
	{
		fill_bkg_rect(6, 1, 1, 1, UI_NUM_0);
	}

	actor1->PositionX = 50U;
	actor1->PositionY = 110U;
	actor1->CurAnimFramePtr = frames_crow_idle[0U];
	actor1->AnimTimer = actor1->CurAnimFramePtr->FrameDuration;

	actor2->PositionX = 70U;
	actor2->PositionY = 100U;
	actor2->CurAnimFramePtr = frames_crow_idle[0U];
	actor2->AnimTimer = actor2->CurAnimFramePtr->FrameDuration - 60;

	actor3->PositionX = 30U;
	actor3->PositionY = 110U;
	actor3->CurAnimFramePtr = frames_crow_idle[0U];
	actor3->AnimTimer = actor3->CurAnimFramePtr->FrameDuration;

	actor4->PositionX = 110U;
	actor4->PositionY = 100U;
	actor4->CurAnimFramePtr = frames_crow_idle[0U];
	actor4->AnimTimer = actor4->CurAnimFramePtr->FrameDuration - 30;

	actor5->PositionX = 30U;
	actor5->PositionY = 70U;
	actor5->CurAnimFramePtr = frames_crow_idle[0U];
	actor5->AnimTimer = actor5->CurAnimFramePtr->FrameDuration;

	actor6->PositionX = 90U;
	actor6->PositionY = 70U;
	actor6->CurAnimFramePtr = frames_crow_idle[0U];
	actor6->AnimTimer = actor6->CurAnimFramePtr->FrameDuration - 90;

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

	for(a_i = 0U; a_i < 6U; a_i = a_i + 1)
	{
		test_actors[a_i]->Update(test_actors[a_i]);
	}
}

UINT8 frame_spritecount = 0U;
UINT8 sprites_allocated_count = 0U;
UINT16 py = 0U;
UINT16 px = 0U;

void Initialize_TestActor1(struct ACTOR* a)
{
	/* Set the tiles for the first frame */
	
	frame_spritecount = a->CurAnimFramePtr->NumTiles;

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
		}
	}
}

void UpdateTestActor1(struct ACTOR* a)
{
	frame_spritecount = a->CurAnimFramePtr->NumTiles;
	sprites_allocated_count = a->SpritesAllocated;

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

		for(_i = 0U; _i < sprites_allocated_count; _i = _i+1)
		{
			if(_i >= frame_spritecount)
			{
				/* hide unused sprites */
				move_sprite(a->SpriteIndexes[_i], 0, 0);
			}
			else
			{
				set_sprite_tile(a->SpriteIndexes[_i], VRAM_ACTORS + a->CurAnimFramePtr->AnimTiles[_i].Tile);
			}
		}
	}

	/* Position the tiles in relation to the actor origin */
	for(_i = 0U; _i < frame_spritecount; _i = _i+1)
	{
		py = a->PositionY + a->CurAnimFramePtr->AnimTiles[_i].YOffset;
		px = a->PositionX + a->CurAnimFramePtr->AnimTiles[_i].XOffset;
		move_sprite(a->SpriteIndexes[_i], px, py);
	}
}
