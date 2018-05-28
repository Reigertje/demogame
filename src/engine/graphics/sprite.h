#ifndef SPRITE_H
#define SPRITE_H

#include "texture.h"

class Sprite {

public:

	Sprite();

	Sprite(Texture* diffuse, Texture* normal);

	const Texture* getDiffuse() const; 

	const Texture* getNormal() const;

	int getPixelWidth() const;

	int getPixelHeight() const;

	void bindDiffuse();

	void bindNormal();

private:

	Texture* diffuse_;

	Texture* normal_;

};

#endif