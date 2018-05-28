#include "graphicsworld.h"

#include "drawobject.h"
#include "renderer.h"
#include "light.h"
#include "shader.h"

#include <algorithm>

#include "GL/glew.h"

#include "utils/utils.h"

#include <iostream>

GraphicsWorld::GraphicsWorld() : ambient_light_(1.0f, 1.0f, 1.0f), view_bounds_(999999.0f, 999999.0f) {
	renderer_ = 0;
}

void GraphicsWorld::addDrawObject(DrawObject* object) {
	draw_objects_.push_back(object);
}

void GraphicsWorld::removeDrawObject(DrawObject* object) {
	draw_objects_.erase(std::remove(draw_objects_.begin(), draw_objects_.end(), object), draw_objects_.end());
}

void GraphicsWorld::addLight(Light* light) {
	lights_.push_back(light);
}

void GraphicsWorld::removeLight(Light* light) {
	lights_.erase(std::remove(lights_.begin(), lights_.end(), light), lights_.end());
}

void GraphicsWorld::addRayLight(RayLight* light) {
	ray_lights_.push_back(light);
}

void GraphicsWorld::removeRayLight(RayLight* light) {
	ray_lights_.erase(std::remove(ray_lights_.begin(), ray_lights_.end(), light), ray_lights_.end());
}

void GraphicsWorld::setRenderer(Renderer* renderer) {
	renderer_ = renderer;
}

void GraphicsWorld::setCameraPosition(float x, float y) {
	setCameraPosition(glm::vec2(x, y));
}

void GraphicsWorld::setCameraPosition(const glm::vec2& camera_position) {
	camera_position_ = camera_position;
	if (camera_position_.x < camera_offset_.x) camera_position_.x = camera_offset_.x;
	if (camera_position_.y < camera_offset_.y) camera_position_.y = camera_offset_.y;
	if (camera_position_.x > view_bounds_.x - camera_offset_.x) camera_position_.x = view_bounds_.x - camera_offset_.x;
	if (camera_position_.y > view_bounds_.y - camera_offset_.y) camera_position_.y = view_bounds_.y - camera_offset_.y;
}

void GraphicsWorld::setViewBounds(float x, float y) {
	setViewBounds(glm::vec2(x, y));
}

void GraphicsWorld::setViewBounds(const glm::vec2& view_bounds) {
	view_bounds_ = view_bounds;
}

void GraphicsWorld::setLightEnabled(bool light_enabled) {
	light_enabled_ = light_enabled;
}

void GraphicsWorld::setCameraEnabled(bool camera_enabled) {
	camera_enabled_ = camera_enabled;
}

void GraphicsWorld::setAmbientLight(const glm::vec3& ambient_light) {
	ambient_light_ = ambient_light;
}

bool GraphicsWorld::isLightEnabled() const {
	return light_enabled_;
}

bool GraphicsWorld::isCameraEnabled() const {
	return camera_enabled_;
}

glm::vec2 GraphicsWorld::getCameraPosition() const {
	return isCameraEnabled() ? camera_position_ : glm::vec2(0.0f, 0.0f);
}

void GraphicsWorld::setCameraOffset(float x, float y) {
	setCameraOffset(glm::vec2(x, y));
}

void GraphicsWorld::setCameraOffset(const glm::vec2& camera_offset) {
	camera_offset_ = camera_offset;
}

glm::vec2 GraphicsWorld::getCameraOffset() const {
	return isCameraEnabled() ? camera_offset_ : glm::vec2(0.0f, 0.0f);
}

std::vector<DrawObject *> GraphicsWorld::getDrawObjectBuffer(int index) {
	if (index == DEFAULT_BUFFER_INDEX) return default_draw_object_buffer_;
	if (index >= indexed_draw_object_buffers_.size()) return std::vector<DrawObject*>();
	return indexed_draw_object_buffers_[index];
}

std::vector<Light*> GraphicsWorld::getLights() const {
	return lights_;
}

