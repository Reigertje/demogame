#ifndef PHYSICSSHAPE_H
#define PHYSICSSHAPE_H

#include "engine/geom/rectangle.h"
#include "engine/geom/vec2f.h"

class PhysicsWorld;

class PhysicsShape {

friend class PhysicsWorld;

public:

	static const int NONE = 0;
	static const int SOLID = 1;
	static const int COLLIDER = 2;
	static const int BOTH = 3;

	PhysicsShape(int type, float half_width, float half_height) : type_(type), rectangle_(half_width, half_height), gravity_factor_(1.0f) {
		resetContacts();
		is_hanging_ = can_hang_on_ = can_hang_ = false;
	};

	PhysicsShape(int type, const Rectangle & rectangle) : type_(type), rectangle_(rectangle), gravity_factor_(1.0f) {
		resetContacts();
		is_hanging_ = can_hang_on_ = can_hang_ = false;
	};

	Rectangle getRectangle() const;

	vec2f getPosition() const;

	float getX() const;

	float getY() const;

	vec2f getVelocity() const;

	float getHorizontalVelocity() const;

	float getVerticalVelocity() const;

	vec2f getAcceleration() const;

	float getHorizontalAcceleration() const;

	float getVerticalAcceleration() const;

	float getMinX() const;

	float getMaxX() const;
	 
	float getMinY() const;

	float getMaxY() const;

	int getType() const;

	bool isHanging() const;

	bool canHang() const;

	bool canHangOn() const;

	void setPosition(float x, float y);

	void setPosition(const vec2f& position);

	void setX(float x);

	void setY(float y);

	void alignLeft(float x);

	void alignRight(float x);

	void alignTop(float y);

	void alignBottom(float y);

	void setVelocity(float x, float y);

	void setVelocity(const vec2f& velocity);

	void setHorizontalVelocity(float x);

	void setVerticalVelocity(float y);

	void setAcceleration(float x, float y);

	void setAcceleration(const vec2f& acceleration);

	void setHorizontalAcceleration(float x);

	void setVerticalAcceleration(float y);

	void setCanHang(bool can_hang);

	void setCanHangOn(bool can_hang_on);

	bool hasTopContact() const;

	bool hasBottomContact() const;

	bool hasLeftContact() const;

	bool hasRightContact() const;

	bool collides(const PhysicsShape& other) const;

	void setGravityFactor(float gravity_factor);

	float getGravityFactor() const;

	void releaseHang();

private:

	void setIsHanging(bool is_hanging);

	void setTopContact(bool top_contact);

	void setBottomContact(bool bottom_contact);

	void setLeftContact(bool left_contact);

	void setRightContact(bool right_contact);

	void resetContacts();

	vec2f getAccelerationWithGravity(float gravity);

	void updateVelocity(float gravity, float dt);

	void updateHorizontalPosition(float gravity, float dt);

	void updateVerticalPosition(float gravity, float dt);

	int type_;

	Rectangle rectangle_;

	vec2f velocity_;

	vec2f acceleration_;

	float gravity_factor_;

	bool top_contact_;

	bool bottom_contact_;

	bool left_contact_;

	bool right_contact_;

	bool is_hanging_;

	// Can THIS object hang to another?
	bool can_hang_;

	// Can another object hang on THIS object
	bool can_hang_on_;	

};

#endif
