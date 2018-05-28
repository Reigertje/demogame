#include "physicsshape.h"

Rectangle PhysicsShape::getRectangle() const {
	return rectangle_;
}

vec2f PhysicsShape::getPosition() const {
	return rectangle_.getPosition();
}

float PhysicsShape::getX() const {
	return rectangle_.getX();
}

float PhysicsShape::getY() const {
	return rectangle_.getY();
}

vec2f PhysicsShape::getVelocity() const {
	return velocity_;
}

float PhysicsShape::getHorizontalVelocity() const {
	return velocity_.x;
}

float PhysicsShape::getVerticalVelocity() const {
	return velocity_.y;
}

vec2f PhysicsShape::getAcceleration() const {
	return acceleration_;
}

float PhysicsShape::getHorizontalAcceleration() const {
	return acceleration_.x;
}

float PhysicsShape::getVerticalAcceleration() const {
	return acceleration_.y;
}

float PhysicsShape::getMinX() const {
	return rectangle_.getMinX();
}

float PhysicsShape::getMaxX() const {
	return rectangle_.getMaxX();
}
 
float PhysicsShape::getMinY() const {
	return rectangle_.getMinY();
}

float PhysicsShape::getMaxY() const {
	return rectangle_.getMaxY();
}

int PhysicsShape::getType() const {
	return type_;
}

bool PhysicsShape::isHanging() const {
	return is_hanging_;
}

bool PhysicsShape::canHang() const {
	return can_hang_;
}

bool PhysicsShape::canHangOn() const {
	return can_hang_on_;
}

void PhysicsShape::setPosition(float x, float y) {
	rectangle_.setPosition(x, y);
}

void PhysicsShape::setPosition(const vec2f& position) {
	rectangle_.setPosition(position);
}

void PhysicsShape::setX(float x) {
	rectangle_.setX(x);
}

void PhysicsShape::setY(float y) {
	rectangle_.setY(y);
}

void PhysicsShape::alignLeft(float x) {
	rectangle_.alignLeft(x);
}

void PhysicsShape::alignRight(float x) {
	rectangle_.alignRight(x);
}

void PhysicsShape::alignTop(float y) {
	rectangle_.alignTop(y);
}

void PhysicsShape::alignBottom(float y) {
	rectangle_.alignBottom(y);
}

void PhysicsShape::setVelocity(float x, float y) {
	velocity_.set(x, y);	
}

void PhysicsShape::setVelocity(const vec2f& velocity) {
	velocity_ = velocity;
}

void PhysicsShape::setHorizontalVelocity(float x) {
	velocity_.x = x;
}

void PhysicsShape::setVerticalVelocity(float y) {
	velocity_.y = y;
}

void PhysicsShape::setAcceleration(float x, float y) {
	acceleration_.set(x, y);
}

void PhysicsShape::setAcceleration(const vec2f& acceleration) {
	acceleration_ = acceleration;
}

void PhysicsShape::setHorizontalAcceleration(float x) {
	acceleration_.x = x;
}

void PhysicsShape::setVerticalAcceleration(float y) {
	acceleration_.y = y;
}

void PhysicsShape::setCanHang(bool can_hang) {
	can_hang_ = can_hang;
}

void PhysicsShape::setCanHangOn(bool can_hang_on) {
	can_hang_on_ = can_hang_on;
}


bool PhysicsShape::hasTopContact() const {
	return top_contact_;
}

bool PhysicsShape::hasBottomContact() const {
	return bottom_contact_;
}

bool PhysicsShape::hasLeftContact() const {
	return left_contact_;
}

bool PhysicsShape::hasRightContact() const {
	return right_contact_;
}

bool PhysicsShape::collides(const PhysicsShape& other) const {
	return rectangle_.collides(other.rectangle_);
}

void PhysicsShape::setGravityFactor(float gravity_factor) {
	gravity_factor_ = gravity_factor;
}

float PhysicsShape::getGravityFactor() const {
	return gravity_factor_;
}

void PhysicsShape::setTopContact(bool top_contact) {
	top_contact_ = top_contact;
}

void PhysicsShape::setBottomContact(bool bottom_contact) {
	bottom_contact_ = bottom_contact;
}

void PhysicsShape::setLeftContact(bool left_contact) {
	left_contact_ = left_contact;
}

void PhysicsShape::setRightContact(bool right_contact) {
	right_contact_ = right_contact;
}

void PhysicsShape::setIsHanging(bool is_hanging) {
	is_hanging_ = is_hanging;
}

void PhysicsShape::releaseHang() {
	is_hanging_ = false;
}

void PhysicsShape::resetContacts() {
	top_contact_ = bottom_contact_ = left_contact_ = right_contact_ = false;
}

vec2f PhysicsShape::getAccelerationWithGravity(float gravity) {
	return vec2f(acceleration_.x, acceleration_.y + (isHanging() ? 0 : gravity * gravity_factor_));
}

void PhysicsShape::updateVelocity(float gravity, float dt) {
	velocity_ += getAccelerationWithGravity(gravity) * dt;
}

void PhysicsShape::updateHorizontalPosition(float gravity, float dt) {
	if (isHanging()) return;
	vec2f addPos = (velocity_ + getAccelerationWithGravity(gravity) * dt / 2.0f) * dt;
	setPosition(getPosition().x + addPos.x, getPosition().y);
}

void PhysicsShape::updateVerticalPosition(float gravity, float dt) {
	if (isHanging()) return;
	vec2f addPos = (velocity_ + getAccelerationWithGravity(gravity) * dt / 2.0f) * dt;
	setPosition(getPosition().x, getPosition().y + addPos.y);
}