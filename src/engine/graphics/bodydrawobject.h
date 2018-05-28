#ifndef BODYDRAWOBJECT_H
#define BODYDRAWOBJECT_H

#include "drawobject.h"
#include "frameanimation.h"
#include <map>
#include <vector>

class Shader;
class GraphicsWorld;
class SpriteDrawObject;

class BodyDrawObject;

class BodyPartDrawObject : public DrawObject {
friend class BodyDrawObject;

public:

	class Frame {
		
		public:

			Frame(DrawObject* draw_object, const glm::vec2& position) : 
				draw_object_(draw_object), position_(position) {};

			~Frame() { delete draw_object_; };

			DrawObject* getDrawObject() { return draw_object_; };

			const DrawObject* getDrawObject() const { return draw_object_; };

			glm::vec2 getPosition() const { return position_; }

		private:

			DrawObject* draw_object_;

			glm::vec2 position_;

	};

	class Attachment {
		public:
			virtual void onBodyPartStep(const BodyPartDrawObject& bodypart, float dt) = 0;
	};

	BodyPartDrawObject(const std::map<std::string, std::vector<Frame*>>& animation_map);

	~BodyPartDrawObject();

	// Override layer
	GLfloat getDrawLayer() const;

	Frame* getActiveFrameObject();

	const Frame* getActiveFrameObject() const;

	glm::vec2 getFramePosition() const;

	void step(float dt);

	void draw(const GraphicsWorld& world, const glm::mat4& projection, Shader& shader);

	std::string getActiveAnimation() const;

	void setActiveAnimation(const std::string& animation);

	int getActiveFrame() const;

	void setActiveFrame(int frame);

	void putAttachment(Attachment* attachment);

	std::vector<Attachment*> getAttachments();
	
private:

	std::map<std::string, std::vector<Frame*>> animation_map_;	

	std::vector<Attachment*> attachments_;

	std::string active_animation_;

	int active_frame_;

};

class BodyDrawObject : public DrawObject {

public:

	BodyDrawObject();

	~BodyDrawObject();

	void addBodyPart(BodyPartDrawObject* bodypart);

	void removeBodyPart(BodyPartDrawObject* bodypart);

	void setAnimation(const std::string& animation);

	void transitionAnimation(const std::string& animation);

	void setCurrentAnimationDuration(float duration);

	void setCurrentAnimationSpeed(float speed);

	void draw(const GraphicsWorld& world, const glm::mat4& projection, Shader& shader);

	void addLink(const std::string& a, const std::string& b);

	void addAlias(const std::string& animation, const std::string& alias, int offset);

	void addFrameAnimation(const std::string& key, const FrameAnimation& frame_animation);

	int getFrameCount(const std::string& animation) const;

	std::vector<BodyPartDrawObject::Attachment*> getAllAttachments();

	void step(float dt);

private:

	struct Link {
		std::string a_;
		std::string b_;
		Link(const std::string& a, const std::string& b) : a_(a), b_(b) {};
	};

	struct Alias {
		std::string alias_;
		std::string animation_;
		int offset_;
		Alias(const std::string& animation, const std::string& alias, int offset) : alias_(alias), animation_(animation), offset_(offset) {};
	};

	std::map<std::string, FrameAnimation> animations_;

	std::vector<BodyPartDrawObject*> bodyparts_;	

	std::vector<Link> links_;

	std::vector<Alias> aliases_;

	std::string active_animation_;

	int active_frame_;

	FrameAnimation* active_frame_animation_;

	void handleAnimationChange();

	void updateBodyParts(float dt);

	bool areLinked(const std::string& a, const std::string& b) const;

	const Alias* findAlias(const std::string& animation) const;

};

#endif