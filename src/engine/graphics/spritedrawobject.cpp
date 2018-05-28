#include "spritedrawobject.h"
#include "graphicsworld.h"
#include "shader.h"

#include <iostream>

SpriteDrawObject::SpriteDrawObject(const Sprite& sprite, int width, int height) : vertex_data_(4) {
	initialize(sprite, 0, 0, width, height);
}

SpriteDrawObject::SpriteDrawObject(const Sprite& sprite) : vertex_data_(4) {
	initialize(sprite, 0, 0, sprite.getPixelWidth(), sprite.getPixelHeight());
}

SpriteDrawObject::SpriteDrawObject(const Sprite& sprite, int dx, int dy, int width, int height) : vertex_data_(4) {
	initialize(sprite, dx, dy, width, height);
}

SpriteDrawObject::SpriteDrawObject(const Sprite& sprite, const VertexData& vertexdata) : vertex_data_(vertexdata) {
	sprite_ = sprite;

	setBoundingBox(vertex_data_.generateBoundingBox());
}

void SpriteDrawObject::initialize(const Sprite& sprite, int dx, int dy, int width, int height) {
	sprite_ = sprite;
/*
	int lwidth = width/2;
	int rwidth = width%2 == 0 ? width/2 : width/2 + 1;

	int bheight = height/2;
	int theight = height%2 == 0 ? height/2 : height/2 + 1;

	setBoundingBox(GraphicsBoundingBox(glm::vec2(dx - lwidth, dy - bheight), glm::vec2(dx + rwidth, dy + theight)));*/

	vertex_data_.setRectangleVertices(dx, dy, width, height);

/*	vertex_data_.setVertex(0, dx - lwidth, dy - bheight);
	vertex_data_.setVertex(1, dx + rwidth, dy - bheight);
	vertex_data_.setVertex(2, dx + rwidth, dy + theight);
	vertex_data_.setVertex(3, dx - lwidth, dy + theight);
*/
	vertex_data_.setRectangleTextureCoordinates(*sprite_.getDiffuse());



	setBoundingBox(vertex_data_.generateBoundingBox());
/*
	vertex_data_.setTextureCoordinate(0, sprite_.getDiffuse()->getX0(), sprite_.getDiffuse()->getY0());
	vertex_data_.setTextureCoordinate(1, sprite_.getDiffuse()->getX1(), sprite_.getDiffuse()->getY0());
	vertex_data_.setTextureCoordinate(2, sprite_.getDiffuse()->getX1(), sprite_.getDiffuse()->getY1());
	vertex_data_.setTextureCoordinate(3, sprite_.getDiffuse()->getX0(), sprite_.getDiffuse()->getY1());*/
}

/*
static bool isRectangleInView(const glm::vec2& p0, const glm::vec2& p1, const glm::mat4& projection) {
	return !(p1.x < -1 || p0.x > 1 || p0.y > 1 || p1.y < -1); 
}
*/
void SpriteDrawObject::draw(const GraphicsWorld& world, const glm::mat4& projection, Shader& shader) {

	//glm::mat4 mv = glm::translate(glm::vec3((getPosition() - world.getCameraPosition()) * getCameraFactor() + world.getCameraOffset(), 0.0f));
	glm::mat4 mvp = calculateModelViewProjectionMatrix(world, projection);
	
	GraphicsBoundingBox bb = getBoundingBox();
	bb.transform(mvp);

	if (!bb.isProjectedOnScreen()) {
		return;
	} 

	glm::mat4 m = calculateModelMatrix();

	bb = getBoundingBox();

	bb.transform(m);

	world.submitLights(bb, getDepth(), shader);
	

	glUniformMatrix4fv(shader.getUniformHandler("mvp"), 1, false, glm::value_ptr(mvp));
	glUniformMatrix4fv(shader.getUniformHandler("m"), 1, false, glm::value_ptr(m));

	glUniform4fv(shader.getUniformHandler("mix_color"), 1, glm::value_ptr(getMixColor()));
	glUniform4fv(shader.getUniformHandler("multiply_color"), 1, glm::value_ptr(getMultiplyColor()));

	glActiveTexture(GL_TEXTURE0);
	sprite_.bindDiffuse();
	glUniform1i(shader.getUniformHandler("diffuse_sampler"), 0);

	glActiveTexture(GL_TEXTURE1);
	sprite_.bindNormal();
    glUniform1i(shader.getUniformHandler("normal_sampler"), 1);

	vertex_data_.draw(shader);
}