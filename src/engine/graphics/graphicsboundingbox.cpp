#include "graphicsboundingbox.h"

glm::vec2 GraphicsBoundingBox::getP0() const {
	return p0_;
}

glm::vec2 GraphicsBoundingBox::getP1() const {
	return p1_;
}

void GraphicsBoundingBox::setP0(const glm::vec2& p0) {
	p0_ = p0;
}

void GraphicsBoundingBox::setP1(const glm::vec2& p1) {
	p1_ = p1;
}

void GraphicsBoundingBox::transform(const glm::mat4& t) {
	p0_ = glm::vec2(t * glm::vec4(p0_, 0.0f, 1.0f));
	p1_ = glm::vec2(t * glm::vec4(p1_, 0.0f, 1.0f));

	// Fix voor horizontal mirror transform
	// TODO might also be necessary for y
	if (p0_.x > p1_.x) {
		float tmp = p0_.x;
		p0_.x = p1_.x;
		p1_.x = tmp;
	}

}

void GraphicsBoundingBox::translate(const glm::vec2& t) {
	p0_ += t;
	p1_ += t;
}

// Assumes box is already projected!
bool GraphicsBoundingBox::isProjectedOnScreen() const {
	return !(p1_.x < -1.0f || p0_.x > 1.0f || p0_.y > 1.0f || p1_.y < -1.0f);
}

bool GraphicsBoundingBox::intersects(const GraphicsBoundingBox& other) const {
	return p0_.x < other.p1_.x && p1_.x > other.p0_.x &&
		   p0_.y < other.p1_.y && p1_.y > other.p0_.y;
}