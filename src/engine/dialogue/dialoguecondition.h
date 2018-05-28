#ifndef DIALOGUECONDITION_H
#define DIALOGUECONDITION_H

#include <vector>
#include <string>

class Dialogue;

class DialogueCondition {

public:

	// Empty condition is always true
	DialogueCondition();

	DialogueCondition(const std::string& condition);

	DialogueCondition(const std::string& condition, const std::vector<std::string>& arguments);

	std::string getCondition() const;

	std::vector<std::string> getArguments() const;

	bool test(const Dialogue& dialogue);

private:

	std::string condition_;

	std::vector<std::string> arguments_;

};

#endif