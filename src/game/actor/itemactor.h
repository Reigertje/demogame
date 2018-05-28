#ifndef ITEMACTOR_H
#define ITEMACTOR_H

#include "actor.h"

class Item;

class ItemActor : public Actor {

public:

	ItemActor(const std::string& item_key);

	ItemActor(const Item* item_);

	void onStep(float dt);

private:

	void initialize();

	const Item* item_;

};


#endif