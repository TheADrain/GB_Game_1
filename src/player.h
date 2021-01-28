#ifndef PLAYER_H
#define PLAYER_H
#include <gb/gb.h>
#include "input.h"

/* init the camera in the bottom left corner of the map for now */
#define CAMERA_START_X 0U
#define CAMERA_START_Y ((32U*8U) - 145U)

#define CAMERA_TOP_BOUND 0U
#define CAMERA_BTM_BOUND ((255U) - 144U)
#define CAMERA_LEFT_BOUND 0U
#define CAMERA_RIGHT_BOUND ((255U) - 160U)

#define SCREEN_HEIGHT 144U
#define SCREEN_WIDTH 160U

#define PLAYER_WIDTH 8U
#define PLAYER_HALF_WIDTH 4U
#define PLAYER_HEIGHT 16U
#define PLAYER__HALF_HEIGHT 8U

#define CAMERA_OFFSET_X ((160U/2U) - (4U))
#define CAMERA_OFFSET_Y ((144U/2U) - (8U))

extern INT16 player_world_x; /* the players position in world-space */
extern INT16 player_world_y;

extern UINT8 player_sub_x; /* todo: sub-pixel motion */
extern UINT8 player_sub_y;

extern UINT8 player_sprite_num;		/* which sprite number the player occupies, should always be 0 */
extern UINT8 player_sprite_tile; /* the tile data for player graphic */

extern const UINT8 player_speed;

extern UINT8 camera_x; /* the screen scroll offsets */
extern UINT8 camera_y;
extern UINT8 player_scr_x; /* the scree-space sprite position for the player */
extern UINT8 player_scr_y;

void init_game_camera();

void init_player_sprite();
void update_player();
void update_camera();

#endif