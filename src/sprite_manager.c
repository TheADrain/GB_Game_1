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
/* think of a 'shape' as a meta sprite */

UINT8 shape_sprite_arrays[MAX_SHAPES][MAX_SPRITES_PER_SHAPE];
struct SHAPE shapes_array[MAX_SHAPES];

UINT8 shapes_in_use[MAX_SHAPES];
/* temp storage var for the created shapes ptr */
/* 
	intended use: 
	SHAPE* ourNewShape;
	if(create_shape(shapeType) == SHAPE_AVAILABLE) 
	{
		ourNewShape = created_shape_ptr;
	}
*/

struct SHAPE * created_shape_ptr;

void init_shape_manager()
{
	UINT8 i = 0U;
	for(i = 0; i < MAX_SHAPES; i = i + 1)
	{
		shapes_array[i].ShapeType = 0U;
		shapes_array[i].ShapeIndex = i;
		shapes_array[i].Width = 0U;
		shapes_array[i].Height = 0U;
		shapes_array[i].PositionX = 0U;
		shapes_array[i].PositionY = 0U;
		shapes_array[i].SpriteArrayPtr = &shape_sprite_arrays[i]; 
	}

	i = 0;
	for(i = 0; i < MAX_SHAPES; i = i + 1)
	{
		shapes_in_use[i] = SHAPE_FREE;
	}
}

UINT8 create_shape(UINT8 shape_type)
{
	if(shape_type >= NUM_DEFINED_SHAPES)
	{
		return SHAPE_NOT_AVAILABLE;
	}

	/* init a new shape if we can, if not return a failure result */
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

		/* populate the rest of the shape struct */
		shapes_array[shape_index].ShapeType = shapes_defs[shape_type].ShapeType;
		shapes_array[shape_index].ShapeIndex = shape_index;
		shapes_array[shape_index].NumSprites = num_sprites;
		shapes_array[shape_index].Width = shapes_defs[shape_type].Width;
		shapes_array[shape_index].Height = shapes_defs[shape_type].Height;

		shapes_array[shape_index].PositionX = 0U;
		shapes_array[shape_index].PositionY = 0U;

		/* assign func pointers */
		shapes_array[shape_index].Hide = shapes_defs[shape_type].Hide;
		shapes_array[shape_index].Place = shapes_defs[shape_type].Place;

		/* get our sprites from the grabbed_sprites array */
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

const struct SHAPE_DEFINITION shapes_defs[NUM_DEFINED_SHAPES] = {
	{
		SHAPE_1x1,
		1U, /* width */
		1U, /* height */
		1U, /* numsprites */
		/* function pointers */
		&hide_shape,
		&place_shape_1x1
	},
	{
		SHAPE_2x2,
		2U, /* width */
		2U, /* height */
		4U, /* numsprites */
		&hide_shape,
		&place_shape_2x2
	},
	{
		SHAPE_2x3,
		2U, /* width */
		3U, /* height */
		6U, /* numsprites */
		&hide_shape,
		&place_shape_2x3
	},
	{
		SHAPE_2x1,
		2U, /* width */
		1U, /* height */
		2U, /* numsprites */
		&hide_shape,
		&place_shape_2x1
	},
	{
		SHAPE_1x2,
		1U, /* width */
		2U, /* height */
		2U, /* numsprites */
		&hide_shape,
		&place_shape_1x2
	}
};

UINT8 spr_index_temp = 0U;
void hide_shape(struct SHAPE * shape_ptr)
{
	UINT8 i = 0U;
	UINT8 limit = shape_ptr->NumSprites;
	for(i = 0; i < limit; i = i + 1)
	{
		spr_index_temp	= (*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[i];
		move_sprite(spr_index_temp, 0U, 0U);
	}
}

UINT16 t_pos_x = 0U;
UINT16 t_pos_y = 0U;
void place_shape_1x1(struct SHAPE * shape_ptr) 
{
	spr_index_temp	= (*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[0U];

	t_pos_x = (shape_ptr->PositionX - SCX_REG) + 8;
	t_pos_y = (shape_ptr->PositionY - SCY_REG) + 16;

	move_sprite(spr_index_temp, t_pos_x, t_pos_y);
}

void place_shape_2x2(struct SHAPE * shape_ptr) 
{
	t_pos_x = (shape_ptr->PositionX - SCX_REG) + 8;
	t_pos_y = (shape_ptr->PositionY - SCY_REG) + 16;

	spr_index_temp	= (*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[0U];
	move_sprite(spr_index_temp, t_pos_x - 8, t_pos_y - 8);

	spr_index_temp	= (*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[1U];
	move_sprite(spr_index_temp, t_pos_x, t_pos_y - 8);

	spr_index_temp	= (*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[2U];
	move_sprite(spr_index_temp, t_pos_x - 8, t_pos_y);

	spr_index_temp	= (*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[3U];
	move_sprite(spr_index_temp, t_pos_x, t_pos_y);
}

void place_shape_2x3(struct SHAPE * shape_ptr) 
{
	t_pos_x = (shape_ptr->PositionX - SCX_REG) + 8;
	t_pos_y = (shape_ptr->PositionY - SCY_REG) + 16;

	spr_index_temp	= (*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[0U];
	move_sprite(spr_index_temp, t_pos_x - 8, t_pos_y - 16);

	spr_index_temp	= (*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[1U];
	move_sprite(spr_index_temp, t_pos_x, t_pos_y - 16);

	spr_index_temp	= (*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[2U];
	move_sprite(spr_index_temp, t_pos_x - 8, t_pos_y - 8);

	spr_index_temp	= (*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[3U];
	move_sprite(spr_index_temp, t_pos_x, t_pos_y - 8);

	spr_index_temp	= (*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[4U];
	move_sprite(spr_index_temp, t_pos_x - 8, t_pos_y);

	spr_index_temp	= (*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[5U];
	move_sprite(spr_index_temp, t_pos_x, t_pos_y);
}

void place_shape_2x1(struct SHAPE * shape_ptr) 
{
	t_pos_x = (shape_ptr->PositionX - SCX_REG) + 8;
	t_pos_y = (shape_ptr->PositionY - SCY_REG) + 16;

	spr_index_temp	= (*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[0U];
	move_sprite(spr_index_temp, t_pos_x-8, t_pos_y);

	spr_index_temp	= (*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[1U];
	move_sprite(spr_index_temp, t_pos_x, t_pos_y);
}

void place_shape_1x2(struct SHAPE * shape_ptr) 
{
	t_pos_x = (shape_ptr->PositionX - SCX_REG) + 8;
	t_pos_y = (shape_ptr->PositionY - SCY_REG) + 16;

	spr_index_temp	= (*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[0U];
	move_sprite(spr_index_temp, t_pos_x, t_pos_y-8);

	spr_index_temp	= (*shapes_array[shape_ptr->ShapeIndex].SpriteArrayPtr)[1U];
	move_sprite(spr_index_temp, t_pos_x, t_pos_y);
}