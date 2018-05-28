#ifndef LIGHT_H
#define LIGHT_H

#include "graphicsobject.h"

class Light : public GraphicsObject {

public:

	Light(int radius);

	void setColor(const glm::vec3& color);

	glm::vec3 getColor() const;

	void setRadius(int radius);

	int getRadius() const;

	void bufferData(const glm::vec2& view, float * buffer) const;

private: 

	int radius_;

	glm::vec3 color_;

};

class RayLight : public GraphicsObject {

public : 

	RayLight();

	glm::vec3 getColor() const;

	GLfloat getAlpha() const;

	void setColor(const glm::vec3& color);

	void setAlpha(GLfloat alpha);

private:

	glm::vec3 color_;

	GLfloat alpha_;

};

#endif