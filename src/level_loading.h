#ifndef LEVEL_LOADING_H
#define LEVEL_LOADING_H
#include <gb/gb.h>

void levelcard_init();
void levelcard_update();

void load_current_level();

void set_current_level(UINT8 newLevel);
void load_current_level_graphics();
void load_current_level_map();	
void start_level();
void end_level();

#endif
