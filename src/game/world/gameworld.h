#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "game/scene/levelscene.h"

#include <map>

class PlayerAgent;

class GameWorld {

public:

	GameWorld();
	
	LevelScene* getCurrentLevel();

	void requestChangeLevel(const std::string& key);

	PlayerAgent* getPlayer();

	void step(float dt);

	float getTime() const;// debug 
	
	void debugShape(const Shape& shape);

private:

	bool pending_switch_;

	std::string pending_level_key_;

	LevelScene* current_level_;

	std::string current_level_key_;

	std::map<std::string, LevelScene *> levels_;

	PlayerAgent* player_;

	float time_;

	void generateForest();

	void switchLevels();

};


#endif