std::vector<RayLight*> GraphicsWorld::getRayLights() const {
	return ray_lights_;
}

glm::vec3 GraphicsWorld::getAmbientLight() const {
	return isLightEnabled() ? ambient_light_ : glm::vec3(1.0f, 1.0f, 1.0f);
}

void GraphicsWorld::submitLights(const GraphicsBoundingBox& area, float depth, Shader& shader) const {
	int count = 0;
	float data[MAX_LIGHTS * 8];

	if (isLightEnabled()) {
		
		for (int i = 0; i < lights_.size() && count < MAX_LIGHTS; ++i) {

			if (depth < lights_[i]->getDepth() || depth - lights_[i]->getDepth() > lights_[i]->getRadius()) continue;

			glm::vec2 radius(lights_[i]->getRadius(), lights_[i]->getRadius());

			GraphicsBoundingBox lightArea = GraphicsBoundingBox(lights_[i]->getPosition() - radius, lights_[i]->getPosition() + radius);
			


			if (area.intersects(lightArea)) {
				std::cout << lights_.size() << std::endl;
				lights_[i]->bufferData(glm::vec2(0, 0), data + count * 8);
				count++;
			}
		}
		//std::cout << count << std::endl;
	}

	glUniform3fv(shader.getUniformHandler("ambient_light"), 1, glm::value_ptr( isLightEnabled() ? getAmbientLight() : glm::vec3(1.0f, 1.0f, 1.0f)));
	glUniform1i(shader.getUniformHandler("light_count"), count);
    glUniform4fv(shader.getUniformHandler("light_data"), count * 2, data);
}

void GraphicsWorld::submitRayLight(RayLight* light, Shader& shader) const {
	glUniform4fv(shader.getUniformHandler("light_rgba"), 1, glm::value_ptr(glm::vec4(light->getColor(), light->getAlpha())));
	glUniform2fv(shader.getUniformHandler("light_xy"), 1, glm::value_ptr(light->getPosition()));
}

void GraphicsWorld::bufferDrawObject(DrawObject* object) {

	if (!object->isVisible()) return;
	if (object->isAllowFilterByPosition()) {
		glm::vec2 dl = object->getPosition() - camera_position_;
		// TODO iets beters dan hardcoded 320x320
		if (dl.x * dl.x + dl.y * dl.y > 320 * 320) return;
	}

	int buffer_index = object->getDrawBufferIndex();
	if (buffer_index == DEFAULT_BUFFER_INDEX) {
		bufferDrawObject(object, default_draw_object_buffer_);
	} else {
		if (buffer_index >= indexed_draw_object_buffers_.size()) {
			indexed_draw_object_buffers_.resize(buffer_index + 1);
		}
		bufferDrawObject(object, indexed_draw_object_buffers_[buffer_index]);
	}
}

void GraphicsWorld::bufferDrawObject(DrawObject* object, std::vector<DrawObject*>& buffer) {
	buffer.push_back(object);
}

void GraphicsWorld::clearDrawObjectBuffers() {
	default_draw_object_buffer_.clear();
	for (int i = 0; i < indexed_draw_object_buffers_.size(); ++i) {
		indexed_draw_object_buffers_.clear();
	}
}


static bool compareDrawObject(DrawObject* a, DrawObject* b) { 
	return b->getDrawLayer() < a->getDrawLayer();
}

void GraphicsWorld::render() {

	clearDrawObjectBuffers();

	for (int i = 0; i < draw_objects_.size(); ++i) {
		bufferDrawObject(draw_objects_[i]);
	}

	std::sort(default_draw_object_buffer_.begin(), default_draw_object_buffer_.end(), compareDrawObject);
	for (int i = 0; i < indexed_draw_object_buffers_.size(); ++i) {
		std::sort(indexed_draw_object_buffers_[i].begin(), indexed_draw_object_buffers_[i].end(), compareDrawObject);
	}

	if (renderer_) {
		renderer_->render(*this);
	}
}