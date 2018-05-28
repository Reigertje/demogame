#include "shape.h"
#include "rectangle.h"
#include "circle.h"

#include <algorithm>

int Shape::getType() const {
	return type_;
}

void Shape::setPosition(float x, float y) {
	position_.set(x, y);
}

void Shape::setPosition(const vec2f& position) {
	position_ = position;
}

vec2f Shape::getPosition() const {
	return position_;
}

float Shape::getX() const {
	return position_.x;
}

float Shape::getY() const {
	return position_.y;
}

void Shape::setX(float x) {
	position_.x = x;
}

void Shape::setY(float y) {
	position_.y = y;
}

void Shape::alignLeft(float x) {
	setX(x + (position_.x - getMinX()));
}

void Shape::alignRight(float x) {
	setX(x - (getMaxX() - position_.x));
}

void Shape::alignBottom(float y) {
	setY(y + (position_.y - getMinY()));
}

void Shape::alignTop(float y) {
	setY(y - (getMaxY() - position_.y));
}

bool Shape::collides(const Shape& other) const {
	if (type_ == RECTANGLE) {
		if (other.getType() == RECTANGLE) {
			return rectangle_rectangle_collision((Rectangle&)*this, (Rectangle&)other); 
		} else {
			return rectangle_circle_collision((Rectangle&)*this, (Circle&)other);
		}
	} else {
		if (other.getType() == RECTANGLE) {
			return rectangle_circle_collision((Rectangle&)other, (Circle&)*this);
		} else {
			return circle_circle_collision((Circle&)*this, (Circle&)other);
		}
	}
}

bool Shape::rectangle_rectangle_collision(const Rectangle& a, const Rectangle& b) {
	return a.getMinX() < b.getMaxX() && a.getMaxX() > b.getMinX()  &&
		   a.getMinY() < b.getMaxY() && a.getMaxY() > b.getMinY(); 
}

bool Shape::rectangle_circle_collision(const Rectangle& a, const Circle& b) {
	return a.contains(b.getPosition()) ||
		   b.contains(std::max(std::min(b.getX(), a.getMaxX()), a.getMinX()), 
		   			std::max(std::min(b.getY(), a.getMaxY()), a.getMinY()));
}

bool Shape::circle_circle_collision(const Circle& a, const Circle& b) {
	return (a.getX() - b.getX()) * (a.getX() - b.getX()) + (a.getY() - b.getY()) * (a.getY() - b.getY()) < a.getRadius() * a.getRadius() + b.getRadius() * b.getRadius();
}