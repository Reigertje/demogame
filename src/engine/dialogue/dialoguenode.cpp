#include "dialoguenode.h"

#include "dialogue.h"

#include <iostream>

std::string DialogueNode::getDisplayText() const { 
	return "";
}

std::vector<DialogueOption> DialogueNode::getOptions() const { 
	return std::vector<DialogueOption>();
}

bool DialogueNode::hasOptions() const { 
	return getOptions().size() > 0;
}

int DialogueNode::getSelectedOption() const { 
	return -1;
}

// TEXT NODE

DialogueTextNode::DialogueTextNode(const std::string& text, int next_node) 
	: text_(text), next_node_(next_node) {}

std::string DialogueTextNode::getDisplayText() const {
	return text_;
}

int DialogueTextNode::getNextNode() const { 
	return next_node_;
}

// OPTION NODE

DialogueOptionNode::DialogueOptionNode(const std::string& text, const std::vector<DialogueOption>& options) 
	: text_(text), options_(options), selected_option_(0) {}

std::vector<DialogueOption> DialogueOptionNode::getOptions() const {
	return filtered_options_; 
} 

int DialogueOptionNode::getSelectedOption() const { 
	return selected_option_; 
}

void DialogueOptionNode::selectOption(int option) { 
	selected_option_ = option; 
	if (selected_option_ >= filtered_options_.size()) {
		selected_option_ = selected_option_%filtered_options_.size();
	} else if (selected_option_ < 0) {
		selected_option_ = filtered_options_.size() + selected_option_;
	}
}

int DialogueOptionNode::getNextNode() const { 
	return filtered_options_[selected_option_].getNextNode(); 
}

std::string DialogueOptionNode::getDisplayText() const { 
	return text_; 
}

void DialogueOptionNode::filterOptions(const Dialogue& dialogue) {
	filtered_options_.clear();
	for (int i = 0; i < options_.size(); ++i) {
		if (options_[i].getCondition().test(dialogue)) {
			filtered_options_.push_back(options_[i]);
		}
	}
}

void DialogueOptionNode::onSelect(Dialogue& dialogue) {
	filterOptions(dialogue);
}

// CONDITION NODE

DialogueConditionNode::DialogueConditionNode(int next_success, int next_fail, const DialogueCondition& condition) 
	: next_success_(next_success), next_fail_(next_fail), next_node_(next_fail), condition_(condition) {};

int DialogueConditionNode::getNextNode() const {
	return next_node_;
}

void DialogueConditionNode::onSelect(Dialogue& dialogue) {
	next_node_ = condition_.test(dialogue) ? next_success_ : next_fail_;
	dialogue.next();
}