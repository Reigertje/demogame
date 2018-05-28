#ifndef PLAYERAGENT_H
#define PLAYERAGENT_H

#include "humanagent.h"
#include "playerinventory.h"

class PlayerAgent : public HumanAgent {
friend class PlayerInventory;
public:

	PlayerAgent();

	bool canInteract(const Interaction& interaction);

	bool interact(const Interaction& interaction);

	void onStep(float dt);

private:

	void controlAgent(float dt);

	void controlInventory(float dt);

	void useItem(const Item* item);

	void useInventoryEntry(const Inventory::Entry& entry);

	PlayerInventory inventory_;

	bool control_inventory_;

	float jump_delay_;

	float gravity_delay_;

	float y_position_;

};

#endif