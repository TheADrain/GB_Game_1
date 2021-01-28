#ifndef INPUT_H
#define INPUT_H

#include <gb/gb.h>

/* Util functions for processing input state */
/* use macros for actually testing inputs to avoid having to return a bool from functions */

#define P1F_5 0x20U /* %00100000 P15 out port, select button keys */
#define P1F_4 0x10U /* %00010000 P14 out port, select direction keys */
#define P1F_3 0x08U /* %00001000 P13 in port, down or start */
#define P1F_2 0x04U /* %00000100 P12 in port, up or select */
#define P1F_1 0x02U /* %00000010 P11 in port, left or b */
#define P1F_0 0x01U /* %00000001 P10 in port, right or a */

/* GBDK doesn't use the 2x3 grid selector it just fills a byte with all 8 buttons */

#define BTN_RIGHT	(0x01U)
#define BTN_LEFT	(0x02U)
#define BTN_UP		(0x04U)
#define BTN_DOWN	(0x08U)

#define BTN_A		(0x10U)
#define BTN_B		(0x20U)
#define BTN_SELECT	(0x40U)
#define BTN_START	(0x80U)

#define JOY_DOWN(__btn)	((__btn) & (pad_state))
#define JOY_PRESSED(__btn) ((__btn) & (pad_pressed))
#define JOY_HELD(__btn)	((__btn) & (pad_held))
#define JOY_RELEASED(__btn)	((__btn) & (pad_released))

extern char pad_state;
extern char pad_state_prev;

extern char pad_pressed;
extern char pad_held;
extern char pad_relesed;

void poll_input();
void reset_input();

#endif