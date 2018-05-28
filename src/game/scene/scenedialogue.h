#ifndef SCENEDIALOGUE_H
#define SCENEDIALOGUE_H

#include <vector>
#include "engine/graphics/textdrawobject.h"

class GameDialogue;
class Scene;

class SceneDialogue {

public:

	SceneDialogue();

	~SceneDialogue();

	void addToScene(Scene& scene);

	void startDialogue(const std::string& key);

	bool isActive() const;

	void step(float dt);

private:

	void onNextNode();

	void updateOptions();

	GameDialogue* dialogue_;

	TextDrawObject* display_text_;

	std::vector<TextDrawObject> display_options_;

};


#endif