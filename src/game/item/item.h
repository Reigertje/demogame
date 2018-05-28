#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>
#include "../actor/agent/agentstatus.h"
#include "game/combat/weapon.h"
#include "game/combat/shield.h"

class Item {

public:

	static const int DEFAULT = 0;
	static const int STATUS = 1;
	static const int APPEARANCE = 2;
	static const int WEAPON = 3;
	static const int SHIELD = 4;

	static const int GROUP_APPEARANCE = 0;
	static const int GROUP_COMBAT = 1;
	static const int GROUP_RESOURCES = 2;
	static const int GROUP_KEY = 3;
	static const int NUMBER_OF_GROUPS = 4;

	std::string getName() const { return name_; };

	int getType() const { return type_; };

	int getGroup() const { return group_; };

	bool isStackable() const { return stackable_; };

	virtual ~Item() = default;

protected:

	Item(int group, int type, bool stackable, const std::string& name) :  group_(group), type_(type), stackable_(stackable), name_(name) {};

private:

	int type_;

	int group_;

	bool stackable_;

	std::string name_;

};

class StatusItem : public Item {

public:

	StatusItem(const std::string& name, const AgentStatus& agent_status) : Item(GROUP_RESOURCES, STATUS, true, name), agent_status_(agent_status) {}

private:

	AgentStatus agent_status_;

};

class AppearanceItem : public Item {

public:

	AppearanceItem(const std::string& name, int slot, const AgentAttributes agent_attributes, const std::vector<std::string>& bodypart_keys);

	std::vector<std::string> getBodyPartKeys() const;

	AgentAttributes getAgentAttributes() const;

	int getSlot() const;

private:

	int slot_;

	std::vector<std::string> bodypart_keys_;

	AgentAttributes agent_attributes_;

};

class WeaponItem : public Item {

public:

	WeaponItem(const std::string& name, const std::vector<std::string>& bodypart_keys, const Weapon::Timing& timing);

	std::vector<std::string> getBodyPartKeys() const;

	virtual Weapon* generateWeapon() const = 0; 

	virtual ~WeaponItem() = default;

protected:

	Weapon::Timing getTiming() const;

private:

	std::vector<std::string> bodypart_keys_;

	Weapon::Timing timing_;

};


class MeleeWeaponItem : public WeaponItem {

public:

	MeleeWeaponItem(const std::string& name, const std::vector<std::string>& bodypart_keys, const Weapon::Timing& timing) : 
		WeaponItem(name, bodypart_keys, timing) {};

	Weapon* generateWeapon() const;

};

class ShieldItem : public Item {

public:

	ShieldItem(const std::string& name, const std::vector<std::string>& bodypart_keys) :
		Item(GROUP_COMBAT, SHIELD, false, name), bodypart_keys_(bodypart_keys) {};

	Shield* generateShield() const;

	std::vector<std::string> getBodyPartKeys() const;

private:

	std::vector<std::string> bodypart_keys_;

};

#endif