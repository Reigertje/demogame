#ifndef TEXTURE_H
#define TEXTURE_H

#include "glm.h"

class Texture {

public:

	Texture() : pixel_width_(0), pixel_height_(0) {};

	virtual ~Texture() = default;

	virtual void load() = 0;

	virtual void unload() = 0;

	virtual void bind() = 0;

	int getPixelWidth() const;

	int getPixelHeight() const;

	glm::vec2 getP0() const;

	glm::vec2 getP1() const;

	float getX0() const;

	float getX1() const;

	float getY0() const;

	float getY1() const;

protected:

	void setPixelWidth(int pixel_width);

	void setPixelHeight(int pixel_height);

	void setP0(const glm::vec2& p0); 

	void setP1(const glm::vec2& p1);

private:

	int pixel_width_;

	int pixel_height_;

	glm::vec2 p0_;

	glm::vec2 p1_;

};

class SubTexture : public Texture {

public:

	SubTexture(Texture* texture, int x, int y, int width, int height);

	void load();

	void unload();

	void bind();

private:

	Texture* texture_;
};

#endif