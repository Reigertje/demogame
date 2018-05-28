#include "item.h"


AppearanceItem::AppearanceItem(const std::string& name, int slot, const AgentAttributes agent_attributes, const std::vector<std::string>& bodypart_keys)
	: Item(GROUP_APPEARANCE, APPEARANCE, false, name)
{
	agent_attributes_ = agent_attributes;
	bodypart_keys_ = bodypart_keys;
	slot_ = slot;
}

std::vector<std::string> AppearanceItem::getBodyPartKeys() const {
	return bodypart_keys_;
}

AgentAttributes AppearanceItem::getAgentAttributes() const {
	return agent_attributes_;
}

int AppearanceItem::getSlot() const {
	return slot_;
}

WeaponItem::WeaponItem(const std::string& name, const std::vector<std::string>& bodypart_keys, const Weapon::Timing& timing) 
	: Item(GROUP_COMBAT, WEAPON, false, name), timing_(timing)
{
	bodypart_keys_ = bodypart_keys;
}

std::vector<std::string> WeaponItem::getBodyPartKeys() const {
	return bodypart_keys_;
}

Weapon::Timing WeaponItem::getTiming() const {
	return timing_;
}


Weapon* MeleeWeaponItem::generateWeapon() const {
	return new MeleeWeapon(getTiming());
}

Shield* ShieldItem::generateShield() const {
	return new Shield();
}

std::vector<std::string> ShieldItem::getBodyPartKeys() const {
	return bodypart_keys_;
}