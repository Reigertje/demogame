#ifndef DIALOGUEOPTION_H
#define DIALOGUEOPTION_H

#include <string>
#include "dialoguecondition.h"

class DialogueOption {

public:

	DialogueOption(const std::string& text, int next_node, const DialogueCondition& condition);

	DialogueOption(const std::string& text, int next_node);

	std::string getDisplayText() const;

	int getNextNode() const;

	DialogueCondition getCondition() const;

private:

	DialogueCondition condition_;

	std::string text_;

	int next_node_;

};

#endif