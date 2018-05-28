#include "spriteactor.h"

#include "engine/graphics/spritedrawobject.h"
#include "engine/physics/physicsshape.h"

#include "engine/graphics/sprite.h"

#include "game/game.h"

#include "game/scene/scene.h"

#include <iostream>

SpriteActor::SpriteActor(const Sprite& sprite) {
	addDrawObject(new SpriteDrawObject(sprite));

	width_ = sprite.getPixelWidth();
	height_ = sprite.getPixelHeight();
}

SpriteActor::SpriteActor(const std::string& key) {
	Sprite t = Game::getInstance().getResources().getSprite(key);
	addDrawObject(new SpriteDrawObject(t));

	width_ = t.getPixelWidth();
	height_ = t.getPixelHeight();
}

void SpriteActor::setDrawLayer(float layer) {
	getDrawObject(0)->setDrawLayer(layer);
}

void SpriteActor::setDepth(float depth) {
	getDrawObject(0)->setDepth(depth);
}

void SpriteActor::setParallax(float x, float y, float offset_x, float offset_y) {
	getDrawObject(0)->setCameraFactor(x, y);
	getDrawObject(0)->setCameraOffset(offset_x, offset_y);
}

void SpriteActor::setMirrorHorizontal(bool mirror_horizontal) {
	getDrawObject(0)->setMirrorHorizontal(mirror_horizontal);
}

void SpriteActor::setDrawBufferIndex(int index) {
	getDrawObject(0)->setDrawBufferIndex(index);
}

void SpriteActor::addPhysicsShape(int type, float gravity_factor) {
	PhysicsShape* shape = new PhysicsShape(type, width_/2, height_/2);
	shape->setGravityFactor(gravity_factor);
	Actor::setPhysicsShape(shape);
}

void SpriteActor::onStep(float dt) {

}