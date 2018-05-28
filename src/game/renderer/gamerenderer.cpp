#include "gamerenderer.h"

#include "game/game.h"
#include "engine/graphics/fbo.h"

#include "engine/graphics/light.h"
#include "engine/graphics/graphicsworld.h"
#include "engine/graphics/drawobject.h"
#include "engine/graphics/shader.h"

GameRenderer::GameRenderer() : 
	game_buffer_(Game::getInstance().getResolution().x, Game::getInstance().getResolution().y),
	rays_buffer_(Game::getInstance().getResolution().x / 2, Game::getInstance().getResolution().y  / 2, true),
	cast_buffer_(Game::getInstance().getResolution().x, Game::getInstance().getResolution().y)
{
	projection_matrix_ = glm::ortho(0.0f, (float)Game::getInstance().getResolution().x, 0.0f, (float)Game::getInstance().getResolution().y, 0.0f, -4000.0f);
}

void GameRenderer::createCastBuffer(GraphicsWorld& world, const std::vector<DrawObject*>& objects) {

	Shader* plain_shader = Game::getInstance().getResources().findShader("plain");
	world.setLightEnabled(false);
	world.setCameraEnabled(true);
	cast_buffer_.bind();

	plain_shader->bind();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

	for (DrawObject* drawobject : objects) {
		drawobject->draw(world, projection_matrix_, *plain_shader);
	}
}

void GameRenderer::renderRays(GraphicsWorld& world) {

	std::vector<RayLight*> ray_lights = world.getRayLights();
	world.setLightEnabled(false);
	world.setCameraEnabled(false);

	Shader* rays_shader = Game::getInstance().getResources().findShader("rays");

	rays_shader->bind();

	rays_buffer_.bind();

	//glUniform2f(glGetUniformLocation(data.shader->getId(), "light_xy"), 0.25f, 0.75f);


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < ray_lights.size(); ++i) {
		world.submitRayLight(ray_lights[i], *rays_shader);
		cast_buffer_.draw(*rays_shader, true);
	}

}

void GameRenderer::render(GraphicsWorld& world) {

	std::vector<DrawObject*> defaultDrawObjects = world.getDrawObjectBuffer(GraphicsWorld::DEFAULT_BUFFER_INDEX);
	std::vector<DrawObject*> backdropDrawObjects = world.getDrawObjectBuffer(DRAWBUFFER_BACKDROP);
	std::vector<DrawObject*> guiDrawObjects = world.getDrawObjectBuffer(DRAWBUFFER_GUI);

	createCastBuffer(world, defaultDrawObjects);
	renderRays(world);

	vec2f window_size = Game::getInstance().getWindowSize();

	game_buffer_.bind();

	Shader* default_shader = Game::getInstance().getResources().findShader("default");
	Shader* fbo_default_shader = Game::getInstance().getResources().findShader("fbo_default"); 

	default_shader->bind();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	world.setLightEnabled(false);
	world.setCameraEnabled(true);

	for (DrawObject* drawObject : backdropDrawObjects) {
		drawObject->draw(world, projection_matrix_, *default_shader);
	}

	world.setLightEnabled(true);
	for (DrawObject* drawObject : defaultDrawObjects) {
		drawObject->draw(world, projection_matrix_, *default_shader);
	}

	FBO::bindScreen(window_size.x, window_size.y);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBlendFunc(GL_ONE, GL_ZERO); 
	fbo_default_shader->bind(); 

	game_buffer_.draw(*fbo_default_shader, true);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	rays_buffer_.draw(*fbo_default_shader, true);

	default_shader->bind();

	world.setLightEnabled(false);
	world.setCameraEnabled(false);
	glBlendFunc(GL_ONE, GL_ZERO); 

	for (DrawObject* d : guiDrawObjects) {
		d->draw(world, projection_matrix_, *default_shader);
	}

}