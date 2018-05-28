#include "humannpcagent.h"

#include "game/actor/agent/human/playeragent.h"

#include "game/game.h"
#include "engine/geom/circle.h"
#include "game/interaction/interaction.h"



HumanNPCAgent::HumanNPCAgent() {
	addInteractionShape(new Circle(5));
	addInteraction(new DialogueInteraction("hunter"));
}

void HumanNPCAgent::onStep(float dt) {
	PlayerAgent* player = Game::getInstance().getGameWorld().getPlayer();


	setOrientation(player->getPosition().x < getPosition().x ? LEFT : RIGHT);

	HumanAgent::onStep(dt);

}