#include "game.h"

#include "SDL2/SDL.h"
#include <iostream>


#include "engine/graphics/light.h"
#include "game/scene/scene.h"
#include "game/actor/spriteactor.h"
#include "game/actor/agent/human/playeragent.h"
#include "engine/graphics/textdrawobject.h"
#include "game/actor/itemactor.h"
#include "game/actor/lightparticlesactor.h"
#include "game/actor/levelgridactor.h"

Game * Game::instance_ = 0;

/**
	STATIC CONTROL
**/

void Game::initialize(int screen_width, int screen_height) {
	instance_ = new Game(screen_width, screen_height);
	
	instance_->world_ = new GameWorld();
}

void Game::destroy() {
	delete instance_;
}

Game& Game::getInstance() {
	return *instance_;
}

Resources& Game::getResources() {
	return resources_;
}

Input& Game::getInput() {
	return input_;
};

vec2f Game::getWindowSize() const {
	return window_size_;
}

vec2f Game::getResolution() const {
	return resolution_;
}

GameWorld& Game::getGameWorld() {
	return *world_;
}

Game::Game(int screen_width, int screen_height) {
	window_size_ = vec2f(screen_width, screen_height);
	resolution_ = vec2f(320, 180);

	input_.addActionKey(ACTION_UP, SDLK_UP);
	input_.addActionKey(ACTION_DOWN, SDLK_DOWN);
	input_.addActionKey(ACTION_LEFT, SDLK_LEFT);
	input_.addActionKey(ACTION_RIGHT, SDLK_RIGHT);
	input_.addActionKey(ACTION_ATTACK, SDLK_z);
	input_.addActionKey(ACTION_BLOCK, SDLK_v);
	input_.addActionKey(ACTION_INTERACT, SDLK_SPACE);
	input_.addActionKey(ACTION_INVENTORY, SDLK_i);
	input_.addActionKey(ACTION_TAB, SDLK_TAB);
	input_.addActionKey(ACTION_SPRINT, SDLK_LSHIFT);

}

Game::~Game() {
	if (world_) delete world_;
}

void Game::step(float dt) {
	world_->step(dt);
	input_.flush();
}

void Game::render() {
	if (world_->getCurrentLevel()) {
		world_->getCurrentLevel()->render();
	}
}