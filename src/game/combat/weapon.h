#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <vector>
#include "combat.h"

class vec2f;
class Agent;
class Actor;
class Shape;
class CombatProperties;

class Weapon {

public:

	static const int REST = 0;
	static const int PREPARE = 1;
	static const int LOAD = 2;
	static const int CHARGE = 3;
	static const int ATTACK = 4;
	static const int COOLDOWN = 5;

	struct Timing {

		Timing(float prepare, float load, float charge, float attack, float cooldown);

		Timing(float prepare, float attack, float cooldown);

		float states_[6];

	};

	Weapon(const Timing& timing);

	virtual ~Weapon() = default;

	bool isAttacking() const;

	int getCurrentState() const;

	std::string getCurrentStateString() const;

	float getCurrentStateTime() const;

	float getStateTime(int state) const;

	void startAttack();

	void step(Agent& agent, float dt);

protected:

	virtual void performAttack(Agent& agent) = 0;

private:

	Timing timing_;

	float current_time_;

	int current_state_;

	void nextState();

	void changeState(int previous_state, int new_state);

	virtual void onStateChanged(int previous_state, int new_state) {};

};

class MeleeWeapon : public Weapon {

public:

	MeleeWeapon(const Timing& timing);

private:

	void performAttack(Agent& agent);

	void onStateChanged(int previous_state, int new_state);

	Attack attack_;

};


#endif