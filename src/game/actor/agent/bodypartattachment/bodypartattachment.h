#ifndef BODYPARTATTACHMENT_H
#define BODYPARTATTACHMENT_H

#include "engine/graphics/bodydrawobject.h"

class Scene;
class Light;
class AnimationDrawObject;

class BodyPartAttachment : public BodyPartDrawObject::Attachment {

public:

	virtual void onBodyPartStep(const BodyPartDrawObject& bodypart, float dt) = 0;

	void addToScene(Scene* scene);

	void removeFromScene(Scene* scene);

protected:

	Scene* getScene();

	virtual void onAddToScene() {};

	virtual void onRemoveFromScene() {};

private:

	Scene* scene_;

};


class FlameAttachment : public BodyPartAttachment {

public:

	FlameAttachment();

	~FlameAttachment();

	void onBodyPartStep(const BodyPartDrawObject& bodypart, float dt);

protected:

	void onAddToScene();

	void onRemoveFromScene();

private:

	Light* light_;

	AnimationDrawObject* draw_object_;

};

#endif