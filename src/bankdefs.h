#ifndef BANKDEFS
#define BANKDEFS
/* -----------------------ROM BANKING DEFINES------------------------ */
/* bank 1 and 2 are handled in make.bat and contain GBT player and music data */
#include "bank_3.h"
#include "bank_4.h"
#include "bank_5.h"
#include "bank_6.h"
#include "bank_7.h"

/* yeah this is wasteful but it keeps things organized */
#define BANK_GBT 1U
#define BANK_MUSIC 2U

#define BANK_TITLE 3U			/* all the intro/title/menu graphics */

#define BANK_GRAPHICS_DATA_1 4U	/* all the gameplay graphics */
#define BANK_GRAPHICS_DATA_2 5U	/* reserved incase we need a graphics 2 bank */

#define BANK_MAPDATA_01 6U		/* Level data blob 1*/
#define BANK_MAPDATA_02 7U		/* Level data blob 2*/

/* ...etc */

#endif