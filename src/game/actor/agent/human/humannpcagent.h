#ifndef HUMANNPCAGENT_H
#define HUMANNPCAGENT_H

#include "humanagent.h"
#include "playerinventory.h"

class HumanNPCAgent : public HumanAgent {
friend class PlayerInventory;

public:

	HumanNPCAgent();

	void onStep(float dt);

private:

	std::string dialogue_key_;

};

#endif