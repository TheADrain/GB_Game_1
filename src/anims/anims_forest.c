#include "anims_forest.h"
#include "../sprite_manager.h"

const struct ANIM_FRAME frame_crow_perched = 
{
	/* perched frame */
	3U,
	{
		/* tile 1 */{ 0U, 0U, 2U, FR_FLIP_NONE },
		/* tile 2 */{ 8U, 0U, 18U, FR_FLIP_NONE },
		/* tile 3 */{ 8U, 8U, 19U, FR_FLIP_NONE }
	}
};

const struct ANIM_FRAME frame_crow_fly = 
{
	/* fly frame */
	4U,
	{
		/* tile 1 */{ 0U, 0U, 0U, FR_FLIP_NONE },
		/* tile 2 */{ 0U, 8U, 1U, FR_FLIP_NONE },
		/* tile 3 */{ 8U, 0U, 16U, FR_FLIP_NONE },
		/* tile 4 */{ 8U, 8U, 17U, FR_FLIP_NONE }
	}
};

const struct ANIM_FRAME frame_crow_swoop = 
{
	/* swoop attack frame */
	2U,
	{
		/* tile 1 */{ 0U, 0U, 3U, FR_FLIP_NONE },
		/* tile 2 */{ 0U, 8U, 4U, FR_FLIP_NONE }
	}
};

const struct ANIM_FRAME *const frames_crow[CROW_FRAMES] = 
{
	{ &frame_crow_perched },
	{ &frame_crow_fly },
	{ &frame_crow_swoop }
};

const struct ANIM_FRAME skelly_stand = 
{
	/* standing frame */
		5U,
		{
			/* tile 1 */{ 4U, 4U, 32U, FR_FLIP_NONE },
			/* tile 2 */{ 0U, 0U, 33U, FR_FLIP_X },
			/* tile 3 */{ 8U, 0U, 33U, FR_FLIP_NONE },
			/* tile 4 */{ 0U, 8U, 49U, FR_FLIP_X },
			/* tile 5 */{ 8U, 8U, 49U, FR_FLIP_NONE }
		}
};

const struct ANIM_FRAME skelly_walk_1 = 
{
	/* walk frame 1 */
	5U,
	{
		/* tile 1 */{ 4U, 4U, 32U, FR_FLIP_NONE },
		/* tile 2 */{ 0U, 0U, 33U, FR_FLIP_X },
		/* tile 3 */{ 8U, 0U, 33U, FR_FLIP_NONE },
		/* tile 4 */{ 0U, 8U, 50U, FR_FLIP_X },
		/* tile 5 */{ 8U, 8U, 49U, FR_FLIP_NONE }
	}
};

const struct ANIM_FRAME skelly_walk_2 = 
{
	/* walk frame 2 */
	5U,
	{
		/* tile 1 */{ 4U, 4U, 32U, FR_FLIP_NONE },
		/* tile 2 */{ 0U, 0U, 33U, FR_FLIP_X },
		/* tile 3 */{ 8U, 0U, 33U, FR_FLIP_NONE },
		/* tile 4 */{ 0U, 8U, 49U, FR_FLIP_X },
		/* tile 5 */{ 8U, 8U, 50U, FR_FLIP_NONE }
	}
};

const struct ANIM_FRAME *const frames_skelly[SKELLY_FRAMES] =
{
	{ &skelly_stand	},
	{ &skelly_walk_1 },
	{ &skelly_walk_2 }
};