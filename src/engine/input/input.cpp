#include "input.h"

#include <iostream>

Input::~Input() {
	for (std::pair<int, ActionKey*> pair : keycode_map_) {
		delete pair.second;
	}
	// TODO remove actions
}

bool Input::isKeyDown(int action) const {
	std::map<int, ActionKey*>::const_iterator it = action_map_.find(action);
	if (it != action_map_.end()) return it->second->is_down_;
	return false; 
}

bool Input::wasKeyPressed(int action) const {
	std::map<int, ActionKey*>::const_iterator it = action_map_.find(action);
	if (it != action_map_.end()) return it->second->was_pressed_;
	return false; 
}

bool Input::wasKeyReleased(int action) const {
	std::map<int, ActionKey*>::const_iterator it = action_map_.find(action);
	if (it != action_map_.end()) return it->second->was_released_;
	return false; 
}

void Input::press(int keycode) {
	std::map<int, ActionKey*>::iterator it = keycode_map_.find(keycode);
	if (it != keycode_map_.end()) {
		it->second->was_pressed_ = true;
		it->second->is_down_ = true;
	}
}

void Input::release(int keycode) { 
	std::map<int, ActionKey*>::iterator it = keycode_map_.find(keycode);
	if (it != keycode_map_.end()) {
		it->second->is_down_ = false;
		it->second->was_released_ = true;
	}
}

void Input::flush() {
	for (std::pair<int, ActionKey*> pair : keycode_map_) {
		pair.second->was_pressed_ = false;
		pair.second->was_released_ = false;
	}
}

void Input::addActionKey(int action, int key) {
	ActionKey* new_key = new ActionKey();
	std::map<int, ActionKey*>::iterator action_it = action_map_.find(action);

	if (action_it != action_map_.end()) {
		std::cerr << "Error : Conflicting action in input" << std::endl;
		exit(-1);
	}

	std::map<int, ActionKey*>::iterator keycode_it = keycode_map_.find(key);

	if (keycode_it != keycode_map_.end()) {
		std::cerr << "Error : conflicting keycode in input" << std::endl;
		exit(-1);
	}

	action_map_[action] = new_key;
	keycode_map_[key] = new_key;
}
