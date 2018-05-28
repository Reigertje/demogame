#include "actor.h"

#include <cmath>
#include <algorithm>

#include "game/scene/scene.h"
#include "engine/graphics/drawobject.h"
#include "engine/physics/physicsshape.h"
#include "game/interaction/interaction.h"

long Actor::next_id = 1;

long Actor::getNextId() {
	return next_id++;
}

Actor::Actor(Type type) {
	id_ = getNextId();
	setPhysicsShape(0);
	type_ = type;
}

Actor::~Actor() {
	for (int i = 0; i < draw_objects_.size(); ++i) delete draw_objects_[i];
	for (int i = 0; i < hit_shapes_.size(); ++i) delete hit_shapes_[i];
	for (int i = 0; i < interaction_shapes_.size(); ++i) delete interaction_shapes_[i];
	for (int i = 0; i < interactions_.size(); ++i) delete interactions_[i];

	if (physics_shape_) delete physics_shape_;
}

long Actor::getId() const {
	return id_;
}

Actor::Type Actor::getType() const {
	return type_;
}

bool Actor::isAgent() const {
	return type_ == AGENT;
}

Agent* Actor::asAgent() {
	return isAgent() ? (Agent*)this : 0;
}

Scene* Actor::getScene() {
	return scene_;
}

bool Actor::hitsInteractionShape(const Shape& shape) const {
	for (int i = 0; i < interaction_shapes_.size(); ++i) {
		if (interaction_shapes_[i]->collides(shape)) return true;
	}
	return false; 
}

bool Actor::hitsHitShape(const Shape& shape) const {
	for (int i = 0; i < hit_shapes_.size(); ++i) {
		if (hit_shapes_[i]->collides(shape)) return true;
	}
	return false; 
}

void Actor::addDrawObject(DrawObject* draw_object, const vec2f& offset) {
	draw_objects_.push_back(draw_object);
	draw_object_offsets_.push_back(offset);
}

void Actor::addHitShape(Shape* hit_shape, const vec2f& offset) {
	hit_shapes_.push_back(hit_shape);
	hit_shape_offsets_.push_back(offset);
}

void Actor::addInteractionShape(Shape* interaction_shape, const vec2f& offset) {
	interaction_shapes_.push_back(interaction_shape);
	interaction_shape_offsets_.push_back(offset);
}

void Actor::setPhysicsShape(PhysicsShape* physics_shape) {
	physics_shape_ = physics_shape;
}

void Actor::setPosition(const vec2f& position) {
	if (physics_shape_) {
		physics_shape_->setPosition(position);
	} else {
		position_ = position;
	}
}

void Actor::setDrawObjectOffset(int index, const vec2f& offset) {
	draw_object_offsets_[index] = offset;
}

void Actor::setHitShapeOffset(int index, const vec2f& offset) {
	hit_shape_offsets_[index] = offset;
}

void Actor::setInteractionShapeOffset(int index, const vec2f& offset) {
	interaction_shape_offsets_[index] = offset;
}

void Actor::addInteraction(Interaction* interaction) {
	interactions_.push_back(interaction);
}

void Actor::removeInteraction(Interaction* interaction) {
	interactions_.erase(std::remove(interactions_.begin(), interactions_.end(), interaction), interactions_.end());
	// TODO wil ik dit wel?
	delete interaction;
}

DrawObject* Actor::getDrawObject(int index) {
	return draw_objects_[index];
}

Shape* Actor::getHitShape(int index) {
	return hit_shapes_[index];
}

Shape* Actor::getInteractionShape(int index) {
	return interaction_shapes_[index];
}

PhysicsShape* Actor::getPhysicsShape() {
	return physics_shape_;
}

vec2f Actor::getPosition() const {
	return physics_shape_ ? physics_shape_->getPosition() : position_;
}

void Actor::hit(const AttackHit& attack_hit) {
	attack_hits_.push_back(attack_hit);
}

const std::vector<AttackHit>& Actor::getAttackHits() const {
	return attack_hits_;
}

const std::vector<Interaction*>& Actor::getInteractions() {
	return interactions_;
}

void Actor::addToScene(Scene* scene) {
	scene_ = scene;
	for (int i = 0; i < draw_objects_.size(); ++i) scene->addDrawObject(draw_objects_[i]);
	if (physics_shape_) scene->addPhysicsShape(physics_shape_);
	onAddToScene();
}

void Actor::removeFromScene(Scene* scene) {
	onRemoveFromScene();
	for (int i = 0; i < draw_objects_.size(); ++i) scene->removeDrawObject(draw_objects_[i]);
	if (physics_shape_) scene->removePhysicsShape(physics_shape_);
	scene_ = NULL;
	// Destroy when?
}

void Actor::step(float dt) {
	onStep(dt);
	for (int i = 0; i < draw_objects_.size(); ++i) {
		draw_objects_[i]->setPosition(getPosition().x + draw_object_offsets_[i].x, getPosition().y + draw_object_offsets_[i].y);	
		draw_objects_[i]->step(dt);
	}
	for (int i = 0; i < hit_shapes_.size(); ++i) hit_shapes_[i]->setPosition(getPosition() + hit_shape_offsets_[i]);
	for (int i = 0; i < interaction_shapes_.size(); ++i) interaction_shapes_[i]->setPosition(getPosition() + interaction_shape_offsets_[i]);
	attack_hits_.clear();
}
