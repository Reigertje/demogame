#include "agent.h"

#include "engine/physics/physicsshape.h"
#include "game/scene/scene.h"
#include "game/actor/effect/spriteanimationeffectactor.h"
#include "engine/graphics/texture.h"
#include "engine/graphics/bodydrawobject.h"
#include "game/combat/combat.h"
#include "./bodypartattachment/bodypartattachment.h"

#include "game/game.h"

#include <iostream>

Agent::Agent(Group group) : Actor(AGENT) {
	setOrientation(Agent::RIGHT);
	setSpeed(100.0f);
	setJump(200.0f);
	setGroup(group);
	moving_ = false;
	state_ = 0;
	body_draw_object_ = 0;
}

Agent::Agent(const AgentAttributes& base_attributes, Group group) : Actor(AGENT), base_attributes_(base_attributes) {
	setGroup(group);
	setOrientation(Agent::RIGHT);
	setSpeed(100.0f);
	setJump(200.0f);
	moving_ = false;
	state_ = 0;
	body_draw_object_ = 0;
}

Agent::~Agent() {
	
}

void Agent::onStep(float dt) {
	if (body_draw_object_) {
		body_draw_object_->setMirrorHorizontal(orientation_);
		glm::vec4 color = body_draw_object_->getMixColor();
		if (color.a > 0) {
			body_draw_object_->setMixColor(glm::vec4(color.r, color.g, color.b, color.a - 2.0f * dt >= 0.0f ? color.a - 2.0f *dt : 0.f));
		}
	}

	for (const AttackHit attack_hit : getAttackHits()) {
		handleAttackHit(attack_hit);
	}

	status_.step(getAttributes(), dt);
}

void Agent::handleAttackHit(const AttackHit& hit) {
	
	float damage = Attack::calculateDamage(hit.getAttackProperties(), getDefenceProperties());

	if (damage > 0) {
		status_.setHp(status_.getHp() - damage);
		if (body_draw_object_) body_draw_object_->setMixColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	
	if (status_.getHp() <= 0) {
		std::vector<std::string> keys = {"poof1", "poof2", "poof3", "poof4"};
		SpriteAnimationEffectActor * effect = new SpriteAnimationEffectActor(keys);
		effect->setPosition(getPosition());
		getScene()->addActor(effect);
		getScene()->removeActor(this);
	}
}

void Agent::setBodyDrawObject(const std::string& key) {
	body_draw_object_ = Game::getInstance().getResources().getBodyDrawObject(key);
	addDrawObject(body_draw_object_);
}

void Agent::setGroup(Group group) {
	group_ = group;
} 

void Agent::setSpeed(float speed) {
	speed_ = speed;
}

void Agent::setJump(float jump) {
	jump_ = jump;
}

BodyDrawObject* Agent::getBodyDrawObject() {
	return body_draw_object_;
}

Agent::Group Agent::getGroup() const {
	return group_;
}

bool Agent::getOrientation() const {
	return orientation_;
}

float Agent::getSpeed() const {
	return speed_;
}

float Agent::getJump() const {
	return jump_;
}

bool Agent::isMoving() const {
	return moving_;
}

int Agent::getState() const {
	return state_;
}

DefenceProperties Agent::getDefenceProperties() const {
	return DefenceProperties();
}

AgentAttributes Agent::getAttributes() const {
	// Can be overridden to calculate sum of attributes of equipment eg
	return base_attributes_;
}

const AgentAttributes& Agent::getBaseAttributes() const {
	return base_attributes_;
}

const AgentStatus& Agent::getStatus() const {
	return status_;
}

void Agent::setBaseAttributes(const AgentAttributes& base_attributes) {
	base_attributes_ = base_attributes;
	// Base attributes veranderd, status opnieuw initialiseren
	status_.initialize(base_attributes_);
}

void Agent::setStatus(const AgentStatus& status) {
	status_ = status;
}

void Agent::moveLeft() {
	getPhysicsShape()->setHorizontalVelocity(-speed_);
	setOrientation(LEFT);
	moving_ = true;
}

void Agent::moveRight() {
	getPhysicsShape()->setHorizontalVelocity(speed_);
	setOrientation(RIGHT);
	moving_ = true;
}

void Agent::standStill() {
	getPhysicsShape()->setHorizontalVelocity(0.0f);
	moving_ = false;
}

bool Agent::canJump() {
	return getPhysicsShape()->hasBottomContact() || getPhysicsShape()->isHanging();
}

bool Agent::jump() {
	if (canJump()) {
		getPhysicsShape()->releaseHang();
		getPhysicsShape()->setVerticalVelocity(jump_);
		return true;
	}
	return false;
}

void Agent::haltJump() {
	if (getPhysicsShape()->getVerticalVelocity() > 0) {
		getPhysicsShape()->setVerticalVelocity(0);
	}
}

void Agent::setAnimation(const std::string& animation) {
	body_draw_object_->setAnimation(animation);
}

void Agent::transitionAnimation(const std::string& animation) {
	body_draw_object_->transitionAnimation(animation);
}

void Agent::addBodyPart(BodyPartDrawObject* bodypart) {
	body_draw_object_->addBodyPart(bodypart);
	std::vector<BodyPartDrawObject::Attachment*> attachments = bodypart->getAttachments();
	for (int i = 0; i < attachments.size(); ++i) ((BodyPartAttachment*)attachments[i])->addToScene(getScene());
}

void Agent::removeBodyPart(BodyPartDrawObject* bodypart) {
	body_draw_object_->removeBodyPart(bodypart);
	std::vector<BodyPartDrawObject::Attachment*> attachments = bodypart->getAttachments();
	for (int i = 0; i < attachments.size(); ++i) ((BodyPartAttachment*)attachments[i])->removeFromScene(getScene());
}

void Agent::addBodyPart(const std::string& key) {
	body_draw_object_->addBodyPart(Game::getInstance().getResources().getBodyPartDrawObject(key));
}

void Agent::setOrientation(bool orientation) {
	orientation_ = orientation;
}

void Agent::onAddToScene() {
	if (body_draw_object_) {
		std::vector<BodyPartDrawObject::Attachment*> attachments_ = body_draw_object_->getAllAttachments();
		for (int i = 0; i < attachments_.size(); ++i) {
			((BodyPartAttachment*)attachments_[i])->addToScene(getScene());
		}
	}
}

void Agent::onRemoveFromScene() {
	if (body_draw_object_) {
		std::vector<BodyPartDrawObject::Attachment*> attachments_ = body_draw_object_->getAllAttachments();
		for (int i = 0; i < attachments_.size(); ++i) {
			((BodyPartAttachment*)attachments_[i])->removeFromScene(getScene());
		}
	}
}
