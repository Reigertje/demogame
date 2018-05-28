#include "itemactor.h"

#include "game/item/item.h"
#include "game/game.h"

#include "engine/graphics/spritedrawobject.h"
#include "engine/physics/physicsshape.h"
#include "engine/geom/circle.h"
#include "game/interaction/interaction.h"

ItemActor::ItemActor(const std::string& item_key) {
	item_ = Game::getInstance().getResources().findItem(item_key);
	initialize();
}

ItemActor::ItemActor(const Item* item) {
	item_ = item;
	initialize();
}

void ItemActor::initialize() {
	addDrawObject(new SpriteDrawObject(Game::getInstance().getResources().getSprite("forestfire"), 10, 10));
	setPhysicsShape(new PhysicsShape(PhysicsShape::COLLIDER, 5, 5));
	addInteractionShape(new Circle(5));
	addInteraction(new ItemInteraction(this, item_));

	setPosition(vec2f(25, 150));
}


void ItemActor::onStep(float dt) {

}