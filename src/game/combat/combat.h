#ifndef COMBAT_H
#define COMBAT_H

#include <vector>
#include "engine/geom/vec2f.h"

class Shape;
class Actor;
class Agent;

class CombatProperties {

public:

	static const int PHYSICAL = 0;

	static const int ELEMENTAL = 1; 

	static const int TYPE_COUNT = 2;

	CombatProperties();

	void set(int type, float value);

	float get(int type) const;

	float getTotal() const;

	CombatProperties operator+(const CombatProperties& other);

	CombatProperties operator-(const CombatProperties& other);

	CombatProperties operator*(const CombatProperties& other);

private:

	float values_[TYPE_COUNT];

};

class AttackProperties {

public:

	AttackProperties(const CombatProperties& damage_properties = CombatProperties()) : damage_properties_(damage_properties), attack_direction_(0) {};

	CombatProperties getDamageProperties() const;

	int getAttackDirection() const;

	void attack(int attack_direction);

private:

	CombatProperties damage_properties_;

	int attack_direction_;

};

class DefenceProperties {

public:

	DefenceProperties(const CombatProperties& body_properties = CombatProperties()) : 
		body_properties_(body_properties), block_directions_(0) {};

	CombatProperties getBodyProperties() const;

	CombatProperties getBlockProperties() const;

	int getBlockDirections() const;

	void block(const CombatProperties& block_properties, int block_directions);

private:

	CombatProperties body_properties_;

	CombatProperties block_properties_;

	int block_directions_;

};

class AttackHit {

public:

	AttackHit(long actor_id, const AttackProperties& attack_properties) : actor_id_(actor_id), attack_properties_(attack_properties) {};

	AttackHit(const AttackProperties& attack_properties) : actor_id_(0), attack_properties_(attack_properties) {};

	AttackProperties getAttackProperties() const;

	long getActorId() const;

private:

	long actor_id_;

	AttackProperties attack_properties_;

};

class Attack {

public: 

	static const int UP = 1;
	
	static const int LEFT = 2;
	
	static const int DOWN = 4;

	static const int RIGHT = 8;

	Attack();

	~Attack();

	void clearFilter();

	void performAttack(int direction, Agent& agent);

	void setAttackProperties(const AttackProperties& properties);

	void setShape(const Shape& shape);

	void updateShapePosition(const vec2f& position);

	void addGroupToFilter(int group);

	static float calculateDamage(const AttackProperties& attack, const DefenceProperties& defence);
	
private:

	std::vector<long> actor_filter_;

	std::vector<int> group_filter_;

	AttackProperties properties_;

	Shape* shape_;

	bool filter(const Actor& actor);

	bool isInFilter(const Actor& actor);

	bool isGroupInFilter(int group);

	void addToFilter(const Actor& actor);

};

#endif