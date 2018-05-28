#include "enemyagent.h"

#include "engine/physics/physicsshape.h"
#include "game/scene/scene.h"

/*Rectangle EnemyAgent::getDetectionShape() {
	return getOrientation() == RIGHT ? getRightDetectionShape() : getLeftDetectionShape();
}*/

Rectangle EnemyAgent::getHorizontalDetectionShape(float back, float front, float height) {

	if (getOrientation() == RIGHT) {
		return getScene()->getHorizontalProjection(getPosition().x, getPosition().x - back, getPosition().x + front, getPhysicsShape()->getMinY(), getPhysicsShape()->getMinY() + height);
	} else {
		return getScene()->getHorizontalProjection(getPosition().x, getPosition().x - front, getPosition().x + back, getPhysicsShape()->getMinY(), getPhysicsShape()->getMinY() + height);
	}
	
}

bool EnemyAgent::canMoveLeft() {
	if (getPhysicsShape()->hasLeftContact()) return false;
	test_.alignTop(getPhysicsShape()->getMinY());
	test_.alignRight(getPhysicsShape()->getMinX());
	return !getScene()->isAreaFree(test_);
}

bool EnemyAgent::canMoveRight() {
	if (getPhysicsShape()->hasRightContact()) return false;
	test_.alignTop(getPhysicsShape()->getMinY());
	test_.alignLeft(getPhysicsShape()->getMaxX());
	return !getScene()->isAreaFree(test_);
}

void EnemyAgent::leftToRightMovement() {
	// Vorm om te testen of aan het einde van platform is
	if (getPhysicsShape()->hasBottomContact()) {
	 	if (getOrientation() == RIGHT) {
	 		if (canMoveRight()) moveRight();
	 		else setOrientation(LEFT);
	 	} else {
	 		if (canMoveLeft()) moveLeft();
	 		else setOrientation(RIGHT);
	 	}

	} else {
		standStill();
	}

}