#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include <vector>

class Rectangle;
class PhysicsShape;
class Shape;

class PhysicsWorld {

public:

	PhysicsWorld();

	void addPhysicsShape(PhysicsShape* shape);

	void removePhysicsShape(PhysicsShape* shape);

	void setGravity(float gravity);

	float getGravity() const;

	bool isAreaFree(const Shape& shape) const;

	bool isAreaOccupied(const Shape& shape) const;

	Rectangle getHorizontalProjection(float x, float x0, float x1, float y0, float y1) const;

/*	Rectangle getLeftProjection(float x, float y0, float y1) const;

	Rectangle getRightProjection(float x, float y0, float y1) const;*/

	void step(float dt);

private:

	void resolveHanging(PhysicsShape* collider, PhysicsShape* solid);

	void resolveHorizontalCollisions(PhysicsShape* collider);

	void resolveHorizontalCollision(PhysicsShape* collider, PhysicsShape* solid);

	void updateHorizontalContacts(PhysicsShape* collider, PhysicsShape* solid);

	void resolveVerticalCollisions(PhysicsShape* collider);

	void resolveVerticalCollision(PhysicsShape* collider, PhysicsShape* solid);

	void updateVerticalContacts(PhysicsShape* collider, PhysicsShape* solid);

	std::vector<PhysicsShape*> all_;

	std::vector<PhysicsShape*> solids_;

	std::vector<PhysicsShape*> colliders_;

	float gravity_;

};

#endif