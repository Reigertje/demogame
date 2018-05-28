#include "tanukiagent.h"

#include "game/scene/scene.h"

#include "engine/physics/physicsshape.h"
#include "engine/graphics/bodydrawobject.h"

#include <iostream>

TanukiAgent::TanukiAgent() {
	state_ = NORMAL;

	setGroup(TANUKI);
	setBodyDrawObject("tanuki");
	addBodyPart("tanuki_default");

	setAnimation("walk");
	setPhysicsShape(new PhysicsShape(PhysicsShape::COLLIDER, 12, 5));
	addHitShape(new Rectangle(12, 5));
	setSpeed(30.0f);
	setJump(150.f);
	setDrawObjectOffset(0, vec2f(0, 11));
	getBodyDrawObject()->setDrawLayer(-5);

		//TODO hrad coded value to test
	
	CombatProperties properties;
	properties.set(CombatProperties::PHYSICAL, 2);

	Rectangle rect(5, 5);

	attack_.setAttackProperties(AttackProperties(properties));
	attack_.setShape(rect);

	attack_.addGroupToFilter(TANUKI);
	relax_time_ = 0;
}


void TanukiAgent::onStep(float dt) {

	std::vector<Actor*> hits = getScene()->getHits( getHorizontalDetectionShape(15.0f, 100.0f, 30.0f));

	//getScene()->debugShape(getHorizontalDetectionShape(15.0, 100.0, 30.0));

	Actor* threat = 0;
	for (int i = 0; i < hits.size(); ++i) {
		if (hits[i]->isAgent() && hits[i]->asAgent()->getGroup() != TANUKI) {
			threat = hits[i];
			break;
		}
	}

	if (relax_time_ > 0.0f) {
		relax_time_ -= dt;
	}

	if ((state_ == RUNNING || state_ == CORNERED) && relax_time_ < 0.0f) {
		state_ = NORMAL;
		relax_time_ = 0.0f;
	}

	if (threat) {
		relax_time_ = 6.0f; // Seconds
		float distance_to_threat = (getPosition() - threat->getPosition()).length();
		if ((state_ == CORNERED && distance_to_threat < 63) || (distance_to_threat < 15.0f && state_ != ATTACKING)) {
			state_ = START_ATTACK;
		} else if (state_ == NORMAL) {
			state_ = RUNNING;
			setOrientation(hits[0]->getPosition().x < getPosition().x ? RIGHT : LEFT);
		}
	}

	if (state_ == NORMAL) {
		setSpeed(30.0f);
		leftToRightMovement();
	}

	if (state_ == RUNNING) {
		setSpeed(150.0);
		if (getOrientation() == LEFT) {
			if (canMoveLeft()) moveLeft();
			else {
				state_ = CORNERED;
				standStill();
				setOrientation(RIGHT);
			}
		} else {
			if (canMoveRight()) moveRight();
			else {
				state_ = CORNERED;
				standStill();
				setOrientation(LEFT);
			}
		}
	}

	if (state_ == START_ATTACK) {
		if (getPhysicsShape()->hasBottomContact()) {
			jump();
		} else {
			state_ = ATTACKING;
			attack_.clearFilter();
		}
	}

	if (state_ == ATTACKING) {

		attack_.updateShapePosition(getPosition() + vec2f(getOrientation() == LEFT ? -10 : 10, 5));
		attack_.performAttack(getOrientation() == LEFT ? Attack::LEFT : Attack::RIGHT, *this);

		if (getOrientation() == LEFT) {
			moveLeft();
		} else {
			moveRight();
		}
		if (getPhysicsShape()->hasBottomContact()) {
			state_ = RUNNING;
		}
	}
 
	// ANimate

	if (state_ == NORMAL || state_ == RUNNING) {
		setAnimation("walk");
	} else if (state_ == CORNERED) {
		setAnimation(threat ? "cornered" : "walk");
	} else if (state_ == START_ATTACK || ATTACKING) {
		setAnimation("attack");
	}


	EnemyAgent::onStep(dt);
}