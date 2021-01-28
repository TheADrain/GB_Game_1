#ifndef UTILS_1_H
#define UTILS_1_H

#include <gb/gb.h>

/** Set the Tile Data Table start address to $9000 mode */
#define TDT_MODE_0 LCDC_REG&=0xEFU

/** Set the Tile Data Table start address to $8000 mode */
#define TDT_MODE_1 LCDC_REG|=0x10U

extern UINT8 fade_i;
extern UINT8 fade_j;

void SetDefaultPalette_BGK();
void FadeOutNintendoLogo(); 

void SetPaletteAllBlack();
void SetPaletteAllWhite();

void FadeToBlack(UINT8 waitFrames);
void FadeFromBlack(UINT8 waitFrames);

void FadeToWhite(UINT8 waitFrames);
void FadeFromWhite(UINT8 waitFrames);

#endif