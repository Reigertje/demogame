#ifndef LIGHTPARTICLESACTOR_H
#define LIGHTPARTICLESACTOR_H

#include "actor.h"
#include "engine/geom/rectangle.h"
#include "engine/graphics/glm.h"
#include "utils/grid.h"

class Light;
class SpriteDrawObject;
class Sprite;

class LightParticlesActor : public Actor {

public:

	LightParticlesActor(int count, const Sprite& sprite, const Grid<bool>& grid, float cell_width, float cell_height);

	void onAddToScene();

	void onRemoveFromScene();

	void onStep(float dt);

	void setVisibility(float vis);

private:

	vec2f getRandomDirection() const;

	vec2f getRandomPosition() const;

	float min_radius_;

	float max_radius_;

	float depth_;

	float speed_;

	float visibility_;

	glm::vec3 color_;

	Grid<bool> grid_;

	float cell_width_;

	float cell_height_;

	std::vector<Light*> lights_;

	std::vector<SpriteDrawObject*> sprites_;

	std::vector<vec2f> positions_;

	std::vector<vec2f> directions_;

	std::vector<float> radius_;

	std::vector<bool> pulse_;



};

#endif