#include "forestscene.h"

#include "engine/graphics/light.h"
#include "engine/graphics/spritedrawobject.h"

#include "game/game.h"
#include "game/actor/linkactor.h"
#include "game/actor/itemactor.h"
#include "game/actor/agent/enemy/tanukiagent.h"
#include "game/actor/spriteactor.h"
#include "game/actor/agent/human/humannpcagent.h"

#include "utils/random.h"

#include <iostream>
#include <algorithm>

ForestScene::ForestScene(int special) : LevelScene() {

    Grid<bool> grid(10, 10, false);

    std::vector<Link> links;

    links.push_back(Link("NULL", 0, 1));
    links.push_back(Link("forest_0", 9, 1));

    for (int i = 0 ; i < grid.getWidth(); ++i) {
        grid[i][0] = true;
    }

    build(grid, "forest", links);
    initialize();

    SpriteActor* hunter_house = new SpriteActor("hunter_house");
    hunter_house->setPosition(vec2f(5 * 48, 48 + 38));

    HumanNPCAgent* hunter = new HumanNPCAgent();

    hunter->setPosition(vec2f(4 * 48, 48 + 20));

    hunter_house->setDepth(1.0f);
    hunter_house->setDrawLayer(1.0f);

    Light * l = new Light(500);
    Light * l1 = new Light(500);
    l->setDepth(-3.0f);
    l1->setDepth(-3.0f);
    l->setColor(glm::vec3(1.0f, 0.5f, 0.4f));
    l1->setColor(glm::vec3(1.0f, 0.5f, 0.4f));

    l->setPosition(5 * 48 + 12.0f, 48 + 34);
    l1->setPosition(5 * 48 - 51.0f, 48 + 34);
    addLight(l);
    addLight(l1);

    spawnPlayer(new ItemActor("weapontest"));
    spawnPlayer(new ItemActor("shieldtest"));

    addActor(hunter_house);
    addActor(hunter);
}

void ForestScene::initialize() {
    background_gradient_ = new SpriteDrawObject(Game::getInstance().getResources().getSprite("gradient_0"), 320, 180);
    background_gradient_->setCameraFactor(0.0f, 0.0f);
    background_gradient_->setDrawLayer(2000);
    background_gradient_->setDrawBufferIndex(GameRenderer::DRAWBUFFER_BACKDROP);

    background_plain_ = new SpriteDrawObject(Game::getInstance().getResources().getSprite("_"), 320, 180);
    background_plain_->setCameraFactor(0.0f, 0.0f);
    background_plain_->setDrawLayer(3000);
    background_plain_->setDrawBufferIndex(GameRenderer::DRAWBUFFER_BACKDROP);

    stars_ = new SpriteDrawObject(Game::getInstance().getResources().getSprite("stars"));
    stars_->setCameraFactor(0.0f, 0.0f);
    stars_->setDrawLayer(2600);
    stars_->setDrawBufferIndex(GameRenderer::DRAWBUFFER_BACKDROP);

    test_sun_ = new SpriteDrawObject(Game::getInstance().getResources().getSprite("sun"));
    test_sun_->setCameraFactor(0.0f, 0.0f);
    test_sun_->setDrawLayer(2500);
    test_sun_->setDrawBufferIndex(GameRenderer::DRAWBUFFER_BACKDROP);

    test_moon_ = new SpriteDrawObject(Game::getInstance().getResources().getSprite("moon"));
    test_moon_->setCameraFactor(0.0f, 0.0f);
    test_moon_->setDrawLayer(2500);
    test_moon_->setDrawBufferIndex(GameRenderer::DRAWBUFFER_BACKDROP);


    addDrawObject(background_gradient_);
    addDrawObject(background_plain_);

    addDrawObject(test_sun_);
    addDrawObject(test_moon_);
    addDrawObject(stars_);

    SpriteActor* background1 = new SpriteActor("bg_test_2");
    background1->setParallax(0.25, 0);
    background1->setDepth(2000);
    background1->setDrawLayer(1000);
    //background1->setDrawBufferIndex(GameRenderer::DRAWBUFFER_BACKDROP);
    addActor(background1);

    SpriteActor* background = new SpriteActor("bg_test");
    background->setParallax(0.25, 0);
    background->setDepth(1000);
    background->setDrawLayer(1000);
    //background->setDrawBufferIndex(GameRenderer::DRAWBUFFER_BACKDROP);
    addActor(background);

    sun_ = new RayLight();
    moon_ = new RayLight();

    sun_->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    sun_->setPosition(glm::vec2(1.0, 0.0f));
    sun_->setAlpha(0.5);


    moon_->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
    moon_->setPosition(glm::vec2(0.0, 1.0f));
    moon_->setAlpha(0.4);

    getGraphicsWorld().addRayLight(sun_);
    getGraphicsWorld().addRayLight(moon_);
}

