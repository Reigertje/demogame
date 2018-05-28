#ifndef INPUT_H
#define INPUT_H

#include <map>

class Input {

public:

	~Input();

	bool isKeyDown(int action) const;

	bool wasKeyPressed(int action) const;

	bool wasKeyReleased(int action) const;

	void press(int keycode);

	void release(int keycode);

	void addActionKey(int action, int key);

	void flush();

private:

	struct ActionKey {
		ActionKey() : is_down_(false), was_pressed_(false) {};
		bool is_down_;
		bool was_pressed_;
		bool was_released_;
	};

	std::map<int, ActionKey*> action_map_;
	std::map<int, ActionKey*> keycode_map_;
};


#endif