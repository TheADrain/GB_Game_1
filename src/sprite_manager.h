#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H
#include <gb/gb.h>

/*
	---------- SPRITE MANAGEMENT -------------
	sprites.h contains the sprite engine and the code for bundling meta-sprites into
	a single point of reference

*/

/* 20 for 8x16 mode or 40 for 8x8 mode */
#define MAX_SPRITES 40

#define SPRITES_NOT_AVAILABLE 0x00
#define SPRITES_AVAILABLE 0x01

#define SPRITE_FREE 0x00
#define SPRITE_USED 0x01

extern UINT8 used_sprites;
extern UINT8 free_sprites;

/* flags for each sprite 

	0x00 == UNUSED
	0x01 == USED
	0x02... maybe other flags in future?
*/

/* internal sprite tracker */
extern UBYTE sprite_list[MAX_SPRITES];

/* when we successfully 'grab' sprites, get the sprite indexes from this array */
extern UINT8 grabbed_sprites_length; /* current length of this array */
extern UINT8 grabbed_sprites_buffer[MAX_SPRITES];

void init_sprite_manager();

/* checks to see if the number of sprites we want are available */
UBYTE grab_sprites(UINT8 num_sprites);

/* reserve sprites found in grab_sprites */
void mark_sprites_used();

/* objects should call this when they're done using a sprite */
void release_sprite(UINT8 spritenum);

/*
	---------- ANIM MANAGEMENT -------------
*/


/* now it is an ACTOR that will reserve a number of
	sprites, and it will manage the animations from there
*/



/**** experimental anim frame definitions *****/

#define FR_FLIP_NONE 0x00
#define FR_FLIP_X S_FLIPX
#define FR_FLIP_Y S_FLIPY
#define FR_FLIP_BOTH (S_FLIPX | S_FLIPY)

/* be sure to reserve enough sprites for
	the largest animation frame when creating
	 an actor */

typedef struct ANIM_TILE {
	INT8 YOffset;
	INT8 XOffset;
	UINT8 Tile;

	/* bit 1 (rightmost) == Y-flip
	   bit 2 == x-flip
	*/
	UBYTE Props; 
};

typedef struct ANIM_FRAME {
	UINT8 NumTiles;
	UINT8 FrameDuration;
	struct ANIM_TILE AnimTiles[];
};

/* some vars used by anim code - volatile */
extern UINT8 frame_spritecount;
extern UINT8 sprites_allocated_count;

extern UINT8 a_i;
extern UINT8 _i;

extern INT8 anim_tmr; 

typedef struct ANIMATION {
	UINT8 NumFrames;
	struct ANIM_FRAME* Frames[];
};

#endif