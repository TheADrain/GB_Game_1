#include "anims_forest.h"
#include "../sprite_manager.h"

const struct ANIM_FRAME frame_crow_perched_1 = 
{
	/* perched frame */
	3U,
	128U,
	{
		/*            Y,  X,  TILE_ID, PROPS */
		/* tile 1 */{ -8, 0, 2U, FR_FLIP_NONE },
		/* tile 2 */{ 0, 0, 18U, FR_FLIP_NONE },
		/* tile 3 */{ 0, 8, 19U, FR_FLIP_NONE }
	}
};

const struct ANIM_FRAME frame_crow_perched_2 = 
{
	/* perched frame */
	3U,
	16U,
	{
		/*            Y,  X,  TILE_ID, PROPS */
		/* tile 1 */{ -8, 0, 34U, FR_FLIP_NONE },
		/* tile 2 */{ 0, 0, 50U, FR_FLIP_NONE },
		/* tile 3 */{ 0, 8, 51U, FR_FLIP_NONE }
	}
};

const struct ANIM_FRAME frame_crow_fly = 
{
	/* fly frame */
	4U,
	16U,
	{
		/* tile 1 */{ -8, 0, 0U, FR_FLIP_NONE },
		/* tile 2 */{ -8, 8, 1U, FR_FLIP_NONE },
		/* tile 3 */{ 0, 0, 16U, FR_FLIP_NONE },
		/* tile 4 */{ 0, 8, 17U, FR_FLIP_NONE }
	}
};

const struct ANIM_FRAME frame_crow_swoop = 
{
	/* swoop attack frame */
	2U,
	16U,
	{
		/* tile 1 */{ 0, 0, 3U, FR_FLIP_NONE },
		/* tile 2 */{ 0, 8, 4U, FR_FLIP_NONE }
	}
};

const struct ANIM_FRAME *const frames_crow[CROW_FRAMES] = 
{
	{ &frame_crow_perched_1 },
	{ &frame_crow_perched_2 },
	{ &frame_crow_fly },
	{ &frame_crow_swoop }
};

const struct ANIM_FRAME *const frames_crow_idle[2U] = 
{
	{ &frame_crow_perched_1 },
	{ &frame_crow_perched_2 }
};

const struct ANIM_FRAME skelly_stand = 
{
	/* standing frame */
		5U,
		16U,
		{
			/* tile 1 */{ 4, 4, 32U, FR_FLIP_NONE },
			/* tile 2 */{ 0, 0, 33U, FR_FLIP_X },
			/* tile 3 */{ 8, 0, 33U, FR_FLIP_NONE },
			/* tile 4 */{ 0, 8, 49U, FR_FLIP_X },
			/* tile 5 */{ 8, 8, 49U, FR_FLIP_NONE }
		}
};

const struct ANIM_FRAME skelly_walk_1 = 
{
	/* walk frame 1 */
	5U,
	16U,
	{
		/* tile 1 */{ 4, 4, 32U, FR_FLIP_NONE },
		/* tile 2 */{ 0, 0, 33U, FR_FLIP_X },
		/* tile 3 */{ 8, 0, 33U, FR_FLIP_NONE },
		/* tile 4 */{ 0, 8, 50U, FR_FLIP_X },
		/* tile 5 */{ 8, 8, 49U, FR_FLIP_NONE }
	}
};

const struct ANIM_FRAME skelly_walk_2 = 
{
	/* walk frame 2 */
	5U,
	16U,
	{
		/* tile 1 */{ 4, 4, 32U, FR_FLIP_NONE },
		/* tile 2 */{ 0, 0, 33U, FR_FLIP_X },
		/* tile 3 */{ 8, 0, 33U, FR_FLIP_NONE },
		/* tile 4 */{ 0, 8, 49U, FR_FLIP_X },
		/* tile 5 */{ 8, 8, 50U, FR_FLIP_NONE }
	}
};

const struct ANIM_FRAME *const frames_skelly[SKELLY_FRAMES] =
{
	{ &skelly_stand	},
	{ &skelly_walk_1 },
	{ &skelly_walk_2 }
};