#include "agentattributes.h"


int AgentAttributes::getRegeneration() const {
	return regeneration_;
}

void AgentAttributes::setRegeneration(int regeneration) {
	regeneration_ = regeneration;
}

int AgentAttributes::getVitality() const {
	return vitality_;
}

void AgentAttributes::setVitality(int vitality) {
	vitality_ = vitality;
}

int AgentAttributes::getEndurance() const {
	return endurance_;
}

void AgentAttributes::setEndurance(int endurance) {
	endurance_ = endurance;
}

int AgentAttributes::getRecovery() const {
	return recovery_;
}

void AgentAttributes::setRecovery(int recovery) {
	recovery_ = recovery;
}

int AgentAttributes::getSpirituality() const {
	return spirituality_;
}

void AgentAttributes::setSpirituality(int spirituality) {
	spirituality_ = spirituality;
}

int AgentAttributes::getWillpower() const {
	return willpower_;
}

void AgentAttributes::setWillpower(int willpower) {
	willpower_ = willpower;
}