void ForestScene::createLink(const LevelScene::Link& link) {
	if (link.to != "NULL") {
		addActor(new LinkActor("forest_exit", link.to, link.x * 48, link.y * 48));	
	}
}

void ForestScene::onLevelGridGenerated(Grid<bool>& grid) {
	std::vector<HorizontalPlatform> platforms = getHorizontalPlatforms(grid);

	if (platforms.size() == 0) return;

	std::sort(platforms.begin(), platforms.end(), [](const HorizontalPlatform& a, const HorizontalPlatform& b) -> bool { return a.width > b.width;});

	int tanukiCount = platforms.size() / 5;

	for (int i = 0; i < tanukiCount; ++i) {


		// std::cout << platforms[i].x << ", " << platforms[i].y << " : " << platforms[i].width << std::endl;
	    TanukiAgent* enemy = new TanukiAgent();

	    float x = utils::random(platforms[i].x * 48 + 24, (platforms[i].x + platforms[i].width) * 48 - 24);
	    float y = platforms[i].y * 48 + 48 + 5;
	    // std::cout << "tanuki x " << x << ", " << y << std::endl;

	    enemy->setPosition(vec2f(x, y));

	    addActor(enemy);
	}

    hotaru_ = new LightParticlesActor((grid.getWidth() * grid.getHeight()) / 4, Game::getInstance().getResources().getSprite("hotaru"), grid, 48, 48);
    addActor(hotaru_);
}

void ForestScene::onLinksCreated(const Grid<bool>& grid, const std::vector<Link>& links) {

}

void ForestScene::step(float dt) {
    LevelScene::step(dt);
    updateSceneForTime();
}

