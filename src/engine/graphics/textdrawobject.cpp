#include "textdrawobject.h"

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <iterator>

#include "shader.h"

#include "graphicsworld.h"

static std::vector<std::string> tokenize(const std::string& text) {
	std::vector<std::string> result;
	int word_start = 0;
	for (int i = 0; i < text.size(); ++i) {
		if (text[i] == ' ' || text[i] == '\n' || i == text.size() - 1) {
			int word_end = i == text.size() - 1 ? i + 1 : i;
			if (word_end - word_start > 0) result.push_back(text.substr(word_start, word_end - word_start));
			if (text[i] == '\n') result.push_back("\n");
			word_start = word_end + 1;
		}
	}
	return result;
}

static std::vector<std::string> lineize(const std::vector<std::string>& tokens, int line_length) {
	std::vector<std::string> result;
	std::string line;
	for (int i = 0; i < tokens.size(); ++i) {
		// std::cout << tokens[i] << std::endl;
		if (tokens[i] == "\n") {
			// Newline token
			result.push_back(line);
			line = "";
		} else {
			// Word token
			std::string new_line = line + (line.size() > 0 ? " " + tokens[i] : tokens[i]);
			if (new_line.size() <= line_length) {
				line = new_line;
				
			} else {
				result.push_back(line);
				line = tokens[i];				
			}
			if (i == tokens.size() - 1) result.push_back(line);
		}
	}
	return result;
}


static int getCharacterKey(char val) {
	return 
		(val >= 'a' && val <= 'z') ? val - 'a' :
		(val >= 'A' && val <= 'Z') ? val - 'A' + 26 :
		val == '?' ? 53 :
		val == '!' ? 54 :
		val == '.' ? 55 :
		val == ',' ? 56 :
		val == '<' ? 57 :
		val == '>' ? 58 :
		52;
}

void TextDrawObject::set(int position, int character_key) {
	int idx = position * 6;

	int row_width = SPRITE_WIDTH / CHAR_WIDTH * CHAR_WIDTH;

	float w = (float) CHAR_WIDTH / SPRITE_WIDTH;
	float h = (float) CHAR_HEIGHT / SPRITE_HEIGHT;

	float tx = (float)((character_key  * CHAR_WIDTH) % row_width) / SPRITE_WIDTH;
	float ty = (float)((character_key  * CHAR_WIDTH) / row_width * CHAR_HEIGHT) / SPRITE_HEIGHT;

	vertex_data_.setTextureCoordinate(idx + 0, tx, ty + h);
	vertex_data_.setTextureCoordinate(idx + 1, tx + w, ty + h);
	vertex_data_.setTextureCoordinate(idx + 2, tx + w, ty);

	vertex_data_.setTextureCoordinate(idx + 3, tx, ty + h);
	vertex_data_.setTextureCoordinate(idx + 4, tx + w,ty);
	vertex_data_.setTextureCoordinate(idx + 5, tx, ty);
}


TextDrawObject::TextDrawObject(const Sprite& sprite, int rows, int cols) : vertex_data_(rows * cols * 6), rows_(rows), cols_(cols) {
	sprite_ = sprite;
	setAnimationEnabled(false);
	time_ = 0.0f;
	animation_finished_ = false;
	current_index_ = 0;
	current_row_ = 0;

	initializeVertices();
}

