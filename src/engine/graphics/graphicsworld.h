#ifndef GRAPHICSWORLD_H
#define GRAPHICSWORLD_H

#include <vector>
#include "glm.h"

class Light;
class RayLight;
class DrawObject;
class Renderer;
class GraphicsBoundingBox;
class Shader;

class GraphicsWorld {

public:

	static const int DEFAULT_BUFFER_INDEX = -1;

	static const int MAX_LIGHTS = 24;

	GraphicsWorld();

	void addDrawObject(DrawObject* object);

	void removeDrawObject(DrawObject* object);

	void addLight(Light* light);

	void removeLight(Light* light);

	void addRayLight(RayLight* light);

	void removeRayLight(RayLight* light);

	void render();

	void setLightEnabled(bool light_enabled);

	void setCameraEnabled(bool camera_enabled);

	void setCameraPosition(float x, float y);

	void setCameraPosition(const glm::vec2& camera_position);

	void setViewBounds(float x, float y);

	void setViewBounds(const glm::vec2& camera_max);

	void setAmbientLight(const glm::vec3& ambient_light);

	bool isLightEnabled() const;

	bool isCameraEnabled() const;

	glm::vec2 getCameraPosition() const;

	void setCameraOffset(float x, float y);

	void setCameraOffset(const glm::vec2& camera_offset);

	glm::vec2 getCameraOffset() const;

	glm::vec3 getAmbientLight() const;

	void setRenderer(Renderer* renderer);

	std::vector<DrawObject*> getDrawObjectBuffer(int index);

	std::vector<Light*> getLights() const;

	std::vector<RayLight*> getRayLights() const;

	void submitLights(const GraphicsBoundingBox& area, float depth, Shader& shader) const;

	void submitRayLight(RayLight* light, Shader& shader) const;

private:

	bool light_enabled_;

	bool camera_enabled_;

	void bufferDrawObject(DrawObject* object);

	void bufferDrawObject(DrawObject* object, std::vector<DrawObject*>& buffer);

	void clearDrawObjectBuffers();

	std::vector<DrawObject*> draw_objects_;

	std::vector<Light*> lights_;

	std::vector<RayLight*> ray_lights_;

	std::vector<DrawObject*> default_draw_object_buffer_;

	std::vector<std::vector<DrawObject*>> indexed_draw_object_buffers_;

	glm::vec2 camera_position_;

	glm::vec2 view_bounds_;

	glm::vec2 camera_offset_;

	glm::vec3 ambient_light_;

	Renderer* renderer_;

};

#endif