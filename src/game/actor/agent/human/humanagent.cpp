#include "humanagent.h"

#include "game/game.h"
#include "game/combat/weapon.h"
#include "engine/physics/physicsshape.h"

#include "engine/graphics/texture.h"
#include "engine/graphics/bodydrawobject.h"
#include "engine/graphics/frameanimation.h"

#include "../bodypartattachment/bodypartattachment.h"

#include "game/item/item.h"

#include "game/scene/scene.h"

#include <iostream>

HumanAgent::HumanAgent() {
	setGroup(HUMAN);
	for (int i = 0; i < EQUIPMENT_SLOT_COUNT; ++i) {
		equipment_[i] = 0;
	}
	weapon_equipment_ = 0;
	shield_equipment_ = 0;

	setBodyDrawObject("human");
	addBodyPart("human.b");
	addBodyPart("human.ba");
	addBodyPart("human.bl");
	addBodyPart("human.fl");
	addBodyPart("human.fa");

	getBodyDrawObject()->setMixColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.1f));

	setAnimation("stand");

	setPhysicsShape(new PhysicsShape(PhysicsShape::COLLIDER, 6, 16));

	getPhysicsShape()->setCanHang(true);
	setDrawObjectOffset(0, vec2f(0, -16));

	addHitShape(new Rectangle(6, 16));

	setPosition(vec2f(0, 100));
	setSpeed(100.0f);
	setJump(240.0f);
}

DefenceProperties HumanAgent::getDefenceProperties() const {

	DefenceProperties result;
	if (isBlocking() && shield_equipment_) {
		result.block(shield_equipment_->shield_->getCombatProperties(), getOrientation() == LEFT ? Attack::RIGHT : Attack::LEFT);
	}
	return result;

}

void HumanAgent::startAttack() {
	if (weapon_equipment_) {
		stopBlock();
		weapon_equipment_->weapon_->startAttack();
	}
}

void HumanAgent::startBlock() {
	if (shield_equipment_) {
		shield_equipment_->shield_->startBlock();
	}
}

void HumanAgent::stopBlock() {
	if (shield_equipment_) {
		shield_equipment_->shield_->stopBlock();
	}
}

bool HumanAgent::isAttacking() const {
	return weapon_equipment_ && weapon_equipment_->weapon_->isAttacking();
}

bool HumanAgent::isBlocking() const {
	return shield_equipment_ && shield_equipment_->shield_->isBlocking();
}

bool HumanAgent::canAttack() {
	return !isAttacking();
}

bool HumanAgent::canBlock() {
	return !isAttacking() && !isBlocking();
}

static std::vector<BodyPartDrawObject*> keysToDrawObjects(const std::vector<std::string>& keys) {
	std::vector<BodyPartDrawObject*> bodypart_drawobjects;
	for (int i = 0; i < keys.size(); ++i) {
		BodyPartDrawObject* bodypart_drawobject = Game::getInstance().getResources().getBodyPartDrawObject(keys[i]);

		// TODO hack

		bodypart_drawobjects.push_back(bodypart_drawobject);
	}
	return bodypart_drawobjects;
}

void HumanAgent::equip(const AppearanceItem* item, long entry_id) {
	unequip(item->getSlot());

	// alle bodyparts genereren en toevoegen aan de body
	// nieuwe equipment setten
	std::vector<BodyPartDrawObject*> bodypart_drawobjects = keysToDrawObjects(item->getBodyPartKeys());
	for (int i = 0; i < bodypart_drawobjects.size(); ++i) {
		addBodyPart(bodypart_drawobjects[i]);
	}
	equipment_[item->getSlot()] = new Equipment(item, bodypart_drawobjects, entry_id);
}

void HumanAgent::equip(const WeaponItem* item, long entry_id) {
	unequipWeapon();

	std::vector<BodyPartDrawObject*> bodypart_drawobjects = keysToDrawObjects(item->getBodyPartKeys());
	for (int i = 0; i < bodypart_drawobjects.size(); ++i) {

		if (item->getBodyPartKeys()[i] == "human.torch_flame") {
			BodyPartAttachment* attachment_test = new FlameAttachment();
			attachment_test->addToScene(getScene());

			bodypart_drawobjects[i]->putAttachment(attachment_test);


		}
					addBodyPart(bodypart_drawobjects[i]);

	}
	weapon_equipment_ = new WeaponEquipment(item, bodypart_drawobjects, entry_id, item->generateWeapon());
}

