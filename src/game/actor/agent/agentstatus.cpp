#include "agentstatus.h"

int AgentStatus::getStatusMaxForAttribute(int attribute) {
	return 1 + attribute; 
}

float AgentStatus::getStatusRestorionForAttribute(int attribute) {
	return attribute * 0.25;
}

float AgentStatus::getHp() const {
	return hp_;
}

void AgentStatus::setHp(float hp) {
	hp_ = hp;
}

float AgentStatus::getAp() const {
	return ap_;
}

void AgentStatus::setAp(float ap) {
	ap_ = ap;
}

float AgentStatus::getMp() const {
	return mp_;
}

void AgentStatus::setMp(float mp) {
	mp_ = mp;
}

void AgentStatus::initialize(const AgentAttributes& attributes) {
	setHp(getStatusMaxForAttribute(attributes.getVitality()));
	setAp(getStatusMaxForAttribute(attributes.getEndurance()));
	setMp(getStatusMaxForAttribute(attributes.getSpirituality()));
}

void AgentStatus::step(const AgentAttributes& attributes, float dt) {
	// TODO 

}
