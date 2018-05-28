#ifndef SHAPE_H
#define SHAPE_H

#include "vec2f.h"

class Rectangle;
class Circle;

class Shape {

public:

	static const int RECTANGLE = 0;

	static const int CIRCLE = 1;

	int getType() const;

	void setPosition(float x, float y);

	void setPosition(const vec2f& position);

	vec2f getPosition() const;

	float getX() const;

	float getY() const;

	void setX(float x);

	void setY(float y);

	void alignLeft(float x);

	void alignRight(float x);

	void alignTop(float y);

	void alignBottom(float y);

	bool collides(const Shape& other) const;

	virtual float getMinX() const = 0;

	virtual float getMaxX() const = 0;

	virtual float getMinY() const = 0;

	virtual float getMaxY() const = 0;

	virtual bool contains(const vec2f& point) const = 0;

	virtual bool contains(float x, float y) const = 0;

	virtual Shape* clone() const = 0;

	virtual ~Shape() = default;

protected:

	Shape(int type) : type_(type) {};

private:

	vec2f position_;

	int type_;

	static bool rectangle_rectangle_collision(const Rectangle& a, const Rectangle& b);

	static bool rectangle_circle_collision(const Rectangle& a, const Circle& b);

	static bool	circle_circle_collision(const Circle& a, const Circle& b);

};

#endif