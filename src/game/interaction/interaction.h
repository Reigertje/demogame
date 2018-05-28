#ifndef INTERACTION_H
#define INTERACTION_H

#include <string>
#include <iostream>

#include "../actor/actor.h"

class ItemActor;
class LinkActor;
class Item;

class Interaction {

public:

	static const int ITEM = 0;
	static const int LINK = 1;
	static const int DIALOGUE = 2;

	Interaction(int type);

	int getType() const; 

	void performInteraction(Actor& actor);

	virtual void onPerformed(Actor& actor) = 0;

	virtual std::string getDescription() const = 0;

private:

	int type_;

};

class ItemInteraction : public Interaction {

public:

	ItemInteraction(ItemActor* item_actor, const Item* item) : Interaction(ITEM), item_actor_(item_actor), item_(item) {};

	std::string getDescription() const;

	const Item* getItem() const;

private:

	void onPerformed(Actor& actor);

	ItemActor* item_actor_;

	const Item* item_;

};

class LinkInteraction : public Interaction {

public:

	LinkInteraction(LinkActor* link_actor, const std::string& to) : Interaction(LINK), to_(to), link_actor_(link_actor) {};

	std::string getDescription() const;

	std::string getDestination() const;

private:

	void onPerformed(Actor& actor);

	LinkActor* link_actor_;

	std::string to_;

};

class DialogueInteraction : public Interaction {

public:

	DialogueInteraction(const std::string& key) : Interaction(DIALOGUE), key_(key) {};

	std::string getKey() const;

	std::string getDescription() const;

private:

	void onPerformed(Actor& actor);

	std::string key_;

};

#endif