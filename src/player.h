#ifndef PLAYER_H
#define PLAYER_H
#include <gb/gb.h>
#include "input.h"

#define DEBUG_MOVEMENT

/* init the camera in the bottom left corner of the map for now */
#define CAMERA_START_X 0U
#define CAMERA_START_Y ((32U*8U) - 145U)

#define CAMERA_TOP_BOUND 0U
#define CAMERA_BTM_BOUND ((255U) - 144U)
#define CAMERA_LEFT_BOUND 0U
#define CAMERA_RIGHT_BOUND ((255U) - 160U)

#define SCREEN_HEIGHT 144U
#define SCREEN_WIDTH 160U

#define PLAYER_SPRITE_WIDTH 8U
#define PLAYER_SPRITE_HEIGHT 16U

#define PLAYER_WIDTH 8U
#define PLAYER_HEIGHT 16U
#define PLAYER_HALF_HEIGHT 8U

#define CAMERA_OFFSET_X ((160U/2U) - (4U))
#define CAMERA_OFFSET_Y ((144U/2U) - (8U))

#define TILE_SIZE 8U

#define RISING_GRAVITY_CONSTANT 4
#define FALLING_GRAVITY_CONSTANT 1
#define PLAYER_JUMP_INERTIA 72
#define PLAYER_TERMINAL_VEL 28

#define SUBPIXELS 16
#define SUBPIX_SHIFT 4

#define PLAYER_ACCEL_X 2
#define PLAYER_MAX_SPEED_H 18
#define PLAYER_DECEL_X 8

/* how long after falling can we still jump */
#define JUMP_GRACE_BUFFER 6
/* how many frames before landing should we accept jump input? */
#define REJUMP_BUFFER 8

extern UINT8 jump_buffer;
extern UINT8 jump_last_pressed;

extern UINT8 player_fell;

extern INT8 player_speed_x;

extern INT8 player_move_x; /* temp movement vector for collision calculations */
extern INT8 player_move_y;

extern UINT8 player_grounded;
extern INT8 player_intertia_y;

extern UINT16 player_position_x; /* the players position in sub-pixel world-space */
extern UINT16 player_position_y;
extern UINT16 player_world_x; /* the players position in world-space */
extern UINT16 player_world_y;

extern UINT8 player_sub_x; /* todo: sub-pixel motion */
extern UINT8 player_sub_y;

extern UINT8 player_sprite_num;		/* which sprite number the player occupies, should always be 0 */
extern UINT8 player_sprite_tile; /* the tile data for player graphic */

extern const UINT8 player_speed;

extern UINT16 camera_x; /* the screen scroll offsets */
extern UINT16 camera_y;
extern UINT8 player_scr_x; /* the scree-space sprite position for the player */
extern UINT8 player_scr_y;

void init_game_camera();

void init_player_sprite();
void update_player();
void update_camera();

/* temp place for collision functions */
void HandleCollisionHorizontal();
void HandleCollisionVertical();

/* collision vars */
#define TILE_SOLID 0x01
#define TILE_EMPTY 0xFF
#define COLLISION_FOUND 1U
#define NO_COLLISION_FOUND 0U
#define COLLIDER_TILE_SIZE 8U
extern UINT16 collision_grid_test_x;
extern UINT16 collision_grid_test_y;
extern UINT8 collision_grid_test_result;

#define GROUNDED 1U
#define NOT_GROUNDED 0U

/* some temp vars */
extern UINT8 u8Temp1, u8Temp2;
extern INT8 i8Temp1, i8Temp2;
extern UINT16 u16Temp1, u16Temp2;

/* expects values to be in collision_grid_test_x and collision_grid_test_y */
void TestCollisionAtWorldPosition(); 
void TestCollisionAtGridPosition(); 
void TestGrounded(); 

void handle_scroll_horizontal();
void handle_scroll_vertical();

#endif