#include "sprite.h"

Sprite::Sprite() {
	diffuse_ = normal_ = 0;
}

Sprite::Sprite(Texture* diffuse, Texture* normal) {
	diffuse_ = diffuse;
	normal_ = normal;
}

const Texture* Sprite::getDiffuse() const {
	return diffuse_;
}

const Texture* Sprite::getNormal() const {
	return normal_;
}

int Sprite::getPixelWidth() const {
	return diffuse_->getPixelWidth();
}

int Sprite::getPixelHeight() const {
	return diffuse_->getPixelHeight();
}

void Sprite::bindDiffuse(){ 
	diffuse_->bind();
}

void Sprite::bindNormal(){ 
	normal_->bind();
}