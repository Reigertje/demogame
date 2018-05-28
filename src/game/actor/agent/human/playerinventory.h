#ifndef PLAYERINVENTORY_H
#define PLAYERINVENTORY_H

#include "game/item/inventory.h"

class PlayerAgent;

class PlayerInventory {

public:

	PlayerInventory(PlayerAgent* agent); 

	void onStep(float dt);

	bool addItemIfPossible(const Item* item);

	void removeItem(const Item* item);

private:

	void print_state();

	void setSelectedItem(int selected_item);

	Inventory inventory_;

	int selected_group_;

	int selected_item_;

	PlayerAgent* agent_;

};

#endif