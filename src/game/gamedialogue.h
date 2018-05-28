#ifndef GAMEDIALOGUE_H
#define GAMEDIALOGUE_H

#include "engine/dialogue/dialogue.h"

class GameDialogue : public Dialogue {

public:

	GameDialogue(const std::vector<DialogueNode*>& nodes) : Dialogue(nodes) {};

	bool testCondition(const DialogueCondition& condition) const;

};

#endif