void TextDrawObject::initializeVertices() {
	vertex_data_.setType(GL_TRIANGLES);
	for (int y = 0; y < rows_; ++y) {
		for (int x = 0; x < cols_; ++x) {

			int idx = (y * cols_ + x) * 6;

			vertex_data_.setVertex(idx + 0, x * CHAR_WIDTH, -y * CHAR_HEIGHT - CHAR_HEIGHT);
			vertex_data_.setVertex(idx + 1, x * CHAR_WIDTH + CHAR_WIDTH, -y * CHAR_HEIGHT - CHAR_HEIGHT);
			vertex_data_.setVertex(idx + 2, x * CHAR_WIDTH + CHAR_WIDTH, -y * CHAR_HEIGHT);

			vertex_data_.setVertex(idx + 3, x * CHAR_WIDTH, -y * CHAR_HEIGHT - CHAR_HEIGHT);
			vertex_data_.setVertex(idx + 4, x * CHAR_WIDTH + CHAR_WIDTH, -y * CHAR_HEIGHT);
			vertex_data_.setVertex(idx + 5, x * CHAR_WIDTH, -y * CHAR_HEIGHT);

			set(y * cols_ + x, getCharacterKey(' '));
		}
	}
}

void TextDrawObject::clearText() {
	for (int i = current_index_; i < cols_ * rows_; ++i) {
		set(i, getCharacterKey(' '));
	}
}

void TextDrawObject::updateText(bool return_on_first) {
	for (int i = current_index_; i < cols_ * rows_; ++i) {
		int col = i%cols_;
		int row = current_row_ + i/cols_;
		int key = getCharacterKey(' ');
		if (row < lines_.size() && col < lines_[row].size()) {
			key = getCharacterKey(lines_[row][col]);
		}
		set(i, key);
		if (return_on_first && key != getCharacterKey(' ')) {
			current_index_ = i + 1;
			return;
		}
	}
	animation_finished_ = true;
}

void TextDrawObject::nextPage() {
	time_ = 0;
	current_index_ = 0;
	animation_finished_ = false;
	current_row_ += rows_;
	clearText();
}

bool TextDrawObject::hasNextPage() const {
	return current_row_ + rows_ < lines_.size();
}

bool TextDrawObject::isAnimationFinished() const {
	return animation_finished_;
}

void TextDrawObject::finishAnimation() {
	updateText(false);
}

void TextDrawObject::setText(const std::string& text) {
	tokens_ = tokenize(text);
	lines_ = lineize(tokens_, cols_);

	for (int i = 0; i < lines_.size(); ++i) {
		//	std::cout << lines_[i] << std::endl;
	}
	if (!animation_enabled_) {
		updateText(false);
	} else {
		animation_finished_ = false;
		current_row_ = 0;
		current_index_ = 0;
		time_ = 0;
		clearText();
	}

}

void TextDrawObject::setAnimationEnabled(bool animation_enabled) {
	animation_enabled_ = animation_enabled;
}

void TextDrawObject::stepAnimation(float dt) {
	if (!animation_enabled_) return;

/*	if (animation_finished_ && hasNextPage()) {
		current_row_ += rows_;
		nextPage();
	}*/

	time_ += dt;
	if (time_ >= 0.05) {
		time_ -= 0.05;
		updateText(true);
	}
}

void TextDrawObject::draw(const GraphicsWorld& world, const glm::mat4& projection, Shader& shader) {

	glm::mat4 m = calculateModelMatrix();
	glm::mat4 mvp = calculateModelViewProjectionMatrix(world, projection);

	glUniformMatrix4fv(shader.getUniformHandler("mvp"), 1, false, glm::value_ptr(mvp));
	glUniformMatrix4fv(shader.getUniformHandler("m"), 1, false, glm::value_ptr(glm::translate(glm::vec3(getPosition(), 0.0f))));

	glUniform4fv(shader.getUniformHandler("mix_color"), 1, glm::value_ptr(getMixColor()));
	glUniform4fv(shader.getUniformHandler("multiply_color"), 1, glm::value_ptr(getMultiplyColor()));

	world.submitLights(GraphicsBoundingBox(), getDepth(), shader);

	glActiveTexture(GL_TEXTURE0);
	sprite_.bindDiffuse();
	glUniform1i(shader.getUniformHandler("diffuse_sampler"), 0);

	glActiveTexture(GL_TEXTURE1);
	sprite_.bindNormal();
    glUniform1i(shader.getUniformHandler("normal_sampler"), 1);

	vertex_data_.draw(shader);
}