#include "weapon.h"

#include "game/scene/scene.h"
#include "game/actor/agent/agent.h"
#include "engine/geom/rectangle.h"

#include <iostream>

static const std::string STATE_STRINGS[] = {
	"rest", 
	"prepare", 
	"load", 
	"charge", 
	"attack", 
	"cooldown"
};

Weapon::Timing::Timing(float prepare, float load, float charge, float attack, float cooldown) {
	states_[PREPARE] = prepare;
	states_[LOAD] = load;
	states_[CHARGE] = charge;
	states_[ATTACK] = attack;
	states_[COOLDOWN] = cooldown;
}

Weapon::Timing::Timing(float prepare, float attack, float cooldown) {
	states_[PREPARE] = prepare;
	states_[LOAD] = 0;
	states_[CHARGE] = 0;
	states_[ATTACK] = attack;
	states_[COOLDOWN] = cooldown;
}

Weapon::Weapon(const Timing& timing) : timing_(timing) {
	current_state_ = REST;	
	current_time_ = 0;
	timing_ = timing;
}

bool Weapon::isAttacking() const {
	return current_state_ != REST;
}

int Weapon::getCurrentState() const {
	return current_state_;
}

std::string Weapon::getCurrentStateString() const {
	return STATE_STRINGS[current_state_];
}

float Weapon::getCurrentStateTime() const {
	return timing_.states_[current_state_];
}

float Weapon::getStateTime(int state) const {
	return timing_.states_[state];
}

void Weapon::startAttack() {
	if (current_state_ == REST) {
		nextState();
	}
}

void Weapon::nextState() {
	changeState(current_state_, (current_state_ + 1) % 6);
}

void Weapon::changeState(int previous_state, int new_state) {
	current_state_ = new_state;
	if (current_state_ != REST && timing_.states_[current_state_] == 0) {
		// Skip state
		nextState();
	} else if (current_state_ == REST) {
		current_time_ = 0;
		onStateChanged(previous_state, new_state);
	} else {
		current_time_ += timing_.states_[current_state_];
		onStateChanged(previous_state, new_state);
		if (current_time_ <= 0) nextState();
	}
}

void Weapon::step(Agent& agent, float dt) {
	if (isAttacking()) {
		current_time_ -= dt;
		if (current_state_ == ATTACK) performAttack(agent);
		if (current_time_ <= 0) nextState();
	}
}

//
//
//


MeleeWeapon::MeleeWeapon(const Timing& timing) : Weapon(timing) {
	CombatProperties properties;


	//TODO hrad coded value to test
	properties.set(CombatProperties::PHYSICAL, 2);

	Rectangle rect(16, 16);

	attack_.setAttackProperties(AttackProperties(properties));
	attack_.setShape(rect);
}

void MeleeWeapon::onStateChanged(int previous_state, int new_state) {
	if (new_state == ATTACK) {
		attack_.clearFilter();
	}
}

void MeleeWeapon::performAttack(Agent& agent) {
	

	attack_.updateShapePosition(agent.getPosition() + vec2f(
		agent.getOrientation() == Agent::RIGHT ? 20 : -20, 
		0));

	attack_.performAttack(agent.getOrientation() == Agent::LEFT ? Attack::LEFT : Attack::RIGHT, agent);
}