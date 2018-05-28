#include "GL/glew.h"
#include <algorithm>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_opengl.h>
#include <chrono>
#include <iomanip>

#include "game/game.h"
#include "utils/grid.h"
#include "utils/utils.h"
#include "utils/random.h"
#include "engine/graphics/textdrawobject.h"
#include "engine/dialogue/dialogue.h"

static int SCREEN_WIDTH = 1280;
static int SCREEN_HEIGHT = 720;
static SDL_Window * window;

void init() {
    //Init SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        std::cerr << "Error initializing SDL 1.2" << std::endl; 
        exit(-1);
    }

    window = SDL_CreateWindow(
        "Game demo", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL
    );

    if (window == NULL) {
        std::cerr << "Error init window" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        exit(-1);
    } 

    SDL_GL_CreateContext(window);

    glewInit();
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "Error initializing GLEW : " << gluErrorString(error) << std::endl;
        exit(-1);
    }

    Game::initialize(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void destroy() {
	Game::destroy();
	SDL_DestroyWindow(window);
    SDL_Quit();
}

void gameloop() {
	bool running = true;
	SDL_Event event; 

	long prevTime = utils::getCurrentTimeMillis(); 

	while (running) {
		while(SDL_PollEvent(&event)){
            if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && event.key.repeat == 0) {
                SDL_Keycode key_code = event.key.keysym.sym;
                if (key_code == SDLK_ESCAPE) {
                    running = false;
                } else if (event.type == SDL_KEYDOWN) {
                    Game::getInstance().getInput().press(key_code);
                } else {
                    Game::getInstance().getInput().release(key_code);
                }
            } 
        }

        long time = utils::getCurrentTimeMillis();

        Game::getInstance().step(std::min((time - prevTime) / 1000.0f, 1.0f/30.0f));
        Game::getInstance().render();

        prevTime = time;

		SDL_GL_SwapWindow(window);
	}
}


int main(int argc, char * argv[]) {

    utils::seed(utils::getCurrentTimeMillis());
    init();

    gameloop();
   	destroy();

    return 0;
}