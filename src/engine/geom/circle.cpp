#include "circle.h"

float Circle::getRadius() const {
	return radius_;
}

void Circle::setRadius(float radius) {
	radius_ = radius;
}

float Circle::getMinX() const {
	return getX() - radius_;
}

float Circle::getMaxX() const {
	return getX() + radius_;
}

float Circle::getMinY() const {
	return getY() - radius_;
}

float Circle::getMaxY() const {
	return getY() + radius_;
}

bool Circle::contains(const vec2f& point) const {
	return contains(point.x, point.y);
}

bool Circle::contains(float x, float y) const {
	return (getX() - x) * (getX() - x) + (getY() - y) * (getY() - y) < radius_ * radius_;
}

Shape* Circle::clone() const {
	return new Circle(*this);
}