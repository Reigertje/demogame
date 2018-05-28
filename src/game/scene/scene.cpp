#include "scene.h"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "game/game.h"
#include "game/actor/actor.h"
#include "game/renderer/gamerenderer.h"
#include "engine/geom/shape.h"
#include "engine/geom/rectangle.h"

//debug
#include "engine/graphics/spritedrawobject.h"
#include "utils/utils.h"

Scene::Scene() : graphics_world_(), renderer_() {
	graphics_world_.setRenderer(&renderer_);
	graphics_world_.setCameraOffset(160, 90);
	dialogue_.addToScene(*this);
}

Scene::~Scene() {
	for (int i = 0; i < actors_to_add_.size(); ++i) {
		delete actors_to_add_[i];
	}
	for (int i = 0; i < actors_.size(); ++i) {
		actors_[i]->removeFromScene(this);
		delete actors_[i];
	}
}

void Scene::addActor(Actor* actor) {
	actors_to_add_.push_back(actor);
}

void Scene::removeActor(Actor* actor) {
	actors_to_remove_.push_back(actor);
}

void Scene::addPhysicsShape(PhysicsShape* shape) {
	physics_world_.addPhysicsShape(shape);
}

void Scene::removePhysicsShape(PhysicsShape* shape) {
	physics_world_.removePhysicsShape(shape);
}

void Scene::addDrawObject(DrawObject* object) {
	graphics_world_.addDrawObject(object);
}

void Scene::removeDrawObject(DrawObject* object) {
	graphics_world_.removeDrawObject(object);
}

void Scene::addLight(Light* light) {
	graphics_world_.addLight(light);
}

void Scene::removeLight(Light* light) {
	graphics_world_.removeLight(light);
}

void Scene::synchronize() {
	// Add actors
	for (int i = 0; i < actors_to_add_.size(); ++i) {
		if (actors_.size() == 0 || actors_to_add_[i]->getId() > actors_.back()->getId()) {
			actors_.push_back(actors_to_add_[i]);
		} else {
			std::vector<Actor*>::iterator low = std::lower_bound(actors_.begin(), actors_.end(), actors_to_add_[i], Actor::compareById());
			actors_.insert(low, actors_to_add_[i]);
		}
		actors_to_add_[i]->addToScene(this);
	}
	// Remove actors
	for (int i = 0; i < actors_to_remove_.size(); ++i) {
		for (int j = 0; j < actors_.size(); ++j) {
			if (actors_to_remove_[i] == actors_[j]) {
				actors_.erase(actors_.begin() + j);
				actors_to_remove_[i]->removeFromScene(this);
				////delete actors_to_remove_[i];
				break;
			}
		}
	}

	actors_to_add_.clear();
	actors_to_remove_.clear();
}


float getTimeLerp(float a, float b, float x) {
	return (x - a)/(b - a);
}

void Scene::step(float dt) {

	// debug

	for (int i = 0; i < debug_draw_objects_.size(); ++i) {
		removeDrawObject(debug_draw_objects_[i]);
		delete debug_draw_objects_[i];
	}
	debug_draw_objects_.clear();


	for (int i = 0; i < actors_.size(); ++i) {
		actors_[i]->step(dt);
	}

	physics_world_.step(dt);

	if (dialogue_.isActive()) {
		dialogue_.step(dt);
	}
	synchronize();
}

void Scene::render() {
	graphics_world_.render();
}

void Scene::startDialogue(const std::string& key) {
	if (!isInDialogue()) {
		dialogue_.startDialogue(key);
	}
}

bool Scene::isInDialogue() const {
	return dialogue_.isActive();
}

void Scene::setCameraPosition(const vec2f& position) {
	graphics_world_.setCameraPosition(position.x, position.y);
}

GraphicsWorld& Scene::getGraphicsWorld() {
	return graphics_world_;
}

bool Scene::isAreaOccupied(const Shape& shape) {
	return physics_world_.isAreaOccupied(shape);
}

bool Scene::isAreaFree(const Shape& shape) {
	return physics_world_.isAreaFree(shape);
}

Rectangle Scene::getHorizontalProjection(float x, float x0, float x1, float y0, float y1) const {
	return physics_world_.getHorizontalProjection(x, x0, x1, y0, y1);
}

/*Rectangle Scene::getLeftProjection(float x, float y0, float y1) const {
	return physics_world_.getLeftProjection(x, y0, y1);
}

Rectangle Scene::getRightProjection(float x, float y0, float y1) const {
	return physics_world_.getRightProjection(x, y0, y1);
}*/

std::vector<Actor*> Scene::getHits(const Shape& shape) const {
	std::vector<Actor*> result;
	for (Actor* actor : actors_) {
		if (actor->hitsHitShape(shape)) {
			result.push_back(actor);
		}
	}
	return result;
}

std::vector<Interaction*> Scene::getInteractions(const Shape& shape) const {
	std::vector<Interaction*> result;
	for (Actor* actor : actors_) {
		if (actor->hitsInteractionShape(shape)) {
			const std::vector<Interaction*> interactions = actor->getInteractions();
			result.insert(result.end(), interactions.begin(), interactions.end());
		}
	}
	return result;
}

void Scene::debugShape(const Shape& shape) {

	int half_width = shape.getMaxX() - shape.getX();
	int half_height = shape.getMaxY() - shape.getY();

	DrawObject* debug_draw_object = new SpriteDrawObject(Game::getInstance().getResources().getSprite("_"), shape.getX(), shape.getY(), half_width * 2, half_height * 2);
	debug_draw_objects_.push_back(debug_draw_object);
	addDrawObject(debug_draw_object);
}