#include "rectangle.h"

float Rectangle::getHalfWidth() const {
	return half_width_;
}

float Rectangle::getHalfHeight() const {
	return half_height_;
}

float Rectangle::getWidth() const {
	return half_width_ * 2;
}

float Rectangle::getHeight() const {
	return half_height_ * 2;
}

void Rectangle::setHalfWidth(float half_width) {
	half_width_ = half_width;
}

void Rectangle::setHalfHeight(float half_height) {
	half_height_ = half_height;
}

void Rectangle::setWidth(float width) {
	setHalfWidth(width / 2.0f);
}

void Rectangle::setHeight(float height) {
	setHalfHeight(height / 2.0f);
}

float Rectangle::getMinX() const {
	return getX() - half_width_;
}

float Rectangle::getMaxX() const {
	return getX() + half_width_;
}

float Rectangle::getMinY() const {
	return getY() - half_height_;
}

float Rectangle::getMaxY() const {
	return getY() + half_height_;
}


bool Rectangle::contains(const vec2f& point) const {
	return contains(point.x, point.y);
}

bool Rectangle::contains(float x, float y) const {
	return getMinX() < x && getMaxX() > x &&
		   getMinY() < y && getMaxY() > y;
}

Shape* Rectangle::clone() const {
	return new Rectangle(*this);
}