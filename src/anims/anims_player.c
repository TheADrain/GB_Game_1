#include "anims_player.h"
#include "../sprite_manager.h"

const struct ANIM_FRAME anim_player_idle_frame_0 = 
{
	/* frame 1 */
	4U, /* num tiles */
	128U, /* duration in ACTOR_ANIM_TIMESTEP (5 frames) */
	{
		/*            Y,  X,  TILE_ID, PROPS */
		/* tile 1 */{ 0, -4, 0U, FR_FLIP_NONE },
		/* tile 2 */{ 8, -4, 16U, FR_FLIP_NONE },
		/* tile 3 */{ 0, 4, 1U, FR_FLIP_NONE },
		/* tile 4 */{ 8, 4, 17U, FR_FLIP_NONE }
	}
};

const struct ANIM_FRAME anim_player_idle_frame_1 =
{
	4U, /* num tiles */
	46U, /* duration in ACTOR_ANIM_TIMESTEP (5 frames) */
	{
		/*            Y,  X,  TILE_ID, PROPS */
		/* tile 1 */{ 0, -4, 0U, FR_FLIP_NONE },
		/* tile 2 */{ 8, -4, 32U, FR_FLIP_NONE },
		/* tile 3 */{ 0, 4, 1U, FR_FLIP_NONE },
		/* tile 4 */{ 8, 4, 17U, FR_FLIP_NONE }
	}
};

const struct ANIM_FRAME anim_player_idle_frame_2 =
{
	4U, /* num tiles */
	46U,/* duration in ACTOR_ANIM_TIMESTEP (5 frames) */
	{
		/*            Y,  X,  TILE_ID, PROPS */
		/* tile 1 */{ 0, -4, 0U, FR_FLIP_NONE },
		/* tile 2 */{ 8, -4, 33U, FR_FLIP_NONE },
		/* tile 3 */{ 0, 4, 1U, FR_FLIP_NONE },
		/* tile 4 */{ 8, 4, 17U, FR_FLIP_NONE }
	}
};

const struct ANIMATION anim_player_idle =
{
	/* NumFrames */
	PLAYER_IDLE_FRAMES,
	{
		{&anim_player_idle_frame_0},
		{&anim_player_idle_frame_1},
		{&anim_player_idle_frame_2}
	}
};