void HumanAgent::equip(const ShieldItem* item, long entry_id) {
	unequipShield();

	std::vector<BodyPartDrawObject*> bodypart_drawobjects = keysToDrawObjects(item->getBodyPartKeys());
	for (int i = 0; i < bodypart_drawobjects.size(); ++i) {
		addBodyPart(bodypart_drawobjects[i]);
	}

	shield_equipment_ = new ShieldEquipment(item, bodypart_drawobjects, entry_id, item->generateShield());
}

void HumanAgent::unequip(int slot) {
	// alle bodyparts van equipment uit de body halen en deleten
	// equipment weer op null zetten
	if (equipment_[slot]) {
		for (int i = 0; i < equipment_[slot]->bodypart_drawobjects_.size(); ++i) {
			removeBodyPart(equipment_[slot]->bodypart_drawobjects_[i]);
			delete equipment_[slot]->bodypart_drawobjects_[i];
		}
		delete equipment_[slot];
	}
	equipment_[slot] = 0;
}

void HumanAgent::unequipWeapon() {
	if (weapon_equipment_) {
		for (int i = 0; i < weapon_equipment_->bodypart_drawobjects_.size(); ++i) {
			removeBodyPart(weapon_equipment_->bodypart_drawobjects_[i]);
			delete weapon_equipment_->bodypart_drawobjects_[i];
		}
		delete weapon_equipment_->weapon_;
		delete weapon_equipment_;
	}
	weapon_equipment_ = 0;
}

void HumanAgent::unequipShield() {
	if (shield_equipment_) {
		for (int i = 0; i < shield_equipment_->bodypart_drawobjects_.size(); ++i) {
			removeBodyPart(shield_equipment_->bodypart_drawobjects_[i]);
			delete shield_equipment_->bodypart_drawobjects_[i];
			delete shield_equipment_->shield_;
		}
		delete shield_equipment_;
	}
	shield_equipment_ = 0;
}

bool HumanAgent::isInventoryEntryEquipped(long entry_id) const {
	if (weapon_equipment_ && weapon_equipment_->entry_id_ == entry_id) return true;
	if (shield_equipment_ && shield_equipment_->entry_id_ == entry_id) return true;
	for (int i = 0; i < EQUIPMENT_SLOT_COUNT; ++i) {
		if (equipment_[i] && equipment_[i]->entry_id_ == entry_id) {
			return true;
		}
	}
	return false;
}

void HumanAgent::animate() {

    bool isAirborn = !getPhysicsShape()->hasBottomContact(); 

	if (isAttacking()) {
		transitionAnimation(isAirborn ? "slash_jump." + weapon_equipment_->weapon_->getCurrentStateString() : "slash." +  weapon_equipment_->weapon_->getCurrentStateString());
		getBodyDrawObject()->setCurrentAnimationDuration(weapon_equipment_->weapon_->getCurrentStateTime());
	} else if (isBlocking()) {
		transitionAnimation(isAirborn ? "block_jump" : "block");
	} else if (isAirborn) {
		setAnimation("jump");
	} else if (isMoving()) {
		setAnimation("run");
		getBodyDrawObject()->setCurrentAnimationSpeed(getSpeed() / 100.0f);
	} else {
		setAnimation("stand");
	}
}

void HumanAgent::onStep(float dt) {

	/*if (Game::getInstance().getInput().wasKeyPressed(Game::ACTION_ATTACK)) {
		weapon_->startAttack();
	}

	if (!weapon_->isAttacking() || !getPhysicsShape()->hasBottomContact()) {

		if (Game::getInstance().getInput().isKeyDown(Game::ACTION_LEFT)) {
			moveLeft();
		} else if (Game::getInstance().getInput().isKeyDown(Game::ACTION_RIGHT)) {
			moveRight();
		} else {
			standStill();
		}	

	} else {
		standStill();
	}


	if (Game::getInstance().getInput().wasKeyPressed(Game::ACTION_JUMP)) {
		jump();
	}*/

	animate();

	if (weapon_equipment_) weapon_equipment_->weapon_->step(*this, dt);
	if (shield_equipment_) shield_equipment_->shield_->step(dt);

	Agent::onStep(dt);
}