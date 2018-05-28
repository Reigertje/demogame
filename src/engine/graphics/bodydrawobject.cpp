#include "bodydrawobject.h"

#include "graphicsworld.h"
#include "shader.h"

#include "frameanimation.h"
#include "spritedrawobject.h"

#include <iostream>
#include <algorithm>

BodyPartDrawObject::BodyPartDrawObject(const std::map<std::string, std::vector<BodyPartDrawObject::Frame*>>& animation_map) {
	animation_map_ = animation_map;
} 

BodyPartDrawObject::~BodyPartDrawObject() {
	for (std::map<std::string, std::vector<Frame*>>::iterator it = animation_map_.begin(); it != animation_map_.end(); ++it) {
		for (int i = 0; i < it->second.size(); ++i) {
			delete it->second[i];
		}
	}
}

GLfloat BodyPartDrawObject::getDrawLayer() const {
	const Frame* active_frameobject = getActiveFrameObject();
	if (active_frameobject) {
		const DrawObject* active_drawobject = active_frameobject->getDrawObject();
		if (active_drawobject) {
			return active_drawobject->getDrawLayer();
		}
	}
	return DrawObject::getDrawLayer();
}

BodyPartDrawObject::Frame* BodyPartDrawObject::getActiveFrameObject() {
	std::map<std::string, std::vector<Frame*>>::iterator it = animation_map_.find(active_animation_);
	// TODO check if frame exists
	if (it != animation_map_.end()) {
		return it->second[active_frame_];
	}
	return NULL;
}

const BodyPartDrawObject::Frame* BodyPartDrawObject::getActiveFrameObject() const {
	std::map<std::string, std::vector<Frame*>>::const_iterator it = animation_map_.find(active_animation_);
	// TODO check if frame exists
	if (it != animation_map_.end()) {
		return it->second[active_frame_];
	}
	return NULL;
}

std::string BodyPartDrawObject::getActiveAnimation() const {
	return active_animation_;
}

void BodyPartDrawObject::setActiveAnimation(const std::string& animation) {
	active_animation_ = animation;
}

glm::vec2 BodyPartDrawObject::getFramePosition() const {
	const Frame* active_frameobject = getActiveFrameObject();
	if (active_frameobject) {
		glm::vec2 pos = active_frameobject->getPosition();
		return getPosition() + (isMirrorHorizontal() ? glm::vec2(-pos.x, pos.y) : pos);
	}
	return glm::vec2(0.0f, 0.0f);	
}

int BodyPartDrawObject::getActiveFrame() const {
	return active_frame_;
}

void BodyPartDrawObject::setActiveFrame(int frame) {
	active_frame_ = frame;
}

void BodyPartDrawObject::putAttachment(Attachment* attachment) {
	attachments_.push_back(attachment);
}

std::vector<BodyPartDrawObject::Attachment*> BodyPartDrawObject::getAttachments() {
	return attachments_;
}

void BodyPartDrawObject::step(float dt) {
	Frame* active_frameobject = getActiveFrameObject();
	if (active_frameobject) {
		DrawObject* active_drawobject = active_frameobject->getDrawObject();
		if (active_drawobject) {
			active_drawobject->setMultiplyColor(getMultiplyColor());
			active_drawobject->setMixColor(getMixColor());
			active_drawobject->setMirrorHorizontal(isMirrorHorizontal());
			glm::vec2 pos = active_frameobject->getPosition();
			active_drawobject->setPosition(getPosition() + (isMirrorHorizontal() ? glm::vec2(-pos.x, pos.y) : pos));
		}
	}

	for (int i = 0; i < attachments_.size(); ++i) {
		attachments_[i]->onBodyPartStep(*this, dt);
	}

}

void BodyPartDrawObject::draw(const GraphicsWorld& world, const glm::mat4& projection, Shader& shader) {
	Frame* active_frameobject = getActiveFrameObject();
	if (active_frameobject) {
		DrawObject* active_drawobject = active_frameobject->getDrawObject();
		if (active_drawobject) {
			active_drawobject->draw(world, projection, shader);
		}
	}
}

//
//
//

BodyDrawObject::BodyDrawObject() {
	active_frame_animation_ = NULL;
	active_frame_ = 0;
}

BodyDrawObject::~BodyDrawObject() {
	// TODO hier ligt eigenlijk de verantwoordelijkheid niet.
	for (int i = 0; i < bodyparts_.size(); ++i) {
		delete bodyparts_[i];
	}
}

void BodyDrawObject::addFrameAnimation(const std::string& key, const FrameAnimation& frame_animation) {
	animations_.insert(std::pair<std::string, FrameAnimation>(key, frame_animation));
}

void BodyDrawObject::addBodyPart(BodyPartDrawObject* bodypart) {
	bodyparts_.push_back(bodypart);
}

