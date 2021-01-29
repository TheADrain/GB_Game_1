#ifndef SCROLLING_H
#define SCROLLING_H
#include <gb/gb.h>
#include "player.h"
#include "levels.h"

#define LEFT_BUFFER 8
#define RIGHT_BUFFER 168

extern UINT16 scrolling_position;

void init_scrolling();
void handle_scrolling();

#endif