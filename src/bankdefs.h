#ifndef BANKDEFS
#define BANKDEFS
/* -----------------------ROM BANKING DEFINES------------------------ */
#include "bank_1.h"
#include "bank_2.h"
#include "bank_3.h"
#include "bank_4.h"

/* yeah this is wasteful but it keeps things organized */
#define BANK_TITLE 1U			/* all the intro/title/menu graphics */
#define BANK_GRAPHICS_DATA 2U	/* all the gameplay graphics */
#define BANK_MAPDATA_01 3U		/* Level data blob 1*/
#define BANK_MAPDATA_02 4U		/* Level data blob 2*/
/* ...etc */

#endif