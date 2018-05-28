#include "physicsworld.h"
#include "physicsshape.h"

#include <iostream>
#include <algorithm>

PhysicsWorld::PhysicsWorld() {
	gravity_ = -400.0f;
}

void PhysicsWorld::addPhysicsShape(PhysicsShape* shape) {
	all_.push_back(shape);
	if (shape->getType() == PhysicsShape::SOLID || shape->getType() == PhysicsShape::BOTH) {
		solids_.push_back(shape);
	}
	if (shape->getType() == PhysicsShape::COLLIDER || shape->getType() == PhysicsShape::BOTH) {
		colliders_.push_back(shape);
	}
}

void PhysicsWorld::removePhysicsShape(PhysicsShape* shape) {
	all_.erase(std::remove(all_.begin(), all_.end(), shape), all_.end());
	if (shape->getType() == PhysicsShape::SOLID || shape->getType() == PhysicsShape::BOTH) {
		solids_.erase(std::remove(solids_.begin(), solids_.end(), shape), solids_.end());
	}
	if (shape->getType() == PhysicsShape::COLLIDER || shape->getType() == PhysicsShape::BOTH) {
		colliders_.erase(std::remove(colliders_.begin(), colliders_.end(), shape), colliders_.end());
	}
}

void PhysicsWorld::setGravity(float gravity) {
	gravity_ = gravity;
}

float PhysicsWorld::getGravity() const {
	return gravity_;
}

void PhysicsWorld::step(float dt) {
	for (int i = 0; i < all_.size(); ++i) {
		all_[i]->resetContacts();
		all_[i]->updateHorizontalPosition(gravity_, dt);
		if (all_[i]->getType() != PhysicsShape::SOLID) resolveHorizontalCollisions(all_[i]);
		all_[i]->updateVerticalPosition(gravity_, dt);
		if (all_[i]->getType() != PhysicsShape::SOLID) resolveVerticalCollisions(all_[i]);
		all_[i]->updateVelocity(gravity_, dt);
	}
}

void PhysicsWorld::resolveHanging(PhysicsShape* collider, PhysicsShape* solid) {
	if (collider->canHang() && solid->canHangOn() && collider->getMaxY() > solid->getMaxY() && (solid->getMaxX() == collider->getMinX() || solid->getMinX() == collider->getMaxX())) {
		// Simulate vertical velocity update - check if passes hanging point
		// if so - hang
		float orig_y = collider->getMaxY();
		vec2f orig_pos = collider->getPosition();

		// TODO pass actual dt
		collider->updateVerticalPosition(gravity_, 1.0/30.f);
		float y_after_gravity = collider->getMaxY();

		if (orig_y >= solid->getMaxY() && y_after_gravity < solid->getMaxY()) {
			collider->setIsHanging(true);
			if (collider->getVelocity().y < 0) collider->setVelocity(collider->getVelocity().x, 0);
			collider->alignTop(solid->getMaxY());
		} else {
			collider->setPosition(orig_pos);
		}
		
	}
}

void PhysicsWorld::resolveHorizontalCollisions(PhysicsShape* collider) {
	for (int i = 0; i < solids_.size(); ++i) {
		if (collider == solids_[i]) continue;
		if (collider->collides(*solids_[i])) {
			resolveHorizontalCollision(collider, solids_[i]);
			updateHorizontalContacts(collider, solids_[i]);
			
		}
		resolveHanging(collider, solids_[i]);
	}
}

void PhysicsWorld::resolveHorizontalCollision(PhysicsShape* collider, PhysicsShape* solid) {
	if (solid->getMinX() > collider->getMinX()) {
		collider->alignRight(solid->getMinX());
		if (collider->getVelocity().x > 0) collider->setVelocity(0, collider->getVelocity().y);
	} else {
		collider->alignLeft(solid->getMaxX());
		if (collider->getVelocity().x < 0) collider->setVelocity(0, collider->getVelocity().y);
	}
}

