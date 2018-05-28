#include "levelscene.h"

#include "game/game.h"
#include "game/actor/agent/human/playeragent.h"
#include "game/actor/levelgridactor.h"
#include "game/actor/spriteactor.h"
#include "game/actor/lightparticlesactor.h"
#include "game/actor/itemactor.h"

#include "utils/random.h"
#include <algorithm>
#include <iostream>

LevelScene::LevelScene() {

}

void LevelScene::build(const Grid<bool>& grid, const std::string& theme, const std::vector<Link>& links) {
    level_grid_ = grid;
    onLevelGridGenerated(level_grid_);
    links_ = links;

    for (int i = 0; i < links_.size(); ++i) {
        createLink(links_[i]);
    }

    onLinksCreated(level_grid_, links_);

    addActor(new LevelGridActor(level_grid_, theme));

    getGraphicsWorld().setViewBounds(level_grid_.getWidth() * 48.0f, 99999.f);

}


void LevelScene::spawnPlayer(Actor* player, const std::string& origin) {
    for (int i = 0; i < links_.size(); ++i) {
        if (origin == links_[i].to) {
            player->setPosition(vec2f(links_[i].x * 48 + 24, links_[i].y * 48 + 16));
            addActor(player);
            return;
        }
    }
    player->setPosition(vec2f(links_[0].x * 48 + 24, links_[0].y * 48 + 16));
    addActor(player);
}

std::vector<LevelScene::HorizontalPlatform> LevelScene::getHorizontalPlatforms(const Grid<bool>& grid) {
    std::vector<HorizontalPlatform> result;
    for (int x = 0; x < grid.getWidth(); ++x) {
        for (int y = 0; y < grid.getHeight() - 1; ++y) {
            if (grid[x][y] && !grid[x][y + 1]) {
                if (x > 0 && grid[x - 1][y] && !grid[x - 1][y + 1]) continue;
                int ex = x;
                while (ex + 1 < grid.getWidth() && grid[ex + 1][y] && !grid[ex + 1][y + 1]) {
                    ex++;
                }
                result.push_back(HorizontalPlatform(x, y, ex - x + 1));
            }
        }
    }
    return result;
}

void LevelScene::step(float dt) {
    Scene::step(dt);
}