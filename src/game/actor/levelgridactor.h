#ifndef LEVELGRIDACTOR_H
#define LEVELGRIDACTOR_H

#include "actor.h"
#include "utils/grid.h"
#include "engine/graphics/spritedrawobject.h"
#include "engine/physics/physicsshape.h"

class PhysicsShape;
class SpriteDrawObject;

class LevelGridActor : public Actor {

public:

	LevelGridActor(const Grid<bool>& grid, const std::string& sprite_key);

	void onAddToScene();

	void onRemoveFromScene();

	void onStep(float dt);

private:

	static Grid<bool> generateBackTileGrid(const Grid<bool>& grid);

	void generateTileDrawObject(const Grid<bool>& grid, int x, int y);

	void generateBackTileDrawObject(const Grid<bool>& grid, int x, int y);

	void generateTilePhysicsShape(const Grid<bool>& grid, int x, int y);

	std::string sprite_key_;

	// TODO in 1 draw container voor optimalisatie
	std::vector<SpriteDrawObject> draw_objects_;

	std::vector<PhysicsShape> shapes_;

};

#endif