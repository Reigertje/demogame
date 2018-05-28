#include "vertexdata.h"

#include "shader.h"
#include "texture.h"

#include <iostream>

VertexData::VertexData(unsigned int vertex_count, unsigned int index_count) {
	vertices_.resize(vertex_count);
	texture_coordinates_.resize(vertex_count);
	type_ = GL_TRIANGLE_FAN;

	if (index_count) {
		indices_.resize(index_count);
		type_ = GL_TRIANGLES;
	}

}

int VertexData::getVertexCount() const {
	return vertices_.size();
}

int VertexData::getIndexCount() const {
	return indices_.size();
}

void VertexData::setVertex(unsigned int index, float x, float y) {
	setVertex(index, glm::vec2(x, y));
}

void VertexData::setVertex(unsigned int index, const glm::vec2& vertex) {
	vertices_[index] = vertex;
}

void VertexData::setRectangleVertices(int dx, int dy, int width, int height, int offset) {
	int lwidth = width/2;
	int rwidth = width%2 == 0 ? width/2 : width/2 + 1;

	int bheight = height/2;
	int theight = height%2 == 0 ? height/2 : height/2 + 1;

	setVertex(offset + 0, dx - lwidth, dy - bheight);
	setVertex(offset + 1, dx + rwidth, dy - bheight);
	setVertex(offset + 2, dx + rwidth, dy + theight);
	setVertex(offset + 3, dx - lwidth, dy + theight);
}

void VertexData::setCenteredRectangleVertices(int width, int height, int offset) {
	setRectangleVertices(0, 0, width, height, offset);
}

glm::vec2 VertexData::getVertex(unsigned int index) const {
	return vertices_[index];
}

void VertexData::setTextureCoordinate(unsigned int index, float x, float y) {
	setTextureCoordinate(index, glm::vec2(x, y));
}

void VertexData::setTextureCoordinate(unsigned int index, const glm::vec2& texture_coordinate) {
	texture_coordinates_[index] = texture_coordinate;
}

void VertexData::setRectangleTextureCoordinates(const Texture& texture, int offset) {
	setTextureCoordinate(offset + 0, texture.getX0(), texture.getY0());
	setTextureCoordinate(offset + 1, texture.getX1(), texture.getY0());
	setTextureCoordinate(offset + 2, texture.getX1(), texture.getY1());
	setTextureCoordinate(offset + 3, texture.getX0(), texture.getY1());
}

glm::vec2 VertexData::getTextureCoordinate(unsigned int index) const {
	return texture_coordinates_[index];
}

void VertexData::setNormalCoordinate(unsigned int index, float x, float y) {
	setTextureCoordinate(index, glm::vec2(x, y));
}

void VertexData::setNormalCoordinate(unsigned int index, const glm::vec2& normal_coordinate) {
	normal_coordinates_[index] = normal_coordinate;
}

glm::vec2 VertexData::getNormalCoordinate(unsigned int index) const {
	return normal_coordinates_[index];
}

void VertexData::setIndex(unsigned int index, GLushort i) {
	indices_[index] = i;
}

void VertexData::setRectangleIndices(int index_offset, GLushort vertex_offset) {
	setIndex(index_offset + 0, vertex_offset + 0);
	setIndex(index_offset + 1, vertex_offset + 1);
	setIndex(index_offset + 2, vertex_offset + 2);
	setIndex(index_offset + 3, vertex_offset + 0);
	setIndex(index_offset + 4, vertex_offset + 2);
	setIndex(index_offset + 5, vertex_offset + 3);
}

GLushort VertexData::getIndex(unsigned int index) const {
	return indices_[index];
}

GLuint VertexData::getType() const {
	return type_;
}

void VertexData::setType(GLuint type) {
	type_ = type;
}

GraphicsBoundingBox VertexData::generateBoundingBox() const {
	// TODO magic values
	float min_x = 9999999.0f;
	float max_x = -9999999.0f;
	float min_y = 9999999.0f;
	float max_y = -9999999.0f;
	for (int i = 0; i < vertices_.size(); ++i) {
		if (vertices_[i].x < min_x) min_x = vertices_[i].x;
		if (vertices_[i].x > max_x) max_x = vertices_[i].x;
		if (vertices_[i].y < min_y) min_y = vertices_[i].y;
		if (vertices_[i].y > max_y) max_y = vertices_[i].y;
	}

	return GraphicsBoundingBox(glm::vec2(min_x, min_y), glm::vec2(max_x, max_y));
}

void VertexData::draw(Shader& shader) const {
	glVertexAttribPointer(shader.getAttribHandler("vertex_xy"), 2, GL_FLOAT, GL_FALSE, 0, &vertices_[0]);
	glVertexAttribPointer(shader.getAttribHandler("vertex_uv"), 2, GL_FLOAT, GL_FALSE, 0, &texture_coordinates_[0]);
	glVertexAttribPointer(shader.getAttribHandler("vertex_n_uv"), 2, GL_FLOAT, GL_FALSE, 0, &texture_coordinates_[0]);

	if (getIndexCount() > 0) {
		glDrawElements(type_, getIndexCount(), GL_UNSIGNED_SHORT, &indices_[0]);
	} else {
		glDrawArrays(type_, 0, getVertexCount());
	}
}