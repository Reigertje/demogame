#ifndef DIALOGUENODE_H
#define DIALOGUENODE_H

#include "dialogueoption.h"
#include "dialoguecondition.h"

class Dialogue;

class DialogueNode {

public:

	virtual std::string getDisplayText() const;

	virtual std::vector<DialogueOption> getOptions() const;

	bool hasOptions() const;

	virtual int getSelectedOption() const;

	virtual void selectOption(int option) {}

	virtual int getNextNode() const = 0;

	virtual void onSelect(Dialogue& dialogue) {};
};

class DialogueTextNode : public DialogueNode {

	public:

		DialogueTextNode(const std::string& text, int next_node);

		std::string getDisplayText() const;

		int getNextNode() const;

	private:

		std::string text_;

		int next_node_;

};

class DialogueOptionNode  : public DialogueNode {

	public:

		DialogueOptionNode(const std::string& text, const std::vector<DialogueOption>& options);

		std::vector<DialogueOption> getOptions() const;

		int getSelectedOption() const;

		void selectOption(int option);

		int getNextNode() const;

		std::string getDisplayText() const;

		void onSelect(Dialogue& dialogue);

	private:

		void filterOptions(const Dialogue& dialogue);

		std::string text_;

		int selected_option_;

		std::vector<DialogueOption> options_;

		std::vector<DialogueOption> filtered_options_;

};

class DialogueConditionNode : public DialogueNode {

	public:

		DialogueConditionNode(int next_success, int next_fail,const DialogueCondition& condition);

		int getNextNode() const;

		void onSelect(Dialogue& dialogue);

	private:

		int next_success_;

		int next_fail_;

		int next_node_;

		DialogueCondition condition_;

};

#endif