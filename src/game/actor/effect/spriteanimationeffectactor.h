#ifndef SPRITEANIMATIONEFFECTACTOR_H
#define SPRITEANIMATIONEFFECTACTOR_H

#include "engine/graphics/animationdrawobject.h"

#include "game/actor/actor.h"

class SpriteAnimationEffectActor : public Actor {

public:


	SpriteAnimationEffectActor(const std::vector<std::string>& sprite_keys);

	void onStep(float dt);

private:


	AnimationDrawObject* animation_draw_object_;

};

#endif