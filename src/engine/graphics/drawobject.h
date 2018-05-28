#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include "graphicsobject.h"

class Shader;
class GraphicsWorld;

class DrawObject : public GraphicsObject {

public:

	DrawObject();

	virtual ~DrawObject() = default;

	virtual void draw(const GraphicsWorld& world, const glm::mat4& projection, Shader& shader) = 0;

	void setDrawLayer(float draw_layer);

	virtual float getDrawLayer() const;

	int getDrawBufferIndex() const;

	bool isMirrorHorizontal() const;

	bool isAllowFilterByPosition() const;

	void setMirrorHorizontal(bool mirror_horizontal);

	void setAllowFilterByPosition(bool allow_filter_by_position);

	void setDrawBufferIndex(int draw_buffer_index);

	void setMultiplyColor(const glm::vec4& multiply_color);

	void setMixColor(const glm::vec4& mix_color);

	glm::vec4 getMultiplyColor();

	glm::vec4 getMixColor();

	virtual void step(float dt) {};

protected:

	glm::mat4 calculateModelViewProjectionMatrix(const GraphicsWorld& world, const glm::mat4& projection);

	glm::mat4 calculateModelMatrix();

private:

	glm::vec4 multiply_color_;

	glm::vec4 mix_color_;

	float draw_layer_;

	int draw_buffer_index_;

	bool mirror_horizontal_;

	bool allow_filter_by_position_;

};

#endif