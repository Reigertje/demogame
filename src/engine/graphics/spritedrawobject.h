#ifndef SPRITEDRAWOBJECT_H
#define SPRITEDRAWOBJECT_H

#include "drawobject.h"
#include "vertexdata.h"
#include "sprite.h"

class SpriteDrawObject : public DrawObject {

public:

	SpriteDrawObject(const Sprite& sprite);

	SpriteDrawObject(const Sprite& sprite, int width, int height);

	SpriteDrawObject(const Sprite& sprite, int dx, int dy, int width, int height);

	SpriteDrawObject(const Sprite& sprite, const VertexData& vertexdata);

	void draw(const GraphicsWorld& world, const glm::mat4& projection, Shader& shader);

private:

	void initialize(const Sprite& sprite, int dx, int dy, int width, int height);

	VertexData vertex_data_;

	Sprite sprite_;

};

#endif