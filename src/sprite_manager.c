#include "sprite_manager.h"

UINT8 used_sprites = 0U;
UINT8 free_sprites = MAX_SPRITES;

UBYTE sprite_list[MAX_SPRITES];

UINT8 grabbed_sprites_length = 0U; 
UINT8 grabbed_sprites_buffer[MAX_SPRITES];

UINT8 s_i = 0U;
UINT8 s_temp1 = 0U;
UINT8 s_temp2 = 0U;

void init_sprite_manager()
{
	for(s_i = 0; s_i < MAX_SPRITES; s_i = s_i + 1)
	{
		sprite_list[s_i] = SPRITE_FREE;
		grabbed_sprites_buffer[s_i] = 0x00;
	}
}

UBYTE grab_sprites(UINT8 num_sprites)
{
	grabbed_sprites_length = 0U;

	for(s_i = 0; s_i < MAX_SPRITES; s_i = s_i + 1)
	{
		if(grabbed_sprites_length < num_sprites)
		{
			if(sprite_list[s_i] == SPRITE_FREE)
			{
				/* populate the grabbed sprites buffer so we don't need to loop again if successful */
				grabbed_sprites_buffer[grabbed_sprites_length] = s_i;
				grabbed_sprites_length = grabbed_sprites_length + 1;
			}
		}
	}

	/* if we have enough free sprites, mark them as used and return success */
	if(grabbed_sprites_length >= num_sprites)
	{
		mark_sprites_used();
		return SPRITES_AVAILABLE;
	}

	return SPRITES_NOT_AVAILABLE;
}

void mark_sprites_used()
{
	/* grabbed list is pre-populated by the check function, here we just mark them as grabbed */
	s_temp1 = 0U;
	for(s_i = 0; s_i <= grabbed_sprites_length; s_i = s_i + 1)
	{
		s_temp1 = grabbed_sprites_buffer[s_i];
		sprite_list[s_temp1] = SPRITE_USED;		
	}
}

void release_sprite(UINT8 spritenum)
{
	sprite_list[spritenum] = SPRITE_FREE;
	move_sprite(spritenum, 0, 0);
}

/* ==================== SHAPES ==================================*/

/*
UINT8 create_shape(UINT8 shape_type)
{
	if(shape_type >= NUM_DEFINED_SHAPES)
	{
		return SHAPE_NOT_AVAILABLE;
	}

	UINT8 i = 0U;
	UINT8 shape_index = 0U;
	for(i = 0; i < MAX_SHAPES; i = i + 1)
	{
		if(shapes_in_use[i] == SHAPE_FREE)
		{
			shape_index = i;
			break;
		}
	}

	UINT8 num_sprites = shapes_defs[shape_type].NumSprites;

	if(grab_sprites(num_sprites))
	{
		shapes_in_use[shape_index] = SHAPE_USED;

		shapes_array[shape_index].ShapeType = shapes_defs[shape_type].ShapeType;
		shapes_array[shape_index].ShapeIndex = shape_index;
		shapes_array[shape_index].NumSprites = num_sprites;
		shapes_array[shape_index].Width = shapes_defs[shape_type].Width;
		shapes_array[shape_index].Height = shapes_defs[shape_type].Height;

		shapes_array[shape_index].PositionX = 0U;
		shapes_array[shape_index].PositionY = 0U;

		shapes_array[shape_index].Hide = shapes_defs[shape_type].Hide;
		shapes_array[shape_index].Place = shapes_defs[shape_type].Place;

		UINT8 j = 0;
		for(j = 0; j < grabbed_sprites_length; j = j + 1)
		{
			(*shapes_array[shape_index].SpriteArrayPtr)[j] = grabbed_sprites_buffer[j];
		}

		created_shape_ptr = &(shapes_array[shape_index]);

		return SHAPE_AVAILABLE;
	}

	return SHAPE_NOT_AVAILABLE;
}

void release_shape(struct SHAPE * shape_ptr)
{
	UINT8 shape_index = shape_ptr->ShapeIndex;
	UINT8 numSpritesToClear = shape_ptr->NumSprites;

	shapes_array[shape_index].ShapeType = 0U;
	shapes_array[shape_index].Width = 0U;
	shapes_array[shape_index].Height = 0U;
	shapes_array[shape_index].NumSprites = 0U;
	shapes_array[shape_index].PositionX = 0U;
	shapes_array[shape_index].PositionY = 0U;

	UINT8 i = 0U;
	for(i = 0; i < numSpritesToClear; i = i + 1)
	{
		release_sprite((*shapes_array[shape_index].SpriteArrayPtr)[i]);
		(*shapes_array[shape_index].SpriteArrayPtr)[i] = 0U;
	}

	shapes_in_use[shape_index] = SHAPE_FREE;
}
*/