#include "dialogueoption.h"

#include <iostream>

DialogueOption::DialogueOption(const std::string& text, int next_node, const DialogueCondition& condition) : 
	text_(text), next_node_(next_node), condition_(condition) {}

DialogueOption::DialogueOption(const std::string& text, int next_node) : 
	text_(text), next_node_(next_node) {}

std::string DialogueOption::getDisplayText() const { 
	return text_;
}

int DialogueOption::getNextNode() const { 
	return next_node_;
}

DialogueCondition DialogueOption::getCondition() const { 
	return condition_;
}