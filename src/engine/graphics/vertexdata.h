#ifndef VERTEXDATA_H
#define VERTEXDATA_H

#include "GL/glew.h"
#include "glm.h"
#include "graphicsboundingbox.h"
#include <vector>

class Shader;
class Texture;

class VertexData {

public:
	
	VertexData(unsigned int vertex_count, unsigned int index_count = 0);

	void setVertex(unsigned int index, float x, float y);

	void setVertex(unsigned int index, const glm::vec2& vertex);

	glm::vec2 getVertex(unsigned int index) const; 

	void setCenteredRectangleVertices(int width, int height, int offset = 0);

	void setRectangleVertices(int dx, int dy, int width, int height, int offset = 0);

	void setNormalCoordinate(unsigned int index, float x, float y);

	void setNormalCoordinate(unsigned int index, const glm::vec2& normal_coordinate);

	glm::vec2 getNormalCoordinate(unsigned int index) const;

	void setTextureCoordinate(unsigned int index, float x, float y);

	void setTextureCoordinate(unsigned int index, const glm::vec2& texture_coordinate);

	void setRectangleTextureCoordinates(const Texture& texture, int offset = 0);

	glm::vec2 getTextureCoordinate(unsigned int index) const;

	void setIndex(unsigned int index, GLushort i);

	void setRectangleIndices(int offset = 0, GLushort vertex_offset = 0);

	GLushort getIndex(unsigned int index) const;

	int getVertexCount() const;

	int getIndexCount() const;

	void draw(Shader& shader) const;

	GLuint getType() const;

	void setType(GLuint type);

	GraphicsBoundingBox generateBoundingBox() const;

private:

	GLuint type_;

	std::vector<glm::vec2> vertices_;

	std::vector<glm::vec2> texture_coordinates_;

	std::vector<glm::vec2> normal_coordinates_;

	std::vector<GLushort> indices_;

	void bindVertexAttrib(int handler) const;

	void bindTextureCoordinateAttrib(int handler) const;

};

#endif