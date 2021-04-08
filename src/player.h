#ifndef PLAYER_H
#define PLAYER_H
#include <gb/gb.h>
#include "input.h"
#include "collision.h"
#include "actors.h"

/* init the camera in the bottom left corner of the map for now */
#define CAMERA_START_X 0U
#define CAMERA_START_Y ((32U*8U) - 145U)

#define CAMERA_TOP_BOUND 0U
#define CAMERA_BTM_BOUND ((255U) - 144U)
#define CAMERA_LEFT_BOUND 0U
#define CAMERA_RIGHT_BOUND ((255U) - 160U)

#define SCREEN_HEIGHT 144U
#define SCREEN_WIDTH 160U

#define PLAYER_SPRITE_WIDTH 16U
#define PLAYER_SPRITE_HEIGHT 32U

#define PLAYER_WIDTH 14U 
#define PLAYER_HALF_WIDTH 7U
#define PLAYER_HEIGHT 14U
#define PLAYER_HALF_HEIGHT 7U
#define PLAYER_HEIGHT_CHECK_1 11U
#define PLAYER_HEIGHT_CHECK_2 4U

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

#define GROUNDED 1U
#define NOT_GROUNDED 0U

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

extern UINT16 prev_player_world_x; 
extern UINT16 prev_player_world_y;

extern UINT8 player_sub_x; /* todo: sub-pixel motion */
extern UINT8 player_sub_y;

extern UINT8 player_sprite_num;		/* which sprite number the player occupies, should always be 0 */
extern UINT8 player_sprite_tile; /* the tile data for player graphic */

extern const UINT8 player_speed;

extern struct ACTOR * player_actor;

/* init and update functions */

void init_player_sprite();
void cleanup_player_sprite();
void InitializePlayerActor(struct ACTOR* a);
void UpdatePlayerActor(struct ACTOR* a);
void update_player();



/* collision functions */
#define COLLIDER_TILE_SIZE 8U
extern UINT16 collision_grid_test_x;
extern UINT16 collision_grid_test_y;
extern UINT8 collision_grid_test_result;
/* some temp vars */
extern UINT8 u8Temp1, u8Temp2;
extern INT8 i8Temp1, i8Temp2;
extern UINT16 u16Temp1, u16Temp2;

/* temp place for collision functions */
void HandleCollisionHorizontal();
void HandleCollisionVertical();

/* expects values to be in collision_grid_test_x and collision_grid_test_y */
void TestCollisionAtWorldPosition(); 
void TestCollisionAtGridPosition(); 
void TestGrounded(); 

/* player state functions */
extern UINT8 player_state;
#define PLAYER_STATE_IDLE 0U
#define PLAYER_STATE_JUMPING 1U
#define PLAYER_STATE_FALLING 2U
#define PLAYER_STATE_DASHING 3U
#define PLAYER_STATE_DROPPING_FROM_PLATFORM 4U
#define PLAYER_STATE_HANGING_FROM_PLATFORM 5U

#define DASH_HORIZ_SPEED 12U
#define DASH_VERT_SPEED 12U
#define DASH_DURATION 120U
extern UINT8 dash_timer;

void HandlePlayerStateIdle();
void HandlePlayerStateJumping();
void HandlePlayerStateFalling();

void HandlePlayerDashing();
void HandlePlayerDroppingFromPlatform();
void HandlePlayerHangingFromPlatform();

#endif