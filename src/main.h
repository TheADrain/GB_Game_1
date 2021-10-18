#ifndef MAIN_H
#define MAIN_H
#include <gb/gb.h>

#define _TRUE 1U
#define _FALSE 0U

/* Some useful memory address defines */
#define BG_MAP_1 0x9800U
#define BG_MAP_2 0x9C00U

#define VRAM_START 0x00U
#define VRAM_PLAYER 0x00U
#define VRAM_GAMEPLAY 0x50U
#define VRAM_MISC_UI_1 0x70U
#define VRAM_TSET_OVERFLOW 0x80U
#define VRAM_MISC_UI_2 0xA0U
#define VRAM_ACTORS 0xC0U

#define UI_NUM_0 (VRAM_MISC_UI_2+16U)
#define UI_NUM_1 (VRAM_MISC_UI_2+16U+1U)
#define UI_NUM_2 (VRAM_MISC_UI_2+16U+2U)
#define UI_NUM_3 (VRAM_MISC_UI_2+16U+3U)
#define UI_NUM_4 (VRAM_MISC_UI_2+16U+4U)
#define UI_NUM_5 (VRAM_MISC_UI_2+16U+5U)
#define UI_NUM_6 (VRAM_MISC_UI_2+16U+6U)
#define UI_NUM_7 (VRAM_MISC_UI_2+16U+7U)
#define UI_NUM_8 (VRAM_MISC_UI_2+16U+8U)
#define UI_NUM_9 (VRAM_MISC_UI_2+16U+9U)
#define UI_NUM_DASH (VRAM_MISC_UI_2+16U+10U)



//#define DEBUG_MOVEMENT
//#define START_DEBUG_LEVEL_1

#define FIRST_LVL 0U

#define USE_TEST_SCENE

/* ---------------------MUSIC-------------------------- */
#include "gbt_player.h"
extern const unsigned char * song_Data[];
/* ---------------------GAME FLOW-------------------------- */

/* GAME FLOW STATE */
#define GAMEFLOW_BOOT 0U
#define GAMEFLOW_SPLASH 1U
#define GAMEFLOW_TITLE 2U
#define GAMEFLOW_LEVELCARD 3U
#define GAMEFLOW_GAME 4U

/* GAME FLOW VARS */
extern UINT8 GAME_FLOW_STATE;

#define BKG_WIDTH 32
#define BKG_HEIGHT 32

/* ---------------------GLOBAL VARS-------------------------- */

/* INPUT VARS */
extern UINT8 pad_state_temp ;
extern UINT8 global_frame_count;

/* HELPER VARS */
/* volatile counters, don't expect them to persist after calling
	any function, always set to 0U before using */
extern UINT8 v_i;
extern UINT8 v_j;
extern UINT8 v_k;

/* ---------------------LEVEL DATA-------------------------- */
extern UINT8 CUR_LEVEL;
extern UINT8 CUR_SUBLEVEL;
extern UINT8 CUR_LEVEL_DATA_IDX;
extern UINT8 CUR_LEVEL_BANK;
extern UINT8 CUR_MAP_WIDTH;
extern UINT8 CUR_MAP_HEIGHT;
extern unsigned char* level_tilemap_data;
extern unsigned char* level_collision_data;

/* ---------------------FUNCTION DECLARATIONS------------------------- */
void vblint(); /* v-blank interrupt routine */
/* game flow states */
void boot_init();
void boot_update();
void splash_init();
void splash_update();
void title_init();
void title_update();

void CheckLevelCompletion();
void CheckLevelCompletion_ExitTiles();
void CheckLevelCompletion_RightBounds();
void CheckLevelCompletion_SmoothToNextChunk();

void DoGraphicsUpdate();
/* ----------------------------------------------- */
#endif