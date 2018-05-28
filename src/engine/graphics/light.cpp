#include "light.h"

Light::Light(int radius) {
	setRadius(radius);
	setColor(glm::vec3(1.0f, 1.0f, 1.0f));
}

void Light::setColor(const glm::vec3& color) {
	color_ = color;
}

glm::vec3 Light::getColor() const {
	return color_;
}

void Light::setRadius(int radius) {
	radius_ = radius;
}

int Light::getRadius() const {
	return radius_;
}

void Light::bufferData(const glm::vec2& view, float * buffer) const {
	buffer[0] = getPosition().x + view.x;
	buffer[1] = getPosition().y + view.y;
	buffer[2] = getDepth();
	buffer[3] = 0.0f; // value is ignored
	
	buffer[4] = getColor().r;
	buffer[5] = getColor().g;
	buffer[6] = getColor().b;
	buffer[7] = getRadius();
}


RayLight::RayLight() {
	setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	alpha_ = 1.0;
};


void RayLight::setColor(const glm::vec3& color) {
	color_ = color;
}

void RayLight::setAlpha(GLfloat alpha) {
	alpha_ = alpha;
}

glm::vec3 RayLight::getColor() const {
	return color_;
}

GLfloat RayLight::getAlpha() const {
	return alpha_;
}