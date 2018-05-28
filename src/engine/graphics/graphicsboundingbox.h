#ifndef GRAPHICSBOUNDINGBOX_H
#define GRAPHICSBOUNDINGBOX_H

#include "glm.h"

class GraphicsBoundingBox {

public:

	GraphicsBoundingBox() : p0_(-9999999.0f, - 9999999.0f), p1_(9999999.0f, 9999999.0f) {};

	GraphicsBoundingBox(const glm::vec2& p0, const glm::vec2& p1) : p0_(p0), p1_(p1) {};

	GraphicsBoundingBox(float radius) : p0_(-radius, -radius), p1_(radius, radius) {};

	glm::vec2 getP0() const;

	glm::vec2 getP1() const;

	void setP0(const glm::vec2& p0);

	void setP1(const glm::vec2& p1);

	void transform(const glm::mat4& t);

	void translate(const glm::vec2& t);

	bool isProjectedOnScreen() const;

	bool intersects(const GraphicsBoundingBox& other) const;

private:

	glm::vec2 p0_;

	glm::vec2 p1_;

};

#endif