#ifndef ENEMYAGENT_H
#define ENEMYAGENT_H

#include "../agent.h"
#include "engine/geom/rectangle.h"

class EnemyAgent : public Agent {

public:

	EnemyAgent() : test_(1, 1) {};

protected:

	Rectangle getHorizontalDetectionShape(float back, float front, float height);

	bool canMoveLeft();

	bool canMoveRight();

	void leftToRightMovement();

private:

	Rectangle test_;

};

#endif