#ifndef LEVELSCENE_H
#define LEVELSCENE_H

#include "scene.h"
#include "utils/grid.h"

class LevelScene : public Scene {

public: 

	struct Link {
		std::string to;
		int x, y;
		Link(const std::string& to, int x, int y) : to(to), x(x), y(y) {};
	};

	struct HorizontalPlatform {
		int x, y;
		int width;
		HorizontalPlatform(int x, int y, int width) : x(x), y(y), width(width) {};
	};	


	LevelScene();

	void build(const Grid<bool>& grid, const std::string& theme, const std::vector<Link>& links);

	virtual void spawnPlayer(Actor* player, const std::string& origin = "NULL");

	virtual void step(float dt);

protected:

	virtual void createLink(const Link& link) = 0;

	virtual void onLevelGridGenerated(Grid<bool>& grid) {};

	virtual void onLinksCreated(const Grid<bool>& grid, const std::vector<Link>& links) {};

	std::vector<HorizontalPlatform> getHorizontalPlatforms(const Grid<bool>& platforms);

private: 

	std::vector<Link> links_;

	Grid<bool> level_grid_;

};

#endif