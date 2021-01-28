#include "utils_1.h"

UINT8 fade_i = 0U;
UINT8 fade_j = 0U;

const UINT8 defaultPal_BKG = 0xE4U;

UINT8 fadeToWhitePalettes[] = {
	/* shades from darkest to lightest: 11, 10, 01, 00 */
	0xE4U, /* 11100100 */
	0x90U, /* 10010000 */
	0x40U, /* 01000000 */
	0x00U  /* 00000000 */
};

UINT8 fadeToBlackPalettes[] = {
	/* shades from darkest to lightest: 11, 10, 01, 00 */
	0xE4U, /* 11100100 */
	0xF9U, /* 11111001 */
	0xFEU, /* 11111110 */
	0xFFU  /* 11111111 */
};

void SetDefaultPalette_BGK()
{
	BGP_REG = defaultPal_BKG;
}

void FadeOutNintendoLogo()
{
	FadeToBlack(4U); // real slow for now
}

void SetPaletteAllBlack()
{
	BGP_REG = fadeToWhitePalettes[3U];
}

void SetPaletteAllWhite()
{
	BGP_REG = fadeToBlackPalettes[3U];
}

void FadeToBlack(UINT8 waitFrames) 
{
	for(fade_i = 0U; fade_i != 4U; fade_i++)
	{
		BGP_REG = fadeToBlackPalettes[fade_i];
		for(fade_j = 0U; fade_j != waitFrames; fade_j++)
		{
			/* todo: sound tick */
			wait_vbl_done();
		}
	}
}

void FadeFromBlack(UINT8 waitFrames) 
{
	for(fade_i = 3U; fade_i != 0U; fade_i--)
	{
		BGP_REG = fadeToBlackPalettes[fade_i-1];
		for(fade_j = 0U; fade_j != waitFrames; fade_j++)
		{
			/* todo: sound tick */
			wait_vbl_done();
		}
	}
}

void FadeToWhite(UINT8 waitFrames) 
{
	for(fade_i = 0U; fade_i != 4U; fade_i++)
	{
		BGP_REG = fadeToWhitePalettes[fade_i];
		for(fade_j = 0U; fade_j != waitFrames; fade_j++)
		{
			/* todo: sound tick */
			wait_vbl_done();
		}
	}
}

void FadeFromWhite(UINT8 waitFrames) 
{
	for(fade_i = 3U; fade_i != 0U; fade_i--)
	{
		BGP_REG = fadeToWhitePalettes[fade_i-1];
		for(fade_j = 0U; fade_j != waitFrames; fade_j++)
		{
			/* todo: sound tick */
			wait_vbl_done();
		}
	}
}