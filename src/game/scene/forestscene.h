#ifndef FORESTSCENE_H
#define FORESTSCENE_H

#include "levelscene.h"
#include "game/actor/lightparticlesactor.h"

class SpriteDrawObject;

class ForestScene : public LevelScene {

public:

	ForestScene(int special);

	void step(float dt);

protected:

	void createLink(const Link& link);

	void onLevelGridGenerated(Grid<bool>& grid);

	void onLinksCreated(const Grid<bool>& grid, const std::vector<Link>& links);

private:

	void initialize();

	void updateSceneForTime();

	void generateTrees(int y, int start_x, int end_x);

	void generateTrees(const Grid<bool>& grid);

	RayLight* sun_;

	RayLight* moon_;
		
	SpriteDrawObject* background_gradient_;

	SpriteDrawObject* background_plain_;

	SpriteDrawObject* test_sun_;

	SpriteDrawObject* test_moon_;

	SpriteDrawObject* stars_;

	LightParticlesActor* hotaru_;

};

#endif