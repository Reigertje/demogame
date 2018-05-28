#ifndef SPRITEACTOR_H
#define SPRITEACTOR_H

#include "actor.h"

class Sprite;

class SpriteActor : public Actor {

public:

	SpriteActor(const std::string& key);

	SpriteActor(const Sprite& sprite);

	void addPhysicsShape(int type, float gravity = 1.0f);

	void setDrawLayer(float layer);

	void setDepth(float depth);

	void setParallax(float x, float y, float offset_x = 0, float offset_y = 0);

	void setMirrorHorizontal(bool mirror_horizontal);

	void setDrawBufferIndex(int drawbufferindex);

	void onStep(float dt);

private:

	int width_, height_;

};

#endif