#ifndef GAMERENDERER_H
#define GAMERENDERER_H

#include <vector>

#include "engine/graphics/glm.h"
#include "engine/graphics/renderer.h"
#include "engine/graphics/fbo.h"

class DrawObject;

class GameRenderer : public Renderer {

public:

	static const int DRAWBUFFER_GUI = 2;
	static const int DRAWBUFFER_BACKDROP = 3;

	GameRenderer();

protected:

	void render(GraphicsWorld& world);

private:

	FBO game_buffer_;
	FBO cast_buffer_;
	FBO rays_buffer_;

	glm::mat4 projection_matrix_;

	void createCastBuffer(GraphicsWorld& world, const std::vector<DrawObject*>& objects);

	void renderRays(GraphicsWorld& world);

};

#endif