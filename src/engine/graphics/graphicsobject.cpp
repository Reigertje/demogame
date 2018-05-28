#include "graphicsobject.h"

glm::vec2 GraphicsObject::getPosition() const {
	return position_;
}

glm::vec2 GraphicsObject::getCameraFactor() const {
	return camera_factor_;
}

glm::vec2 GraphicsObject::getCameraOffset() const {
	return camera_offset_;
}

GLfloat GraphicsObject::getDepth() const {
	return depth_;
}

GraphicsBoundingBox GraphicsObject::getBoundingBox() const {
	return bounding_box_;
}

bool GraphicsObject::isVisible() const {
	return visible_;
}

void GraphicsObject::setPosition(float x, float y) {
	setPosition(glm::vec2(x, y));
}

void GraphicsObject::setPosition(const glm::vec2& position) {
	position_ = position;
}

void GraphicsObject::setCameraFactor(float x, float y) {
	setCameraFactor(glm::vec2(x, y));
}

void GraphicsObject::setCameraFactor(const glm::vec2& camera_factor) {
	camera_factor_ = camera_factor;
}

void GraphicsObject::setCameraOffset(float x, float y) {
	setCameraOffset(glm::vec2(x, y));
}

void GraphicsObject::setCameraOffset(const glm::vec2& camera_offset) {
	camera_offset_ = camera_offset;
}

void GraphicsObject::setDepth(GLfloat depth) {
	depth_ = depth;
}

void GraphicsObject::setBoundingBox(const GraphicsBoundingBox& bounding_box) {
	bounding_box_ = bounding_box;
}

void GraphicsObject::setVisible(bool visible) {
	visible_ = visible;
}