#include "scenedialogue.h"

#include "game/game.h"
#include "game/scene/scene.h"

#include "game/gamedialogue.h"
#include "engine/graphics/textdrawobject.h"

#include <iostream>

// TODO iets voor verzinnen
static bool flag = true;

SceneDialogue::SceneDialogue() {
	dialogue_ = 0;

	display_text_ = new TextDrawObject(Game::getInstance().getResources().getSprite("font"), 2, 35);
	display_text_->setVisible(false);
	display_text_->setDrawBufferIndex(GameRenderer::DRAWBUFFER_GUI);
	display_text_->setAnimationEnabled(true);
	display_text_->setPosition(60, 20);
	// TODO magic number
	for (int i = 0; i < 3; ++i) {
		display_options_.push_back(TextDrawObject(Game::getInstance().getResources().getSprite("font"), 1, 35));
		display_options_[i].setPosition(60, 31 + (i * 8));
		display_options_[i].setAnimationEnabled(false);
		display_options_[i].setDrawBufferIndex(GameRenderer::DRAWBUFFER_GUI);
		display_options_[i].setVisible(false);
	}

}


SceneDialogue::~SceneDialogue() {
	delete display_text_;
	if (dialogue_) delete dialogue_;
}

void SceneDialogue::addToScene(Scene& scene) {
	scene.addDrawObject(display_text_);
	for (int i = 0; i < display_options_.size(); ++i) {
		scene.addDrawObject(&display_options_[i]);
	}
}

void SceneDialogue::startDialogue(const std::string& key) {

	dialogue_ = Game::getInstance().getResources().getDialogue(key);
	dialogue_->start();

	display_text_->setVisible(true);
	onNextNode();
	flag = true;
}

bool SceneDialogue::isActive() const {
	return dialogue_ != 0;
}

void SceneDialogue::updateOptions() {
	if (dialogue_ && dialogue_->hasOptions()) {
		std::vector<std::string> options = dialogue_->getDisplayOptions();

		for (int i = options.size() - 1, display_idx = 0; i >= 0; --i, display_idx++) {
			display_options_[display_idx].setVisible(true);
			display_options_[display_idx].setText(dialogue_->getSelectedOption() == i ? ">" + options[i] : options[i]);
		} 
		for (int i = display_options_.size() - 1; i >= options.size(); --i) {
			display_options_[i].setVisible(false);
		}
	}
}

void SceneDialogue::onNextNode() {
	if (dialogue_->isFinished()) {
		delete dialogue_;
		dialogue_ = 0;
		display_text_->setVisible(false);
	} else {
		display_text_->setText(dialogue_->getDisplayText());
		//updateOptions();
	}
	for (int i = 0; i < display_options_.size(); ++i) {
		display_options_[i].setVisible(false);
	}
}

void SceneDialogue::step(float dt) {
	if (isActive()) {

		display_text_->stepAnimation(dt);

		if (!flag && Game::getInstance().getInput().wasKeyPressed(Game::ACTION_INTERACT)) {

			if (display_text_->isAnimationFinished()) {
				if (display_text_->hasNextPage()) {
					display_text_->nextPage();
				} else {
					dialogue_->next();
					onNextNode();
				}
			} else {
				display_text_->finishAnimation();
			}
		}
		if (display_text_->isAnimationFinished() && dialogue_ && dialogue_->hasOptions() && !display_options_[0].isVisible()) {
			updateOptions();
		}
		if (dialogue_ && dialogue_->hasOptions()) {
			if (display_text_->isAnimationFinished() && !display_options_[0].isVisible()) {
				updateOptions();
			}

			if (Game::getInstance().getInput().wasKeyPressed(Game::ACTION_DOWN)) {
				dialogue_->selectNext();
				updateOptions();
			} else if (Game::getInstance().getInput().wasKeyPressed(Game::ACTION_UP)) {
				dialogue_->selectPrevious();
				updateOptions();
			}

		}

	}
	if (flag) {
		flag = false;
	}
}