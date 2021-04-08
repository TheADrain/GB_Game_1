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

/* some vars used by anim code - volatile */
UINT8 a_i = 0U;
UINT8 _i = 0U;
UINT8 frame_spritecount = 0U;
UINT8 sprites_allocated_count = 0U;
INT8 anim_tmr = 0;