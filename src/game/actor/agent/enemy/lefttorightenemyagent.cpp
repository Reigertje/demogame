#include "lefttorightenemyagent.h"

#include "game/scene/scene.h"

#include "engine/physics/physicsshape.h"

LeftToRightEnemyAgent::LeftToRightEnemyAgent() {
	setBodyDrawObject("test");
	addBodyPart("test");

	setAnimation("test");
	setPhysicsShape(new PhysicsShape(PhysicsShape::COLLIDER, 16, 16));
	setSpeed(30.0f);
}

void LeftToRightEnemyAgent::leftToRightMovement() {
	// Vorm om te testen of aan het einde van platform is
	Rectangle test(1, 1);

	test.alignTop(getPhysicsShape()->getMinY());
	if (getOrientation() == RIGHT) {
		test.alignLeft(getPhysicsShape()->getMaxX());
	} else {
		test.alignRight(getPhysicsShape()->getMinX());
	}

	if (getPhysicsShape()->hasBottomContact()) {

		if (getScene()->isAreaFree(test) || (getOrientation() == RIGHT && getPhysicsShape()->hasRightContact()) || (getOrientation() == LEFT && getPhysicsShape()->hasLeftContact()) ) {
	 		setOrientation(!getOrientation());
	 	}

	 	if (getOrientation() == RIGHT) {
	 		moveRight();
	 	} else {
	 		moveLeft();
	 	}

	} else {
		standStill();
	}

}


void LeftToRightEnemyAgent::onStep(float dt) {

	leftToRightMovement();
	
	Agent::onStep(dt);
}