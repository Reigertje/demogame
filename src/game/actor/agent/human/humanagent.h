#ifndef HUMANAGENT_H
#define HUMANAGENT_H

#include "../agent.h"

class Weapon;
class Shield;
class BodyPartDrawObject;
class AppearanceItem;
class WeaponItem;
class ShieldItem;
class Item;

class HumanAgent : public Agent {

public:

	static const int EQUIPMENT_HEAD = 0;
	static const int EQUIPMENT_BODY = 1;
	static const int EQUIPMENT_LEGS = 2;
	static const int EQUIPMENT_SLOT_COUNT = 3;

	HumanAgent();

	virtual void onStep(float dt);

	bool isAttacking() const;

	bool isBlocking() const;

	DefenceProperties getDefenceProperties() const;

protected:


	struct Equipment {

		Equipment(const Item* item, const std::vector<BodyPartDrawObject*>& bodypart_drawobjects, long entry_id) 
			: item_(item), bodypart_drawobjects_(bodypart_drawobjects), entry_id_(entry_id) {};

		const Item* item_;

		std::vector<BodyPartDrawObject*> bodypart_drawobjects_;

		// Track id in inventory
		long entry_id_;
	};

	struct ShieldEquipment : public Equipment {

		ShieldEquipment(const Item* item, const std::vector<BodyPartDrawObject*>& bodypart_drawobjects, long entry_id, Shield* shield)
			: Equipment(item, bodypart_drawobjects, entry_id), shield_(shield) {};


		Shield* shield_;
	};

	struct WeaponEquipment : public Equipment {

		WeaponEquipment(const Item* item, const std::vector<BodyPartDrawObject*>& bodypart_drawobjects, long entry_id, Weapon* weapon)
			: Equipment(item, bodypart_drawobjects, entry_id), weapon_(weapon) {};

		Weapon* weapon_;

	};

	void equip(const AppearanceItem* item, long entry_id_ = 0);

	void equip(const WeaponItem* item, long entry_id_ = 0);

	void equip(const ShieldItem* item, long entry_id_ = 0);

	void unequip(int slot);

	void unequipWeapon();

	void unequipShield();

	bool isInventoryEntryEquipped(long entry_id) const;

	void startAttack();

	void startBlock();

	void stopBlock();

	bool canAttack();

	bool canBlock();

private:

	void animate();

	WeaponEquipment* weapon_equipment_;

	ShieldEquipment* shield_equipment_;

	Equipment* equipment_[EQUIPMENT_SLOT_COUNT];

};

#endif