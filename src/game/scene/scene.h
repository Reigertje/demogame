#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "engine/physics/physicsworld.h"
#include "engine/graphics/graphicsworld.h"
#include "game/renderer/gamerenderer.h"
#include "engine/geom/vec2f.h"
#include "scenedialogue.h"

class Actor;
class PhysicsShape;
class Rectangle;
class Interaction;
class DrawObject;
class Light;
class PlayerAgent;

class Scene {

friend class GameWorld;

public:

	Scene();

	~Scene();

	void addActor(Actor* actor);

	void removeActor(Actor* actor);

	void addPhysicsShape(PhysicsShape* shape);

	void removePhysicsShape(PhysicsShape* shape);

	void addDrawObject(DrawObject* object);

	void removeDrawObject(DrawObject* object);

	void addLight(Light* light);

	void removeLight(Light* light);
	
	bool isAreaFree(const Shape& shape);
	
	bool isAreaOccupied(const Shape& shape);

	void startDialogue(const std::string& key);

	bool isInDialogue() const;

	Rectangle getHorizontalProjection(float x, float x0, float x1, float y0, float y1) const;

/*	Rectangle getLeftProjection(float x, float y0, float y1) const;

	Rectangle getRightProjection(float x, float y0, float y1) const;
*/
	void setCameraPosition(const vec2f& position);

	std::vector<Actor*> getHits(const Shape& shape) const;

	std::vector<Interaction*> getInteractions(const Shape& shape) const;

	virtual void step(float dt);

	void synchronize();

	void render();

	// debug 
	void debugShape(const Shape& shape);

protected:

	GraphicsWorld& getGraphicsWorld();

private:

	Actor* player_;

	GraphicsWorld graphics_world_;

	GameRenderer renderer_;

	PhysicsWorld physics_world_;

	SceneDialogue dialogue_;

	std::vector<Actor*> actors_;

	std::vector<Actor*> actors_to_add_;

	std::vector<Actor*> actors_to_remove_;

	//debug
	std::vector<DrawObject*> debug_draw_objects_;

};

#endif