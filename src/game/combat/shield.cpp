#include "shield.h"

#include <iostream>


static const float DELAY = 0.2f;


Shield::Shield() {
	current_state_ = REST;
	release_ = false;
	combat_properties_.set(CombatProperties::PHYSICAL, 2);
}

void Shield::startBlock() {
	if (current_state_ != REST) return;
	current_time_ = DELAY;
	current_state_ = START_BLOCK;
	release_ = false;
}

void Shield::stopBlock() {
	release_ = true;
}

bool Shield::isBlocking() const {
	return current_state_ != REST;
}

void Shield::step(float dt) {

	if (current_state_ == REST) return;
	if (current_state_ == BLOCK && release_) {
		current_state_ = REST;
		return;
	} 
	
	current_time_ -= dt;

	if (current_time_ <= 0) {
		current_state_ = BLOCK;
	}
}

CombatProperties Shield::getCombatProperties() const {
	return combat_properties_;
}