#include "playerinventory.h"

#include <iostream>

#include "playeragent.h"
#include "game/game.h"


PlayerInventory::PlayerInventory(PlayerAgent* agent) {
	selected_group_ = 0;
	selected_item_ = 0;
	agent_ = agent;
}

bool PlayerInventory::addItemIfPossible(const Item* item) {
	if (inventory_.canAddItem(item)) {
		inventory_.addItem(item);
		//inventory_.debug_print();
		return true;
	}
	return false;
}

void PlayerInventory::removeItem(const Item* item) {
	inventory_.removeItem(item);
	setSelectedItem(selected_item_);
}

void PlayerInventory::print_state() {
	std::cout << "=== Group " << selected_group_ << " ===" << std::endl;
	std::vector<Inventory::Entry> entries = inventory_.getEntries(selected_group_);
	if (entries.size() == 0) std::cout << "No entries." << std::endl;
	else {
		for (int i = 0; i < entries.size(); ++i) {
			std::cout << (selected_item_ == i ? "> " : "- ");
			std::cout << entries[i].getCount() << "x " << entries[i].getItem()->getName() ;
			if (agent_->isInventoryEntryEquipped(entries[i].getId())) {
				std::cout << " (E)";
			}
			std::cout << std::endl;
		}
	}
}

void PlayerInventory::setSelectedItem(int selected_item) {
	selected_item_ = selected_item;
	// FIX
	std::vector<Inventory::Entry> entries = inventory_.getEntries(selected_group_);
	if (entries.size() == 0) selected_item_ = 0;
	else if (selected_item_ < 0) selected_item_ = entries.size() + selected_item_;
	else {
		selected_item_ = selected_item_%entries.size();
	}
}

void PlayerInventory::onStep(float dt) {
	if (Game::getInstance().getInput().wasKeyPressed(Game::ACTION_INVENTORY)) {
		std::cout << "Inventory opened" << std::endl;
		print_state();
	}

	std::vector<Inventory::Entry> entries = inventory_.getEntries(selected_group_);

	if (Game::getInstance().getInput().wasKeyPressed(Game::ACTION_TAB)) {
		selected_group_ = (selected_group_ + 1)%Item::NUMBER_OF_GROUPS;
		selected_item_ = 0;
		print_state();
	} else if (Game::getInstance().getInput().wasKeyPressed(Game::ACTION_UP)) {
		setSelectedItem(selected_item_ - 1);
		print_state();
	} else if (Game::getInstance().getInput().wasKeyPressed(Game::ACTION_DOWN)) {
		setSelectedItem(selected_item_ + 1);
		print_state();
	} else if (Game::getInstance().getInput().wasKeyPressed(Game::ACTION_INTERACT) && entries.size() > 0) {
		agent_->useInventoryEntry(entries[selected_item_]);
		print_state();
	}
}