#include "spriteanimationeffectactor.h"

#include "engine/graphics/spritedrawobject.h"

#include "game/scene/scene.h"
#include "game/game.h"

SpriteAnimationEffectActor::SpriteAnimationEffectActor(const std::vector<std::string>& sprite_keys) {
	std::vector<DrawObject*> frames;
	for (int i = 0; i < sprite_keys.size(); ++i) {
		frames.push_back(new SpriteDrawObject(Game::getInstance().getResources().getSprite(sprite_keys[i])));
	}
	animation_draw_object_ = new AnimationDrawObject(frames, false);
	addDrawObject(animation_draw_object_);
}

void SpriteAnimationEffectActor::onStep(float dt) {
	if (animation_draw_object_->hasAnimationFinished()) {
		getScene()->removeActor(this);
	}
}