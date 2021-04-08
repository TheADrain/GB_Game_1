#ifndef ANIMS_PLAYER_H
#define ANIMS_PLAYER_H
#include <gb/gb.h>
#include "../sprite_manager.h"
/* definitions for the shapes used by forest level actors */

#define PLAYER_IDLE_FRAMES 3U
extern const struct ANIM_FRAME anim_player_idle_frame_0;
extern const struct ANIM_FRAME anim_player_idle_frame_1;
extern const struct ANIM_FRAME anim_player_idle_frame_2;

extern const struct ANIMATION anim_player_idle;

#define PLAYER_JUMP_FRAMES 1U
extern const struct ANIM_FRAME anim_player_jump_frame_0;
extern const struct ANIMATION anim_player_jump;

#define PLAYER_FALL_FRAMES 1U
extern const struct ANIM_FRAME anim_player_fall_frame_0;
extern const struct ANIMATION anim_player_fall;

#endif