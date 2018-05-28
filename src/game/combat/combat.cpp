#include "combat.h"

#include "game/scene/scene.h"
#include "game/actor/agent/agent.h"
#include "engine/geom/rectangle.h"

#include <iostream>

static float max(float a, float b) {
	return a > b ? a : b;
}

static float min(float a, float b) {
	return a < b ? a : b;
}

CombatProperties::CombatProperties() {
	for (int i = 0; i < TYPE_COUNT; ++i) values_[i] = 0.0f;
}

void CombatProperties::set(int type, float value) {
	values_[type] = value;
}

float CombatProperties::get(int type) const {
	return values_[type];
}

float CombatProperties::getTotal() const {
	float sum = 0.0f;
	for (int i = 0; i < TYPE_COUNT; ++i) sum += max(0, values_[i]);
	return sum;
}

CombatProperties CombatProperties::operator+(const CombatProperties& other) {
	CombatProperties result;
	for (int i = 0; i < TYPE_COUNT; ++i) result.set(i, max(0, get(i) + other.get(i)));
	return result;
}

CombatProperties CombatProperties::operator-(const CombatProperties& other) {
	CombatProperties result;
	for (int i = 0; i < TYPE_COUNT; ++i) result.set(i, max(0, get(i) - other.get(i)));
	return result;
}

CombatProperties CombatProperties::operator*(const CombatProperties& other) {
	CombatProperties result;
	for (int i = 0; i < TYPE_COUNT; ++i) result.set(i, max(0, get(i) * other.get(i)));
	return result;
}

CombatProperties AttackProperties::getDamageProperties() const {
	return damage_properties_;
}

int AttackProperties::getAttackDirection() const {
	return attack_direction_;
}

void AttackProperties::attack(int attack_direction) {
	attack_direction_ = attack_direction;
}

CombatProperties DefenceProperties::getBodyProperties() const {
	return body_properties_;
}

CombatProperties DefenceProperties::getBlockProperties() const {
	return block_properties_;
}

int DefenceProperties::getBlockDirections() const {
	return block_directions_;
}

void DefenceProperties::block(const CombatProperties& block_properties, int block_directions) {
	block_directions_ = block_directions;
	block_properties_ = block_properties;
}

long AttackHit::getActorId() const {
	return actor_id_;
}

AttackProperties AttackHit::getAttackProperties() const {
	return attack_properties_;
}


Attack::Attack() {
	shape_ = NULL;
/*	properties_ = properties;
	shape_ = shape.clone();*/
}

Attack::~Attack() {
	delete shape_;
}

void Attack::setAttackProperties(const AttackProperties& properties) {
	properties_ = properties;
}

void Attack::setShape(const Shape& shape) {
	if (shape_) delete shape_;
	shape_ = shape.clone();
}

void Attack::updateShapePosition(const vec2f& position) {
	if (shape_) shape_->setPosition(position);
}

bool Attack::filter(const Actor& actor) {
	return (isInFilter(actor) || (actor.isAgent() && isGroupInFilter(((const Agent&)actor).getGroup())));
}

bool Attack::isInFilter(const Actor& actor) {
	for (long id : actor_filter_) {
		if (id == actor.getId()) return true;
	}
	return false;
}

bool Attack::isGroupInFilter(int group) {
	for (int filter_group : group_filter_) {
		if (filter_group == group) return true;
	}
	return false;
}

void Attack::addToFilter(const Actor& actor) {
	actor_filter_.push_back(actor.getId());
}

void Attack::addGroupToFilter(int group) {
	group_filter_.push_back(group);
}

void Attack::clearFilter() {
	actor_filter_.clear();
}

void Attack::performAttack(int direction, Agent& agent) {
	if (!shape_) return;

	properties_.attack(direction);

	//agent.getScene()->debugShape(*shape_);
	std::vector<Actor* > hits = agent.getScene()->getHits(*shape_);
	for (int i = 0; i < hits.size(); ++i) {
		if (hits[i] != &agent && !filter(*hits[i])) {
			hits[i]->hit(AttackHit(0, properties_));
			addToFilter(*hits[i]);
		}
	}
}


float Attack::calculateDamage(const AttackProperties& attack, const DefenceProperties& defence) {
	CombatProperties damage = attack.getDamageProperties() - defence.getBodyProperties();

	if (defence.getBlockDirections() & attack.getAttackDirection()) {
		damage = damage - defence.getBlockProperties();
		return damage.getTotal();
	}
	return max(1, damage.getTotal());
}