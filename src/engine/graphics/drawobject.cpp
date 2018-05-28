#include "drawobject.h"

#include "graphicsworld.h"

#include <cmath>
#include <iostream>

DrawObject::DrawObject() : mix_color_(0.0f, 0.0f, 0.0f, 0.0f), multiply_color_(1.0f, 1.0f, 1.0f, 1.0f) {
	draw_layer_ = 0.0f;
	draw_buffer_index_ = GraphicsWorld::DEFAULT_BUFFER_INDEX;
	mirror_horizontal_ = false;
	allow_filter_by_position_ = false;
}

void DrawObject::setDrawLayer(float draw_layer) {
	draw_layer_ = draw_layer;
}

float DrawObject::getDrawLayer() const {
	return draw_layer_;
}

int DrawObject::getDrawBufferIndex() const {
	return draw_buffer_index_;
}

void DrawObject::setDrawBufferIndex(int draw_buffer_index) {
	draw_buffer_index_ = draw_buffer_index;
}

bool DrawObject::isAllowFilterByPosition() const {
	return allow_filter_by_position_;
}

void DrawObject::setAllowFilterByPosition(bool allow_filter_by_position) {
	allow_filter_by_position_ = allow_filter_by_position;
}

bool DrawObject::isMirrorHorizontal() const {
	return mirror_horizontal_;
}

void DrawObject::setMirrorHorizontal(bool mirror_horizontal) {
	mirror_horizontal_ = mirror_horizontal;
}

void DrawObject::setMultiplyColor(const glm::vec4& multiply_color) {
	multiply_color_ = multiply_color;
}

void DrawObject::setMixColor(const glm::vec4& mix_color) {
	mix_color_ = mix_color;
}

glm::vec4 DrawObject::getMultiplyColor() {
	return multiply_color_;
}

glm::vec4 DrawObject::getMixColor() {
	return mix_color_;
}

glm::mat4 DrawObject::calculateModelMatrix() {
	glm::mat4 position_matrix = glm::translate(glm::vec3(getPosition(), 0.0f));
	glm::mat4 orientation_matrix = glm::scale(glm::vec3(isMirrorHorizontal() ? -1.0f : 1.0f, 1.0f, 1.0f));

	position_matrix[3][0] = std::round(position_matrix[3][0]);
	position_matrix[3][1] = std::round(position_matrix[3][1]);

	return position_matrix * orientation_matrix;
}

glm::mat4 DrawObject::calculateModelViewProjectionMatrix(const GraphicsWorld& world, const glm::mat4& projection) {

/*
	glm::vec2 p0(0.0f, 215.3f);
	glm::vec2 p1(0.0f)
*/
/*
	glm::vec2 camera_diff(getPosition()-world.getCameraPosition() * getCame);*/


	glm::mat4 camera_matrix = glm::translate(glm::vec3(-world.getCameraPosition(), 0.0f));
	glm::mat4 camera_offset = glm::translate(glm::vec3(world.getCameraOffset(), 0.0f));

	glm::mat4 position_matrix = glm::translate(glm::vec3((getPosition() - world.getCameraPosition()) * getCameraFactor() + getCameraOffset(), 0.0f));
	glm::mat4 orientation_matrix = glm::scale(glm::vec3(isMirrorHorizontal() ? -1.0f : 1.0f, 1.0f, 1.0f));

	glm::mat4 model_view = position_matrix * orientation_matrix;

	model_view[3][0] =	std::floor(model_view[3][0] + 0.5);
	model_view[3][1] = std::floor(model_view[3][1] + 0.5);

	if (getPosition().y == 90) {
/*
		std::cout << "pf" << std::endl;

		std::cout << getPosition().x << std::endl;
		std::cout << world.getCameraPosition().x << std::endl;

		std::cout << model_view[3][0] << std::endl;*/
	} 

/*	if (getPosition().x == 350) {
		std::cout << model_view[3][0] << std::endl;
	}*/

	/*if (getPosition().x == 350) {
		std::cout << model_view[3][0] << std::endl;
	}*/

/*	model_view[3][0] = std::floor(model_view[3][0]);
	model_view[3][1] = std::floor(model_view[3][1]);*/

	// TODO parallax

	return projection * camera_offset * model_view;
}