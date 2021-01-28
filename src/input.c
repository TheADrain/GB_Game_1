#include <gb/gb.h>
#include "input.h"

char pad_state = 0x00;
char pad_state_prev = 0x00;
char pad_pressed = 0x00;
char pad_held = 0x00;
char pad_released = 0x00;

void poll_input()
{
	/* poll a couple times to avoid errors */
	joypad();
	joypad();

	/* get the stabilised input state */
	pad_state = joypad();

	/* temp to test macros */
	pad_pressed = pad_state & ~pad_state_prev;
	pad_held = pad_state & pad_state_prev;
	pad_released = pad_state_prev & ~pad_state; 
	
	/* record the previous state */
	pad_state_prev = pad_state;
}

void reset_input()
{
	pad_state = 0x00;
	pad_state_prev = 0x00;

	pad_pressed = 0x00;
	pad_held = 0x00;
	pad_released = 0x00;
}

