#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

class Rectangle : public Shape {

public:

	Rectangle(float half_width, float half_height) : Shape(RECTANGLE), half_width_(half_width), half_height_(half_height) {}

	float getHalfWidth() const;

	float getHalfHeight() const;

	float getWidth() const;

	float getHeight() const;

	void setHalfWidth(float half_width);

	void setHalfHeight(float half_height);

	void setWidth(float width);

	void setHeight(float height);

	float getMinX() const;

	float getMaxX() const;

	float getMinY() const;

	float getMaxY() const;

	bool contains(const vec2f& point) const;

	bool contains(float x, float y) const;

	Shape* clone() const;

private:

	float half_width_;
	
	float half_height_;

};

#endif