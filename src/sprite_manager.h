#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H
#include <gb/gb.h>

/*

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

#endif