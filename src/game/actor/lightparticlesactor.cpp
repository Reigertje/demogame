#include "lightparticlesactor.h"

#include "engine/graphics/light.h"
#include "engine/graphics/spritedrawobject.h"
#include "game/scene/scene.h"
#include "utils/random.h"

#include <cmath>

LightParticlesActor::LightParticlesActor(int count, const Sprite& sprite, const Grid<bool>& grid, float cell_width, float cell_height) : grid_(grid) {
	min_radius_ = 5;
	max_radius_ = 40;
	depth_ = -4;
	speed_ = 5;
	cell_width_ = cell_width;
	cell_height_ = cell_height;
	visibility_ = 1.0f;

	color_ = glm::vec3(0.4f, 0.8f, 0.0f);

	for (int i = 0; i < count; ++i) {
		positions_.push_back(getRandomPosition());
		sprites_.push_back(new SpriteDrawObject(sprite));
		directions_.push_back(getRandomDirection());
		radius_.push_back(utils::random(min_radius_, max_radius_));
		pulse_.push_back(utils::random()%2 == 0);
	}

	for (int i = 0; i < count; ++i) {
		lights_.push_back(new Light(radius_[i]));
		lights_[i]->setPosition(glm::vec2(positions_[i].x, positions_[i].y));
		lights_[i]->setColor(color_);
		lights_[i]->setDepth(depth_);
		sprites_[i]->setPosition(glm::vec2(positions_[i].x, positions_[i].y));
		sprites_[i]->setDrawLayer(-4);
	}
}

vec2f LightParticlesActor::getRandomDirection() const {

	float theta = (2.0 * 3.1415) * utils::random(0.0f, 1.0f);

	float x = std::cos(theta);
	float y = std::sin(theta);
	
	return vec2f(x, y);
}



vec2f LightParticlesActor::getRandomPosition() const {
	std::vector<vec2f> cells;
	for (int x = 0; x < grid_.getWidth(); ++x) {
		for (int y = 0; y < grid_.getHeight(); ++y) {
			if (!grid_[x][y]) cells.push_back(vec2f(x, y));
		}
	}

	vec2f random_cell = cells[utils::random() % cells.size()];

	return vec2f(random_cell.x * cell_width_ + utils::random(0.0f, cell_width_), random_cell.y * cell_height_ + utils::random(0.0f, cell_height_));
}

void LightParticlesActor::onAddToScene() {
	for (int i = 0; i < lights_.size(); ++i) {
		getScene()->addLight(lights_[i]);
		getScene()->addDrawObject(sprites_[i]);
	}
}

void LightParticlesActor::onRemoveFromScene() {
	for (int i = 0; i < lights_.size(); ++i) {
		getScene()->removeLight(lights_[i]);
		getScene()->removeDrawObject(sprites_[i]);
	}
}

void LightParticlesActor::onStep(float dt) {
	for (int i = 0; i < lights_.size(); ++i) {

		int current_cell_x = (int)positions_[i].x/cell_width_;
		int current_cell_y = (int)positions_[i].y/cell_height_;

		// UPDATE X
		float new_x = positions_[i].x + directions_[i].x * speed_ * dt;

		int new_cell_x = (int)new_x/cell_width_;
		
		if (grid_.isOutOfBound(new_cell_x, current_cell_y) || grid_[new_cell_x][current_cell_y]) {
			new_x = positions_[i].x;
			directions_[i].x = -directions_[i].x;
		}

		// UPDATE Y

		float new_y = positions_[i].y + directions_[i].y * speed_ * dt;

		int new_cell_y = (int)new_y/cell_height_;
		new_cell_x = (int)new_x/cell_width_;

		if (grid_.isOutOfBound(new_cell_x, new_cell_y) || grid_[new_cell_x][new_cell_y]) {
			new_y = positions_[i].y;
			directions_[i].y = -directions_[i].y;
		}


		if (pulse_[i]) {
			radius_[i] += 20.0 * dt;
			if (radius_[i] >= max_radius_) pulse_[i] = false;
		} else {
			radius_[i] -= 20.0 * dt;
			if (radius_[i] <= min_radius_) pulse_[i] = true;
		}

		positions_[i].x = new_x;
		positions_[i].y = new_y;

		//if (!area_.contains)
		lights_[i]->setPosition(glm::vec2(positions_[i].x, positions_[i].y));
		sprites_[i]->setPosition(glm::vec2(positions_[i].x, positions_[i].y));
		sprites_[i]->setMultiplyColor(glm::vec4(1.0f, 1.0f, 1.0f, visibility_));
		lights_[i]->setRadius(radius_[i] * visibility_);

	}
}

void LightParticlesActor::setVisibility(float vis) {
	visibility_ = vis;
}