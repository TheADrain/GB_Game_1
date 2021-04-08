#ifndef SKELTON_H
#define SKELTON_H

#include <gb/gb.h>
#include "../main.h"
#include "../actors.h"
#include "../sprite_manager.h"

#define SKELTON_STATE_WALK_LEFT 0U
#define SKELTON_STATE_WALK_RIGHT 1U
#define SKELTON_STATE_DIE 2U
#define SKELTON_STATE_DEAD 3U

void Initialize_ActorSkelton(struct ACTOR* a);
void Update_ActorSkelton(struct ACTOR* a);


#endif