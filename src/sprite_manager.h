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
	---------- SHAPE MANAGEMENT -------------
	a shape is a collection of sprites associated with a specific layout
	eg 2x2 or 2x3

*/

/* For now all shapes are rectanguler with a Width
	and height in tile size
*/

/* can't see the gameboy being able to handle 32 meta sprites anyway but I can lower this if necessary */
#define MAX_SPRITES_PER_SHAPE 8
#define MAX_SHAPES 32

#define SHAPE_FREE 0x00
#define SHAPE_USED 0x01

#define SHAPE_NOT_AVAILABLE 0x00
#define SHAPE_AVAILABLE 0x01

typedef struct SHAPE {

	UINT8 ShapeType;

	/* just so we know which shape this is if we only have a ptr to the shape */
	UINT8 ShapeIndex; 

	UINT8 Width;
	UINT8 Height;
	UINT8 NumSprites;

	/* render X and Y value */
	/* origin is assumed to be bottom right of the total sprite area */
	UINT16 PositionX;
	UINT16 PositionY;

	/* ptr to our shapes array of sprite indices in the OAM table */
	UINT8 (* SpriteArrayPtr)[MAX_SPRITES_PER_SHAPE];

	/* function pointers */
	void (*Hide) (struct SHAPE * shape_ptr);
	void (*Place) (struct SHAPE * shape_ptr);
};

/* reserve some ram for the shapes to store their sprite arrays in */
extern UINT8 shape_sprite_arrays[MAX_SHAPES][MAX_SPRITES_PER_SHAPE];

extern struct SHAPE shapes_array[MAX_SHAPES];

extern UINT8 shapes_in_use[MAX_SHAPES];

extern struct SHAPE * created_shape_ptr;

void init_shape_manager();
UINT8 create_shape(UINT8 shape_type);
void release_shape(struct SHAPE * shape_ptr);

#define SHAPE_1x1 0U
#define SHAPE_2x2 1U
#define SHAPE_2x3 2U
#define SHAPE_2x1 3U
#define SHAPE_1x2 4U
#define NUM_DEFINED_SHAPES 5
/* add more shapes if we need em */

/* lookup table for shapes and shape functions */
typedef struct SHAPE_DEFINITION {
	UINT8 ShapeType;
	UINT8 Width;
	UINT8 Height;
	UINT8 NumSprites;

	/* function pointers */
	void (*Hide) (struct SHAPE * shape_ptr);
	void (*Place) (struct SHAPE * shape_ptr);
};

extern const struct SHAPE_DEFINITION shapes_defs[NUM_DEFINED_SHAPES];

/* sprite position functions */
/* call these after updating the shapes x and y coords to position the sprites
	in screen space

	sprites are placed as if the bottom right of the shape is the origin
*/
void hide_shape(struct SHAPE * shape_ptr);

void place_shape_1x1(struct SHAPE * shape_ptr);
void place_shape_2x2(struct SHAPE * shape_ptr);
void place_shape_2x3(struct SHAPE * shape_ptr);
void place_shape_2x1(struct SHAPE * shape_ptr); 
void place_shape_1x2(struct SHAPE * shape_ptr);

/* sprite flip functions */

#endif