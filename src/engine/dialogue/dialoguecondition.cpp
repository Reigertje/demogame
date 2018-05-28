#include "dialoguecondition.h"

#include "dialogue.h"

#include <iostream>

DialogueCondition::DialogueCondition() 
	: condition_("") {}

DialogueCondition::DialogueCondition(const std::string& condition) 
	: condition_(condition) {}

DialogueCondition::DialogueCondition(const std::string& condition, const std::vector<std::string>& arguments) 
	: condition_(condition), arguments_(arguments) {}

std::string DialogueCondition::getCondition() const { 
	return condition_; 
}

std::vector<std::string> DialogueCondition::getArguments() const { 
	return arguments_; 
}

bool DialogueCondition::test(const Dialogue& dialogue) {
	return condition_.length() == 0 || dialogue.testCondition(*this);
}