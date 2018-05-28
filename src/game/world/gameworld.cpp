#include "gameworld.h"

#include "game/scene/forestscene.h"
#include "game/actor/agent/human/playeragent.h"
#include "game/actor/itemactor.h"

#include "utils/utils.h"

#include <iostream>

GameWorld::GameWorld() {

	current_level_key_ = "NULL";

	current_level_ = 0;

	player_ = new PlayerAgent();

	generateForest();

	requestChangeLevel("hunter");

	time_ = 19.0;
}

PlayerAgent* GameWorld::getPlayer() {
	return player_;
}
	
LevelScene* GameWorld::getCurrentLevel() {
	return current_level_;	
}

void GameWorld::requestChangeLevel(const std::string& key) {	
	pending_switch_ = true;
	pending_level_key_ = key;
}

void GameWorld::switchLevels() {

	if (current_level_) {
		current_level_->removeActor(player_);
		current_level_->synchronize();
	}

	current_level_ = levels_[pending_level_key_];
	current_level_->spawnPlayer(player_, current_level_key_);
	current_level_->synchronize();
	current_level_->step(1.0/60.0f);
	current_level_key_ = pending_level_key_;
	pending_switch_ = false;
}

void GameWorld::generateForest() {

	levels_["hunter"] = new ForestScene(0);


}

float GameWorld::getTime() const {
	return time_;
}

void GameWorld::step(float dt) {

	time_ = time_ + 0.1 * dt;

	if (time_ >= 24.0f) {
		time_ -= 24.0f;
	}

	if (pending_switch_) {
		switchLevels();
	}

	if (current_level_) {
		long step_time = utils::getCurrentTimeMillis();
		current_level_->step(dt);
		//std::cout << "Scene step time : "  << utils::getCurrentTimeMillis() - step_time << std::endl;
	}
}

// debug 
void GameWorld::debugShape(const Shape& shape) {
	if (current_level_) {
		current_level_->debugShape(shape);
	}
}