#include "bodypartattachment.h"

#include "engine/graphics/light.h"
#include "engine/graphics/animationdrawobject.h"
#include "engine/graphics/spritedrawobject.h"

#include "game/game.h"
#include "game/scene/scene.h"

void BodyPartAttachment::addToScene(Scene* scene) {
	scene_ = scene;
	onAddToScene();
}

void BodyPartAttachment::removeFromScene(Scene* scene) {
	onRemoveFromScene();
	scene_ = NULL;
}

Scene* BodyPartAttachment::getScene() {
	return scene_;
}

FlameAttachment::FlameAttachment() {
	light_ = new Light(150);
	light_->setDepth(-20);
	light_->setColor(glm::rgb(glm::vec3(255/2, 183/2, 45/2)));
	std::vector<DrawObject*> frames;
	frames.push_back(new SpriteDrawObject(Game::getInstance().getResources().getSprite("flame1")));
	frames.push_back(new SpriteDrawObject(Game::getInstance().getResources().getSprite("flame2")));
	frames.push_back(new SpriteDrawObject(Game::getInstance().getResources().getSprite("flame3")));
	frames.push_back(new SpriteDrawObject(Game::getInstance().getResources().getSprite("flame4")));
	draw_object_ = new AnimationDrawObject(frames);
}

FlameAttachment::~FlameAttachment() {
	delete light_;
	delete draw_object_;
}

void FlameAttachment::onBodyPartStep(const BodyPartDrawObject& bodypart, float dt) {

	const BodyPartDrawObject::Frame* frame = bodypart.getActiveFrameObject();

	if (frame) {
		light_->setPosition(bodypart.getFramePosition());
		draw_object_->setPosition(bodypart.getFramePosition());
	}

	draw_object_->step(dt);
}


void FlameAttachment::onAddToScene() {
	getScene()->addLight(light_);
	getScene()->addDrawObject(draw_object_);
}

void FlameAttachment::onRemoveFromScene() {
	getScene()->removeLight(light_);
	getScene()->removeDrawObject(draw_object_);
}
