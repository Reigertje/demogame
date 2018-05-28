#include "playeragent.h"

#include "game/game.h"
#include "game/scene/scene.h"
#include "game/combat/weapon.h"
#include "engine/physics/physicsshape.h"
#include "engine/geom/circle.h"

#include "game/interaction/interaction.h"

#include "game/item/item.h"

#include <iostream>

PlayerAgent::PlayerAgent() : inventory_(this) {
	control_inventory_ = false;

	AgentAttributes attributes;

	attributes.setVitality(4);

	setBaseAttributes(attributes);

	jump_delay_ = 0.0f;
	gravity_delay_ = 0.0f;
	y_position_ = 0.0f;
}

bool PlayerAgent::canInteract(const Interaction& interaction) {
	return true;	
}

bool PlayerAgent::interact(const Interaction& interaction) {
	if (interaction.getType() == Interaction::ITEM) {
		const ItemInteraction item_interaction = (const ItemInteraction&)interaction;
		
		// Inventory not used in demo for lack of UI
		//return inventory_.addItemIfPossible(item_interaction.getItem());
		useItem(item_interaction.getItem());

		return true;
		
	} else if (interaction.getType() == Interaction::LINK) {
		Game::getInstance().getGameWorld().requestChangeLevel(((const LinkInteraction&)interaction).getDestination());
	} else if (interaction.getType() == Interaction::DIALOGUE) {
		getScene()->startDialogue(((const DialogueInteraction&)interaction).getKey());
	}
	return false;
}

void PlayerAgent::controlAgent(float dt) {
	if (Game::getInstance().getInput().wasKeyPressed(Game::ACTION_ATTACK) && canAttack()) {
		startAttack();
	}  

	if (Game::getInstance().getInput().isKeyDown(Game::ACTION_BLOCK)) {
		if (canBlock()) {
			startBlock();
		}
	} else if (isBlocking()) {
		stopBlock();
	}

	if (Game::getInstance().getInput().isKeyDown(Game::ACTION_SPRINT)) {
		setSpeed(150.f);
	} else {
		setSpeed(100.0f);
	}

	if ((!isBlocking() && !isAttacking()) || !getPhysicsShape()->hasBottomContact()) {

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

	if (Game::getInstance().getInput().wasKeyPressed(Game::ACTION_UP)) {
		if (jump()) {
			jump_delay_ = 0.18f;
		}
	} else if (Game::getInstance().getInput().isKeyDown(Game::ACTION_DOWN)) {
		getPhysicsShape()->releaseHang();
	}

	if (!Game::getInstance().getInput().isKeyDown(Game::ACTION_UP) && jump_delay_ <= 0) {
		haltJump();
	}

	Rectangle r(10.0f, 10.0f);
	r.setPosition(getPosition());

	std::vector<Interaction*> interactions =  getScene()->getInteractions(r); 

	if (Game::getInstance().getInput().wasKeyPressed(Game::ACTION_INTERACT) && interactions.size() > 0) {
		interactions[0]->performInteraction(*this);
	}
}


void PlayerAgent::useItem(const Item* item) {

	if (item->getType() == Item::APPEARANCE) {
		/*if (isInventoryEntryEquipped(entry.getId())) {
			unequip(((const AppearanceItem*)item)->getSlot());
		} else {*/
			equip((const AppearanceItem*)item, 0);
		//}
	} else if (item->getType() == Item::WEAPON){ 
		/*if (isInventoryEntryEquipped(entry.getId())) {
			unequipWeapon();
		} else {*/
			equip((const WeaponItem*)item, 1);
		//}
	} else if (item->getType() == Item::SHIELD){
		/*if (isInventoryEntryEquipped(entry.getId())) {
			unequipShield();
		} else {*/
			equip((const ShieldItem*)item, 2);
		//}
	} else {

		//inventory_.removeItem(item);
	}
}

void PlayerAgent::useInventoryEntry(const Inventory::Entry& entry) {
	// Inventory is not used in demo for lack of UI

	const Item* item = entry.getItem();

	if (item->getType() == Item::APPEARANCE) {
		if (isInventoryEntryEquipped(entry.getId())) {
			unequip(((const AppearanceItem*)item)->getSlot());
		} else {
			equip((const AppearanceItem*)item, entry.getId());
		}
	} else if (item->getType() == Item::WEAPON){ 
		if (isInventoryEntryEquipped(entry.getId())) {
			unequipWeapon();
		} else {
			equip((const WeaponItem*)item, entry.getId());
		}
	} else if (item->getType() == Item::SHIELD){
		if (isInventoryEntryEquipped(entry.getId())) {
			unequipShield();
		} else {
			equip((const ShieldItem*)item, entry.getId());
		}
	} else {

		inventory_.removeItem(item);
	}

}

void PlayerAgent::controlInventory(float dt) {
	// Inventory is not used in demo for lack of UI
	inventory_.onStep(dt);
}


void PlayerAgent::onStep(float dt) {

	if (jump_delay_ > 0.0f) {
		jump_delay_ -= dt;
	}
	// Inventory is not used in demo for lack of UI
	/*if (Game::getInstance().getInput().wasKeyPressed(Game::ACTION_INVENTORY)) {
		standStill();
		control_inventory_ = !control_inventory_;
	}*/

	if (getScene()->isInDialogue()) {
		standStill();
	} else if (control_inventory_) {
		controlInventory(dt);
	} else {
		controlAgent(dt);
	}

	/*if (getPhysicsShape()->hasBottomContact()) {
		gravity_delay_ = 0.25f;
		y_position_ = getPhysicsShape()->getPosition().y;
		getPhysicsShape()->setGravityFactor(1);
		if (getPhysicsShape()->getVerticalVelocity() < 0) {
			getPhysicsShape()->setVerticalVelocity(0);
		}
	} else if (gravity_delay_ > 0.0f) {
		getPhysicsShape()->setGravityFactor(0);
		if (getPhysicsShape()->getVerticalVelocity() < 0) {
			getPhysicsShape()->setVerticalVelocity(0);
			getPhysicsShape()->setY(y_position_);
		}
		gravity_delay_ -= dt;
	} else {
		getPhysicsShape()->setGravityFactor(1);
	}

	std::cout << getPhysicsShape()->getGravityFactor() << std::endl;

	if (getPhysicsShape()->getVerticalVelocity() > 0 || !isMoving()) {
		gravity_delay_ = 0;
	}*/

	// TODO TODO TODO 
	// TODO TODO TODO 

	HumanAgent::onStep(dt);
	getScene()->setCameraPosition(getPosition() + vec2f(0.0f, 20.0f));
}