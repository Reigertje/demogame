#include "linkactor.h"

#include "engine/graphics/spritedrawobject.h"
#include "game/game.h"

#include "engine/geom/rectangle.h"
#include "game/interaction/interaction.h"


LinkActor::LinkActor(const std::string& sprite_key, const std::string& to, int x, int y) {
	// TODO positie fixen
	Sprite sprite = Game::getInstance().getResources().getSprite(sprite_key);

	addDrawObject(new SpriteDrawObject(sprite));
	addInteractionShape(new Rectangle(5, 5));
	addInteraction(new LinkInteraction(this, to));

	setPosition(vec2f(x + 24, y + sprite.getPixelHeight() / 2));
}

void LinkActor::onStep(float dt) {

}