void BodyDrawObject::removeBodyPart(BodyPartDrawObject* bodypart) {
	bodyparts_.erase(std::remove(bodyparts_.begin(), bodyparts_.end(), bodypart), bodyparts_.end());
}

void BodyDrawObject::addLink(const std::string& a, const std::string& b) {
    links_.push_back(Link(a, b));
}

void BodyDrawObject::addAlias(const std::string& animation, const std::string& alias, int offset) {
    aliases_.push_back(Alias(animation, alias, offset));
}

void BodyDrawObject::setAnimation(const std::string& animation) {
	if (active_animation_ == animation) return;
	active_animation_ = animation;
	handleAnimationChange();
}

void BodyDrawObject::transitionAnimation(const std::string& animation) {

    FrameAnimation* prev_frame_animation = active_frame_animation_;
    std::string prev_animation = active_animation_;

    setAnimation(animation);

    if (prev_frame_animation && areLinked(animation, prev_animation)) {
        active_frame_animation_->transition(*prev_frame_animation);
    }
}

void BodyDrawObject::setCurrentAnimationDuration(float duration) {
	if (active_frame_animation_) {
		active_frame_animation_->setSpeed((active_frame_animation_->getDurationWithoutSpeed() / 1000.f) / duration);
	}
}

void BodyDrawObject::setCurrentAnimationSpeed(float speed) {
	if (active_frame_animation_) {
		active_frame_animation_->setSpeed(speed);
	}
}

bool BodyDrawObject::areLinked(const std::string& a, const std::string& b) const {
    const Alias* alias_a = findAlias(a);
    const Alias* alias_b = findAlias(b);

    std::string fixed_a = alias_a ? alias_a->animation_ : a;
    std::string fixed_b = alias_b ? alias_b->animation_ : b;

    for (int i = 0; i < links_.size(); ++i) {
        if ((links_[i].a_ == fixed_a && links_[i].b_ == fixed_b) || (links_[i].a_ == fixed_b && links_[i].b_ == fixed_a)) {
            return true;
        }
    }    

    return false;
}

const BodyDrawObject::Alias* BodyDrawObject::findAlias(const std::string& animation) const {
    for (int i = 0; i < aliases_.size(); ++i) {
        if (aliases_[i].alias_ == animation) return &aliases_[i];
    }
    return NULL;
}

void BodyDrawObject::handleAnimationChange() {
	std::map<std::string, FrameAnimation>::iterator it = animations_.find(active_animation_);
	if (it != animations_.end()) {
		active_frame_animation_ = &(it->second);
		active_frame_animation_->reset();
	} else {
		active_frame_animation_ = NULL;
	}
}

void BodyDrawObject::step(float dt) {
	if (active_frame_animation_) {
		active_frame_animation_->step(dt);
		active_frame_ = active_frame_animation_->getCurrentFrame();
		updateBodyParts(dt);
	}
}

std::vector<BodyPartDrawObject::Attachment*> BodyDrawObject::getAllAttachments() {
	std::vector<BodyPartDrawObject::Attachment*> result;
	for (int i = 0; i < bodyparts_.size(); ++i) {
		std::vector<BodyPartDrawObject::Attachment*> bp_result = bodyparts_[i]->getAttachments();
		result.insert(result.end(), bp_result.begin(), bp_result.end());
	}
	return result;
}

static bool compareBodyPartLayer(const BodyPartDrawObject* a, const BodyPartDrawObject* b) {
	return a->getDrawLayer() > b->getDrawLayer(); 
}

void BodyDrawObject::updateBodyParts(float dt) {
	const Alias* alias = findAlias(active_animation_);

    for (int i = 0; i < bodyparts_.size(); ++i) {
    	bodyparts_[i]->setActiveAnimation(alias ? alias->animation_ : active_animation_);
		bodyparts_[i]->setActiveFrame(alias ? active_frame_ + alias->offset_ : active_frame_);
		bodyparts_[i]->setMirrorHorizontal(isMirrorHorizontal());
		bodyparts_[i]->setMixColor(getMixColor());
		bodyparts_[i]->setPosition(getPosition());
		bodyparts_[i]->step(dt);
    }

    std::sort(bodyparts_.begin(), bodyparts_.end(), compareBodyPartLayer);
}

void BodyDrawObject::draw(const GraphicsWorld& world, const glm::mat4& projection, Shader& shader) {
	for (int i = 0; i < bodyparts_.size(); ++i) {
		bodyparts_[i]->draw(world, projection, shader);
	}
}

int BodyDrawObject::getFrameCount(const std::string& animation) const {
	std::map<std::string, FrameAnimation>::const_iterator it = animations_.find(animation);
	if (it != animations_.end()) {
		return it->second.getFrameCount();	
	}
	return -1;
}