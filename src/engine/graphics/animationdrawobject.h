#ifndef ANIMATIONDRAWOBJECT_H
#define ANIMATIONDRAWOBJECT_H

#include "drawobject.h"
#include "frameanimation.h"

class AnimationDrawObject : public DrawObject {

public:

	AnimationDrawObject(const std::vector<DrawObject *>& children, bool repeat = true);

	void draw(const GraphicsWorld& world, const glm::mat4& projection, Shader& shader);

	void step(float dt);

	bool hasAnimationFinished() const;

private:

	DrawObject* getActiveDrawObject();

	std::vector<DrawObject *> children_;

	FrameAnimation animation_;

};

#endif