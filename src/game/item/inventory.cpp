#include "inventory.h"

#include <iostream>
#include <algorithm>


long Inventory::Entry::next_id = 1;

long Inventory::Entry::getNextId() {
	return next_id++;
}

static bool compareEntries(const Inventory::Entry& a, const Inventory::Entry& b) {
	if (a.getItem() == b.getItem()) {
		return a.getCount() > b.getCount(); 
	}
	if (a.getItem()->getType() == b.getItem()->getType()) {
		return a.getItem()->getName() < b.getItem()->getName();
	}
	return a.getItem()->getType() < b.getItem()->getType();
}


Inventory::Inventory() {
	slot_limits_.resize(Item::NUMBER_OF_GROUPS);
	items_.resize(Item::NUMBER_OF_GROUPS);
	stack_limit_ = 5;

	for (int i = 0; i < Item::NUMBER_OF_GROUPS; ++i) {
		slot_limits_[i] = 5;
	}
}


bool Inventory::canAddItem(const Item* item) const {
	return getAvailableEntry(item) || slot_limits_[item->getGroup()] == -1 || slot_limits_[item->getGroup()] > items_[item->getGroup()].size();
}

void Inventory::addItem(const Item* item) {
	Entry* entry = getAvailableEntry(item); 
	if (entry) {
		// Item zit al in inventory
		entry->count_++;
	} else {
		items_[item->getGroup()].push_back(Entry(item));
	}
	sort();
}

void Inventory::removeItem(const Item* item) {
	// Find smallest stack, list is sorted
	Entry* entry = 0; 
	int entry_index_;
	for (int i = items_[item->getGroup()].size() - 1; i >= 0; --i) {
		if (items_[item->getGroup()][i].item_ == item) {
			entry = &items_[item->getGroup()][i];
			entry_index_ = i;
			break;
		}
	}

	if (entry) {
		entry->count_--;
		if (entry->count_ <= 0) {
			items_[item->getGroup()].erase(items_[item->getGroup()].begin() + entry_index_);
		}
	}
}

std::vector<Inventory::Entry> Inventory::getEntries(int item_class) const {
	return items_[item_class];
}


const Inventory::Entry* Inventory::getAvailableEntry(const Item* item) const {
	if (!item->isStackable()) return 0;
	for (int i = 0; i < items_[item->getGroup()].size(); ++i) {
		if (items_[item->getGroup()][i].item_ == item && items_[item->getGroup()][i].count_ < stack_limit_) {
			return &items_[item->getGroup()][i];
		}
	}
	return 0;
}

Inventory::Entry* Inventory::getAvailableEntry(const Item* item) {
	if (!item->isStackable()) return 0;
	for (int i = 0; i < items_[item->getGroup()].size(); ++i) {
		if (items_[item->getGroup()][i].item_ == item && items_[item->getGroup()][i].count_ < stack_limit_) {
			return &items_[item->getGroup()][i];
		}
	}
	return 0;
}

void Inventory::sort() {
	for (int i = 0; i < Item::NUMBER_OF_GROUPS; ++i) {
		sort(i);
	}
}

void Inventory::sort(int group) {
	std::sort(items_[group].begin(), items_[group].end(), compareEntries);
}

void Inventory::debug_print() const {
	for (int i = 0; i < Item::NUMBER_OF_GROUPS; ++i) {
		std::cout << "GROUP " << i << std::endl;
		if (items_[i].size() == 0) std::cout << "No items." << std::endl;
		else {
			for (int j = 0; j < items_[i].size(); ++j) {
				std::cout << "{ " << items_[i][j].count_ << "x " << items_[i][j].item_->getName() << " }" << std::endl; 
			}
		}
	}
}