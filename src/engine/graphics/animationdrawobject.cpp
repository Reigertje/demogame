#include "animationdrawobject.h"

AnimationDrawObject::AnimationDrawObject(const std::vector<DrawObject *>& children, bool repeat) : children_(children), animation_(children.size(), repeat) {

}

DrawObject* AnimationDrawObject::getActiveDrawObject() {
	return children_[animation_.getCurrentFrame()];
}

void AnimationDrawObject::draw(const GraphicsWorld& world, const glm::mat4& projection, Shader& shader) {

	DrawObject* active_drawobject = getActiveDrawObject();
	if (active_drawobject) {
		active_drawobject->setMultiplyColor(getMultiplyColor());
		active_drawobject->setMixColor(getMixColor());
		active_drawobject->setMirrorHorizontal(isMirrorHorizontal());
		active_drawobject->setPosition(getPosition());
		active_drawobject->draw(world, projection, shader);
	}
}

void AnimationDrawObject::step(float dt) {
	animation_.step(dt);
}

bool AnimationDrawObject::hasAnimationFinished() const {
	return animation_.hasFinished();
}

