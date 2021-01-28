#ifndef MAIN_H
#define MAIN_H
#include <gb/gb.h>
/* ---------------------GAME FLOW-------------------------- */

/* GAME FLOW STATE */
#define GAMEFLOW_BOOT 0U
#define GAMEFLOW_SPLASH 1U
#define GAMEFLOW_TITLE 2U
#define GAMEFLOW_LEVELCARD 3U
#define GAMEFLOW_GAME 4U

/* GAME FLOW VARS */
extern UINT8 GAME_FLOW_STATE;

/* ---------------------GLOBAL VARS-------------------------- */

/* INPUT VARS */
extern UINT8 pad_state_temp ;

/* ---------------------LEVEL DATA-------------------------- */
extern UINT8 CUR_LEVEL;
extern UINT8 CUR_LEVEL_BANK;
extern UINT8 CUR_MAP_WIDTH;
extern UINT8 CUR_MAP_HEIGHT;
extern unsigned char* level_tilemap_data;
extern unsigned char* level_collision_data;

/* ---------------------FUNCTION DECLARATIONS------------------------- */
void vlbint(); /* v-blank interrupt routine */
/* game flow states */
void boot_init();
void boot_update();
void splash_init();
void splash_update();
void title_init();
void title_update();
void levelcard_init();
void levelcard_update();

void load_current_level();

void DoGraphicsUpdate();
/* ----------------------------------------------- */
#endif