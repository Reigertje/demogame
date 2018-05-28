#ifndef LINKACTOR_H
#define LINKACTOR_H

#include "actor.h"

/**
	Actor for link to other scene (e.g. doors, cave entrace etc)
**/

class LinkActor : public Actor {

public:

	LinkActor(const std::string& sprite, const std::string& to, int x, int y);

	void onStep(float dt);

};

#endif