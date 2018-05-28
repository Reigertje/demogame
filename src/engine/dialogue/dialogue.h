#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <vector>
#include <string>

#include "dialoguenode.h"
#include "dialoguecondition.h"
#include "dialogueoption.h"

class Dialogue {

public:

	static const int END = -1;

	Dialogue(const std::vector<DialogueNode*>& nodes);

	~Dialogue();

	void start();

	void next();

	bool isFinished() const;

	bool hasOptions() const;

	std::string getDisplayText() const;

	std::vector<std::string> getDisplayOptions() const;

	int getSelectedOption() const;

	void selectOption(int option);

	void selectNext();

	void selectPrevious();

	virtual bool testCondition(const DialogueCondition& condition) const = 0;

private:

	int selected_node_;

	void changeNode(int node);

	void displaySelectedNode() const;

	std::vector<DialogueNode*> nodes_; 

};


#endif