void ForestScene::updateSceneForTime() {
    float time = Game::getInstance().getGameWorld().getTime();

    float sun_height = 0;

    float transposed_time = time + 6.0;
    if (transposed_time >= 24.0f) {
        transposed_time -= 24.0f;
    }

    float alpha = (1.0 - transposed_time/24.0) * 2.0 * 3.1415;

    float orbit_x = cos(alpha);
    float orbit_y = sin(alpha);

    float sun_x = orbit_x * 120;
    float sun_y = orbit_y * 180 - 10.0;

    float moon_x = -orbit_x * 200;
    float moon_y = -orbit_y * 130 - 70.0;

    sun_->setPosition(glm::vec2((sun_x + 160.0f)/ 320.0f, (sun_y + 90.0)/180.0f));
    moon_->setPosition(glm::vec2((moon_x + 160.0f)/ 320.0f, (moon_y + 90.0)/180.0f));

    test_sun_->setCameraOffset(sun_x, sun_y);
    test_moon_->setCameraOffset(moon_x, moon_y);

    glm::vec4 plain_day(225.0f, 252.0f, 255.0f, 255.0f);
    glm::vec4 plain_sunset(52.0f, 62.0f, 35.0f, 255.0f);
    glm::vec4 plain_night(0.0f, 0.0f, 0.0f, 255.0f);

    glm::vec4 gradient_day(255.0f, 255.0f, 255.0f, 255.0f);
    glm::vec4 gradient_sunset(229.0f, 96.0f, 0.0f, 255.0f);
    //glm::vec4 gradient_night(154.0f, 19.0f, 0.0f, 0.0f);
    glm::vec4 gradient_night(355.0f, 19.0f, 0.0f, 0.0f);

    glm::vec3 ambient_day(255.0f, 255.0f, 255.0f);
    glm::vec3 ambient_sunset(200.0f, 130.0f, 100.0f);
    glm::vec3 ambient_night(20.0f, 15.0f, 25.0f);

    glm::vec3 sun_day(191.0f, 191.0f, 161.0f);
    glm::vec3 sun_sunset(255.0f, 96.0f, 75.0f);
    glm::vec3 sun_night(20.0f, 15.0f, 25.0f);

    glm::vec3 moon_day(0.0f, 0.0f, 0.0f);
    glm::vec3 moon_sunset(0.0f, 0.0f, 0.0f);
    glm::vec3 moon_night(70.0f, 50.0f, 80.0f);

    float sun_alpha_day = 0.5;
    float sun_alpha_sunset = 0.65;
    float sun_alpha_night = 0.0;

    float moon_alpha_day = 0.0;
    float moon_alpha_sunset = 0.1;
    float moon_alpha_night = 0.55;

    float stars_alpha_day = 0.0;
    float stars_alpha_sunset = 0.2;
    float stars_alpha_night = 1.0;

    float hot_vis_day = 0.0;
    float hot_vis_sunset = 0.0;
    float hot_vis_night = 1.0;

    glm::vec4 plain_color;
    glm::vec4 gradient_color;
    glm::vec3 ambient_color;
    glm::vec3 sun_color;
    glm::vec3 moon_color;

    float sun_alpha;
    float moon_alpha;
    float stars_alpha;
    float hot_vis;

    float light_lerp = (sun_y + 90.0f) / 180.0f;

    light_lerp = light_lerp > 1.0 ? 1.0 : light_lerp < 0.0 ? 0.0 : light_lerp;

    if (light_lerp >= 0.3f) {
        float plain_lerp = (light_lerp - 0.3)/0.7;
        plain_color = glm::mix(plain_sunset, plain_day, plain_lerp);
        gradient_color = glm::mix(gradient_sunset, gradient_day, plain_lerp);
        ambient_color = glm::mix(ambient_sunset, ambient_day, plain_lerp);
        sun_color = glm::mix(sun_sunset, sun_day, plain_lerp);
        sun_alpha = (1.0 - plain_lerp) * sun_alpha_sunset + plain_lerp * sun_alpha_day;
        moon_color = glm::mix(moon_sunset, moon_day, plain_lerp);
        moon_alpha = (1.0 - plain_lerp) * moon_alpha_sunset + plain_lerp * moon_alpha_day;
        stars_alpha = (1.0 - plain_lerp) * stars_alpha_sunset + plain_lerp * stars_alpha_day;
        hot_vis = 0.0;
    } else {
        float plain_lerp = (light_lerp)/0.3;
        plain_color = glm::mix(plain_night, plain_sunset, plain_lerp);
        gradient_color = glm::mix(gradient_night, gradient_sunset, plain_lerp);
        ambient_color = glm::mix(ambient_night, ambient_sunset, plain_lerp);
        sun_color = glm::mix(sun_night, sun_sunset, plain_lerp);
        sun_alpha = (1.0 - plain_lerp) * sun_alpha_night + plain_lerp * sun_alpha_sunset;
        moon_color = glm::mix(moon_night, moon_sunset, plain_lerp);
        moon_alpha = (1.0 - plain_lerp) * moon_alpha_night + plain_lerp * moon_alpha_sunset;
        stars_alpha = (1.0 - plain_lerp) * stars_alpha_night + plain_lerp * stars_alpha_sunset;
        hot_vis = (1.0 - plain_lerp) * hot_vis_night + plain_lerp * hot_vis_sunset;
    }  

    background_plain_->setMultiplyColor(plain_color/255.0f);
    background_gradient_->setMultiplyColor(gradient_color/255.0f);

    sun_->setColor(sun_color/255.0f);
    sun_->setAlpha(sun_alpha);

    hotaru_->setVisibility(hot_vis);
    moon_->setColor(moon_color/255.0f);
    moon_->setAlpha(moon_alpha);

    stars_->setMultiplyColor(glm::vec4(1.0f, 1.0f, 1.0f, stars_alpha));
    getGraphicsWorld().setAmbientLight(ambient_color/255.0f);  
}