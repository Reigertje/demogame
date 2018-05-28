#ifndef LEFTTORIGHTENEMYAGENT_H
#define LEFTTORIGHTENEMYAgent_H

#include "../agent.h"

class LeftToRightEnemyAgent : public Agent {

public:

	LeftToRightEnemyAgent();

	virtual void onStep(float dt);

protected:

	void leftToRightMovement();

};

#endif