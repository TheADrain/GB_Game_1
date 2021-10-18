#ifndef LEVEL_LOADING_H
#define LEVEL_LOADING_H
#include <gb/gb.h>

void levelcard_init();
void levelcard_update();

void load_current_level();

void set_current_level(UINT8 newLevel);
void increment_sub_level_data();
void load_current_level_graphics();
void load_current_level_map();	
void load_current_horizontal_level_map_chunk();
void load_current_vertical_level_map_chunk();
void start_level();
void end_level();

extern INT8 chunk_v_offset;
extern INT8 chunk_h_offset;

#endif
