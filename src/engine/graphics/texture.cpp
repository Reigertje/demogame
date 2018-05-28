#include "texture.h"

int Texture::getPixelWidth() const {
	return pixel_width_;
}

int Texture::getPixelHeight() const {
	return pixel_height_;
}

glm::vec2 Texture::getP0() const {
	return p0_;
}

glm::vec2 Texture::getP1() const {
	return p1_;
}

float Texture::getX0() const {
	return p0_.x;
}

float Texture::getX1() const {
	return p1_.x;
}

float Texture::getY0() const {
	return p0_.y;
}

float Texture::getY1() const {
	return p1_.y;
}

void Texture::setPixelWidth(int pixel_width) {
	pixel_width_ = pixel_width;
}

void Texture::setPixelHeight(int pixel_height) {
	pixel_height_ = pixel_height;
}

void Texture::setP0(const glm::vec2& p0) {
	p0_ = p0;
}

void Texture::setP1(const glm::vec2& p1) {
	p1_ = p1;
}

SubTexture::SubTexture(Texture* texture, int x, int y, int width, int height) {
	texture_ = texture;
	setPixelHeight(height);
	setPixelWidth(width);

	setP0(glm::vec2(x / (float)texture->getPixelWidth(), (y + height) / (float)texture->getPixelHeight()));
	setP1(glm::vec2((x + width) / (float)texture->getPixelWidth(), y / (float)texture->getPixelHeight()));
}

void SubTexture::load() {
	texture_->load();
}

void SubTexture::unload() {
	// Handled by parent
}

void SubTexture::bind() {
	texture_->bind();
}