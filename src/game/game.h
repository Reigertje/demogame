#ifndef GAME_H
#define GAME_H

#include "engine/input/input.h"
#include "resources.h"
#include "engine/geom/vec2f.h"
#include "game/world/gameworld.h"

class Scene;

class Game {

public:

	static const int ACTION_UP = 1;
	static const int ACTION_DOWN = 2;
	static const int ACTION_LEFT = 4;
	static const int ACTION_RIGHT = 1000;
	static const int ACTION_ATTACK = 3000;
	static const int ACTION_BLOCK = 3500;
	static const int ACTION_INTERACT = 4000;
	static const int ACTION_INVENTORY = 5000;
	static const int ACTION_TAB = 5500;
	static const int ACTION_SPRINT = 6000;

	static void initialize(int screen_width, int screen_height);

	static void destroy();

	static Game& getInstance();

	void step(float dt);

	void render();

	Input& getInput();

	Resources& getResources();

	GameWorld& getGameWorld();

	vec2f getResolution() const;

	vec2f getWindowSize() const;

private:

	Game(int screen_width, int screen_height);

	~Game();

	Input input_;

	Resources resources_;

	GameWorld* world_;

	static Game* instance_;

	vec2f resolution_;

	vec2f window_size_;

};

#endif