void PhysicsWorld::updateHorizontalContacts(PhysicsShape* collider, PhysicsShape* solid) {
	collider->left_contact_ = collider->left_contact_ || solid->getMaxX() == collider->getMinX();
	collider->right_contact_ = collider->right_contact_ || solid->getMinX() == collider->getMaxX();

	if (collider->canHang() && solid->canHangOn() && solid->getMaxX() == collider->getMinX()) {

		float orig_y = collider->getMaxY();
		vec2f orig_pos = collider->getPosition();
		// TODO p.ass actual dt
		collider->updateVerticalPosition(gravity_, 1.0/30.f);
		float y_after_gravity = collider->getMaxY();

		if (orig_y > solid->getMaxY() && y_after_gravity < solid->getMaxY()) {
			
			collider->setIsHanging(true);

			if (collider->getVelocity().y < 0) collider->setVelocity(collider->getVelocity().x, 0);
			collider->alignTop(solid->getMaxY());
		} else {
			collider->setPosition(orig_pos);
		}
		
	}

}

void PhysicsWorld::resolveVerticalCollisions(PhysicsShape* collider) {
	for (int i = 0; i < solids_.size(); ++i) {
		if (collider == solids_[i]) continue;
		if (collider->collides(*solids_[i])) {
			resolveVerticalCollision(collider, solids_[i]);
			updateVerticalContacts(collider, solids_[i]);
		}
	}
}

void PhysicsWorld::resolveVerticalCollision(PhysicsShape* collider, PhysicsShape* solid) {
	if (solid->getMinY() > collider->getMinY()) {
		collider->alignTop(solid->getMinY());
		if (collider->getVelocity().y > 0) collider->setVelocity(collider->getVelocity().x, 0);
	} else {
		collider->alignBottom(solid->getMaxY());
		if (collider->getVelocity().y < 0) collider->setVelocity(collider->getVelocity().x, 0);
	}
}

void PhysicsWorld::updateVerticalContacts(PhysicsShape* collider, PhysicsShape* solid) {
	collider->bottom_contact_ = collider->bottom_contact_ || solid->getMaxY() == collider->getMinY();
	collider->top_contact_ = collider->top_contact_ || solid->getMinY() == collider->getMaxY();
}


bool PhysicsWorld::isAreaFree(const Shape& shape) const {
	return !isAreaOccupied(shape);
}

bool PhysicsWorld::isAreaOccupied(const Shape& shape) const {
	for (int i = 0; i < solids_.size(); ++i) {
		if (shape.collides(solids_[i]->getRectangle())) return true;
	}	
	return false;
}

Rectangle PhysicsWorld::getHorizontalProjection(float x, float x0, float x1, float y0, float y1) const {
	float max_x = -9999.0f;
	float min_x = 9999.0f;

	for (int i = 0; i < solids_.size(); ++i) {
		if (solids_[i]->getMinY() >= y1 || solids_[i]->getMaxY() <= y0) continue;
		if (solids_[i]->getMaxX() < x && solids_[i]->getMaxX() > max_x) {
			max_x = solids_[i]->getMaxX();
		}
		if (solids_[i]->getMinX() > x && solids_[i]->getMinX() < min_x) {
			min_x = solids_[i]->getMinX();
		}
	}

	if (min_x > x1) min_x = x1;
	if (max_x < x0) max_x = x0; 

	Rectangle result((min_x - max_x)/2.0f, (y1 - y0)/2.0f);
	result.setPosition(max_x + (min_x - max_x)/2.0f, y0 + (y1 - y0)/2.0f);
	return result;
}

/*Rectangle PhysicsWorld::getLeftProjection(float x, float y0, float y1) const {
	// TODO infinity constant
	float max_x = -9999.0f;

	for (int i = 0; i < solids_.size(); ++i) {
		if (solids_[i]->getMaxX() > x) continue;
		if (solids_[i]->getMinY() < y1 && solids_[i]->getMaxY() > y0 && solids_[i]->getMaxX() > max_x) {
			max_x = solids_[i]->getMaxX();
		}
	}
	Rectangle result((x - max_x)/2.0f, (y1 - y0)/2.0f);

	result.setPosition(max_x + (x - max_x)/2.0f, y0 + (y1 - y0)/2.0f);
	return result;
}

Rectangle PhysicsWorld::getRightProjection(float x, float y0, float y1) const {
	// TODO infinity constant
	float min_x = 9999.0f;

	for (int i = 0; i < solids_.size(); ++i) {
		if (solids_[i]->getMinX() < x) continue;
		if (solids_[i]->getMinY() < y1 && solids_[i]->getMaxY() > y0 && solids_[i]->getMinX() < min_x) {
			min_x = solids_[i]->getMinX();
		}
	}
	Rectangle result((min_x - x)/2.0f, (y1 - y0)/2.0f);

	result.setPosition(x + (min_x - x)/2.0f, y0 + (y1 - y0)/2.0f);
	return result;
}*/