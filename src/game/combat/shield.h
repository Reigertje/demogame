#ifndef SHIELD_H
#define SHIELD_H

#include "combat.h"

class Shield {

public:

	static const int REST = 0;
	static const int START_BLOCK = 1;
	static const int BLOCK = 2;

	Shield();

	void startBlock();

	void stopBlock();

	bool isBlocking() const;

	void step(float dt);

	CombatProperties getCombatProperties() const;

private:

	int current_state_;

	float current_time_;

	bool release_;

	CombatProperties combat_properties_;

};

#endif