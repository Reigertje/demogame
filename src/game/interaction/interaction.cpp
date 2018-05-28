#include "interaction.h"

#include <string>

#include "game/actor/itemactor.h"
#include "game/item/item.h"
#include "game/scene/scene.h"

Interaction::Interaction(int type) {
	type_ = type;
}

void Interaction::performInteraction(Actor& actor) {
	if (actor.interact(*this)) {
		onPerformed(actor);	
	}
}

int Interaction::getType() const {
	return type_;
}

// ITEM

void ItemInteraction::onPerformed(Actor& actor) {
	item_actor_->getScene()->removeActor(item_actor_);
}

const Item* ItemInteraction::getItem() const {
	return item_;
}

std::string ItemInteraction::getDescription() const {
	return "Pick up " + item_->getName(); 
}

// LINK

void LinkInteraction::onPerformed(Actor& actor) {
	//item_actor_->getScene()->removeActor(item_actor_);
}

std::string LinkInteraction::getDestination() const {
	return to_;
}

std::string LinkInteraction::getDescription() const {
	return "Enter"; // TODO  
}


// DIALOGUE

void DialogueInteraction::onPerformed(Actor& actor) {

}

std::string DialogueInteraction::getKey() const {
	return key_;
}

std::string DialogueInteraction::getDescription() const {
	return "Talk";
}