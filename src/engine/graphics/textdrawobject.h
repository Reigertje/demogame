#ifndef TEXTDRAWOBJECT_H
#define TEXTDRAWOBJECT_H

#include <string>
#include <vector>

#include "drawobject.h"
#include "vertexdata.h"
#include "sprite.h"

class Texture;

class TextDrawObject : public DrawObject {

public:

	TextDrawObject(const Sprite& sprite, int rows, int cols); 

	void draw(const GraphicsWorld& world, const glm::mat4& projection, Shader& shader);

	void nextPage();

	bool hasNextPage() const;

	bool isAnimationFinished() const;

	void finishAnimation();

	void setAnimationEnabled(bool animation_enabled);

	void stepAnimation(float dt);

	void setText(const std::string& text_);

private:

	static const int CHAR_WIDTH = 6;

	static const int SPRITE_WIDTH = 128;

	static const int CHAR_HEIGHT = 8;

	static const int SPRITE_HEIGHT = 64;

	void set(int position, int character_key);

	void initializeVertices();

	void initializeTextureCoordinates();

	void clearText();

	void updateText(bool return_on_first);

	void updateForIndex(int index);

	Sprite sprite_;

	VertexData vertex_data_;

	std::vector<std::string> tokens_;

	std::vector<std::string> lines_;

	int rows_;

	int cols_;

	int current_row_;

	// Animation

	int current_index_;

	float time_;

	bool animation_enabled_;

	bool animation_finished_;

};

#endif