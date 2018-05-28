#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include "glm.h"
#include "GL/glew.h"
#include "graphicsboundingbox.h"

class GraphicsObject {

public:

	glm::vec2 getPosition() const;

	glm::vec2 getCameraFactor() const;

	glm::vec2 getCameraOffset() const;

	GraphicsBoundingBox getBoundingBox() const;

	GLfloat getDepth() const;

	bool isVisible() const;

	void setPosition(float x, float y);

	void setPosition(const glm::vec2& position);

	void setCameraFactor(float x, float y);

	void setCameraFactor(const glm::vec2& camera_factor);

	void setCameraOffset(float x, float y);

	void setCameraOffset(const glm::vec2& camera_offset);

	void setDepth(GLfloat depth);

	void setVisible(bool visible);

protected:

	GraphicsObject() : camera_factor_(1.0f, 1.0f), depth_(0), visible_(true) {}	

	void setBoundingBox(const GraphicsBoundingBox& bounding_box);

private:

	bool visible_;

	glm::vec2 position_;
	
	glm::vec2 camera_factor_;
	
	glm::vec2 camera_offset_;

	GraphicsBoundingBox bounding_box_;

	GLfloat depth_;

};

#endif