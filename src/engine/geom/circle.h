#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class Circle : public Shape {

public:

	Circle(float radius) : Shape(CIRCLE), radius_(radius) {}

	float getRadius() const;

	void setRadius(float radius);

	float getMinX() const;

	float getMaxX() const;

	float getMinY() const;

	float getMaxY() const;

	bool contains(const vec2f& point) const;

	bool contains(float x, float y) const;

	Shape* clone() const;

private:

	float radius_;


};

#endif