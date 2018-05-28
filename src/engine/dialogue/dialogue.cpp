#include "dialogue.h"
#include <stdlib.h>

#include <iostream>

Dialogue::Dialogue(const std::vector<DialogueNode*>& nodes) : nodes_(nodes) {
	selected_node_ = END;
	//changeNode(0);
}

Dialogue::~Dialogue() {
	for (int i = 0; i < nodes_.size(); ++i) delete nodes_[i];
}

void Dialogue::start() {
	changeNode(0);
}

void Dialogue::displaySelectedNode() const {
	if (selected_node_ == END) {
		std::cout << "Dialogue finished!" << std::endl;
	} else {
		std::cout << nodes_[selected_node_]->getDisplayText() << std::endl;
	}
}

void Dialogue::changeNode(int node) {
	selected_node_ = node;
	if (selected_node_ != END) nodes_[selected_node_]->onSelect(*this);
}

void Dialogue::next() {
	if (selected_node_ != END) {
		changeNode(nodes_[selected_node_]->getNextNode());
	}
}

bool Dialogue::isFinished() const {
	return selected_node_ == END;
}

bool Dialogue::hasOptions() const {
	return selected_node_ != END && nodes_[selected_node_]->hasOptions();
}

std::string Dialogue::getDisplayText() const {
	return selected_node_ == END ? "" : nodes_[selected_node_]->getDisplayText();
}

std::vector<std::string> Dialogue::getDisplayOptions() const {
	if (selected_node_ == END) return std::vector<std::string>();
	std::vector<DialogueOption> options = nodes_[selected_node_]->getOptions();
	std::vector<std::string> result;
	for (int i = 0; i < options.size(); ++i) {
		result.push_back(options[i].getDisplayText());
	}
	return result;
}

int Dialogue::getSelectedOption() const {
	return selected_node_ == END ? -1 : nodes_[selected_node_]->getSelectedOption();
}

void  Dialogue::selectOption(int option) {
	if (selected_node_ != END) nodes_[selected_node_]->selectOption(option);
}

void Dialogue::selectNext() {
	selectOption(getSelectedOption() + 1);
}

void Dialogue::selectPrevious() {
	selectOption(getSelectedOption() - 1);
}