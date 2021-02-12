#ifndef ANIMS_FOREST_H
#define ANIMS_FOREST_H
#include <gb/gb.h>
#include "../sprite_manager.h"
/* definitions for the shapes used by forest level actors */

/* temp: define here how many hardware sprites the crow will use */
#define CROW_RESERVE_SPRITES 4U;

#define CROW_FRAMES 4U
#define CROW_FRAME_PERCHED_1 0U
#define CROW_FRAME_PERCHED_2 1U
#define CROW_FRAME_FLY 2U
#define CROW_FRAME_SWOOP 3U

extern const struct ANIM_FRAME frame_crow_perched_1;
extern const struct ANIM_FRAME frame_crow_perched_2;
extern const struct ANIM_FRAME frame_crow_fly;
extern const struct ANIM_FRAME frame_crow_swoop;

extern const struct ANIM_FRAME *const frames_crow[CROW_FRAMES];

extern const struct ANIM_FRAME *const frames_crow_idle[2U];

/* temp: define here how many hardware sprites the skelly will use */
#define SKELLY_RESERVE_SPRITES 5U;

#define SKELLY_FRAMES 3U
#define SKELLY_STAND_FRAME 0U
#define SKELLY_WALK_FRAMES_START 1U
#define SKELLY_WALK_FRAMES_END 2U

extern const struct ANIM_FRAME skelly_stand;	
extern const struct ANIM_FRAME skelly_walk_1;
extern const struct ANIM_FRAME skelly_walk_2;

extern const struct ANIM_FRAME *const frames_skelly[SKELLY_FRAMES];

#endif