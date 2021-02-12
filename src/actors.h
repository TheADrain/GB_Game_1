#ifndef ACTORS_H
#define ACTORS_H
#include <gb/gb.h>

#define MAX_ACTORS 16

#define ACTOR_FREE 0x00
#define ACTOR_USED 0x01

#define ACTOR_NOT_AVAILABLE 0x00
#define ACTOR_AVAILABLE 0x01

#define MAX_SPRITES_PER_ACTOR 8U

typedef struct ACTOR {

	UINT8 ActorType;
	UINT8 ActorIndex; 

	/* hitbox width/height */
	UINT8 Width;
	UINT8 Height;
	
	UINT16 PositionX;
	UINT16 PositionY;

	//struct ANIM_FRAME *const CurAnim;
	struct ANIM_FRAME * CurAnimFramePtr;
	UINT8 CurAnimFrameIndex;
	UINT8 AnimTimer;

	UINT8 UpdateSprites;

	UINT8 SpritesAllocated;
	UINT8 SpriteIndexes[MAX_SPRITES_PER_ACTOR];

	/* function pointers */
	void (*Initialize) (struct ACTOR * actor_ptr);
	void (*Update) (struct ACTOR * actor_ptr);
};

extern struct ACTOR actors_array[MAX_ACTORS];
extern UINT8 actors_in_use[MAX_ACTORS];
extern struct ACTOR * created_actor_ptr;

void init_actor_manager();
UINT8 create_actor(UINT8 actor_type);
void release_actor(struct ACTOR * actor_ptr);

/* ----------- ACTOR DEFINITIONS ------------- */

#define ACTOR_EMPTY 0U
#define ACTOR_ENEMY_1 1U
/* etc */
#define NUM_DEFINED_ACTORS 3

/* lookup table for shapes and shape functions */
typedef struct ACTOR_DEFINITION {
	UINT8 ActorType;
	UINT8 Width;
	UINT8 Height;

	/* HOW MANY SPRITES TO RESERVE */
	UINT8 SpriteAllocation;

	/* function pointers */
	void (*Initialize) (struct ACTOR * actor_ptr);
	void (*Update) (struct ACTOR * actor_ptr);
};


extern const struct ACTOR_DEFINITION actor_defs[NUM_DEFINED_ACTORS];

void Initialize_EmptyActor(struct ACTOR* actor_ptr);
void UpdateEmptyActor(struct ACTOR* actor_ptr);

void Initialize_TestActor(struct ACTOR* actor_ptr);
void UpdateTestActor(struct ACTOR* actor_ptr);

#endif