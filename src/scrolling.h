#ifndef SCROLLING_H
#define SCROLLING_H
#include <gb/gb.h>
#include "player.h"
#include "levels.h"
#include "main.h"

/* ============================================================================ */
/* =================== camera motion and level scrolling ====================== */
/* ============================================================================ */

extern UINT16 camera_x; /* the screen scroll offsets */
extern UINT16 camera_y;
extern UINT8 player_scr_x; /* the scree-space sprite position for the player */
extern UINT8 player_scr_y;

// store tile load commands until next vblank
extern UBYTE stored_tile_load_command;

extern UINT8 stored_tile_load_bkg_x;
extern UINT8 stored_tile_load_bkg_y;
extern UINT8 stored_tile_load_bkg_w;
extern UINT8 stored_tile_load_bkg_h;
extern unsigned char * stored_scrl_dat_ptr;

extern INT16 camera_bottom_bound;
extern INT16 camera_right_bound;

void init_game_camera();
void update_camera();

void handle_scroll_horizontal();
void handle_scroll_vertical();

#endif