#include "resources.h"

#include "utils/utils.h"

#include "engine/graphics/shader.h"
#include "engine/graphics/image.h"

#include "game/gamedialogue.h"

#include "engine/graphics/sprite.h"
#include "engine/graphics/frameanimation.h"
#include "engine/graphics/bodydrawobject.h"
#include "engine/graphics/spritedrawobject.h"
#include "game/item/item.h"
#include "game/actor/agent/agentstatus.h"

#include "game/actor/agent/human/humanagent.h"

#include <algorithm>
#include <vector>
#include <iostream>
#include <exception>
#include <cmath>
#include "lib/json.hpp"

using json = nlohmann::json;

using namespace utils;

static std::string generateKey(const std::string& folder, const std::string& path) {
	std::size_t lastDot = path.find_last_of('.');
	std::string result = path.substr(folder.size() + 1, lastDot - (folder.size() + 1));
	std::replace(result.begin(), result.end(), '/', '.');
	return result;
}

template <typename T>
static T* findResource(const std::string& key, const std::string& type, std::map<std::string, T*>& map, bool show_error = true) {
	typename std::map<std::string, T*>::iterator it = map.find(key);
	if (it != map.end()) return it->second;
	if (show_error) std::cerr << "Could not find resource " << type << "." << key;
	return NULL;
}

template<typename T> 
static void addResources(const std::string& folder, std::map<std::string, T*>& map, T* (*func)(const std::string&)) {
	std::vector<std::string> fileList = getFilesInFolder(folder, true);
	for (int i = 0; i < fileList.size(); ++i) {
		map.insert(std::pair<std::string, T *>(generateKey(folder, fileList[i]), func(fileList[i])));
	}
}

template<typename T>
static void deleteResources(std::map<std::string, T*>& map) {
	for(typename std::map<std::string, T*>::iterator it = map.begin(); it != map.end(); it++) {
		if (it->second) delete it->second;
	}
}

Shader* Resources::findShader(const std::string& key, bool show_error) {
	return findResource(key, "shader", shaders_, show_error);
}

Texture* Resources::findTexture(const std::string& key, bool show_error) {
	return findResource(key, "texture", textures_, show_error);
}

Sprite Resources::getSprite(const std::string& key) {
	Texture* diffuse = findTexture(key);
	Texture* normal = findTexture(key + ".n", false);
	return Sprite(diffuse ? diffuse : findTexture("_"), normal ? normal : findTexture("_.n"));
}

void Resources::addShader(const std::string& key, Shader* shader) {
	shaders_.insert(std::pair<std::string, Shader*>(key, shader));
}

void Resources::addTexture(const std::string& key, Texture* texture) {
	textures_.insert(std::pair<std::string, Texture*>(key, texture));
}

void Resources::readShaderDirectory() {

	addShader("default", new Shader(readFileToString("res/shader/default.vert"), readFileToString("res/shader/default.frag")));
	addShader("fbo_default", new Shader(readFileToString("res/shader/fbo_default.vert"), readFileToString("res/shader/fbo_default.frag")));
	addShader("plain", new Shader(readFileToString("res/shader/plain.vert"), readFileToString("res/shader/plain.frag")));
	addShader("rays", new Shader(readFileToString("res/shader/rays.vert"), readFileToString("res/shader/rays.frag")));
	

	for (std::pair<std::string, Shader*> pair : shaders_) {
		pair.second->load();
	}
}

static Texture* createImage(const std::string& path) {
	return new Image(path); 
}

void Resources::readImageDirectory() {
	addResources("res/img", textures_, createImage);

	for (std::pair<std::string, Texture*> pair : textures_) {
		pair.second->load();
	}
}

static std::string* readToStringPointer(const std::string& path) {
	return new std::string(readFileToString(path));
} 

void Resources::readAnimationDirectory() {
	addResources("res/anim/body", body_drawobject_data_, readToStringPointer);
	addResources("res/anim/part", bodypart_drawobject_data_, readToStringPointer);
}


void Resources::readSpriteSheetDirectory() {
	std::vector<std::string> fileList = getFilesInFolder("res/sheet", true);
	for (int i = 0; i < fileList.size(); ++i) {
		readSpriteSheet(fileList[i]);
	}
}

void Resources::readSpriteSheet(const std::string& file) {
	std::string data = readFileToString(file);
	json data_json = json::parse(data);

	try {
		std::string texture_key = data_json["texture_key"];
		Texture* t = findTexture(texture_key, true);
		std::vector<json> sub_textures = data_json["sub_textures"];
		for (int i = 0; i < sub_textures.size(); ++i) {
			std::string key = sub_textures[i]["key"];
			int x = sub_textures[i]["x"];
			int y = sub_textures[i]["y"];
			int width = sub_textures[i]["width"]; 
			int height = sub_textures[i]["height"];

			addTexture(texture_key + "." + key, new SubTexture(t, x, y, width, height));
		}
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

}

BodyDrawObject* Resources::getBodyDrawObject(const std::string& key) {
	std::string* data_string = findResource(key, "body", body_drawobject_data_);
	if (data_string) {
		try {

			BodyDrawObject* result = new BodyDrawObject();
			json data_json = json::parse(*data_string);
			std::vector<json> json_animations = data_json["animations"];
			for (int i = 0; i < json_animations.size(); ++i) {
				std::string name = json_animations[i]["name"];
				int frameCount = json_animations[i]["frameCount"];
				bool repeat = json_animations[i]["repeat"];
				result->addFrameAnimation(name, FrameAnimation(frameCount, repeat));
			}
			std::vector<json> json_links = data_json["links"];
			for (int i = 0; i < json_links.size(); ++i) {
				result->addLink(json_links[i]["a"], json_links[i]["b"]);
			}
			std::vector<json> json_splits = data_json["splits"];
			for (int i = 0; i < json_splits.size(); ++i) {
				std::string animation = json_splits[i]["animation"];
				std::vector<std::string> split_names = json_splits[i]["names"];
				std::vector<int> split_offsets = json_splits[i]["split"];
				for (int j = 0; j < split_names.size(); ++j) {
					int start = split_offsets[j];
					int end = j < split_names.size() - 1 ? split_offsets[j + 1] : result->getFrameCount(animation);
					result->addAlias(animation, animation + "." + split_names[j], split_offsets[j]);
					result->addFrameAnimation(animation + "." + split_names[j], FrameAnimation(end - start, false));
				}
			}
			return result;
		} catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
	return NULL;
}

BodyPartDrawObject* Resources::getBodyPartDrawObject(const std::string& key) {
	std::string* data_string = findResource(key, "part", bodypart_drawobject_data_);
	if (data_string) {
		try {
			std::map<std::string, std::vector<BodyPartDrawObject::Frame*>> animation_map;
			json data_json = json::parse(*data_string);
			float depth = data_json["depth"];
			std::vector<json> json_animations = data_json["animations"];
			for (int i = 0; i < json_animations.size(); ++i) {
				std::vector<BodyPartDrawObject::Frame*> frames;
				std::string name = json_animations[i]["name"];
				std::vector<json> json_frames = json_animations[i]["frames"];
				for (int j = 0; j < json_frames.size(); ++j) {

					if (json_frames[j].is_null()) {
						frames.push_back(NULL);
					} else {
						
						SpriteDrawObject* spritedrawobject = NULL;
						if (!json_frames[j]["image"].is_null()) {
							std::string image = json_frames[j]["image"];
							Sprite t = getSprite(image);
							spritedrawobject = new SpriteDrawObject(t, t.getPixelWidth()/2 , t.getPixelHeight()/2 , t.getPixelWidth(), t.getPixelHeight());
						}
						
						int x = json_frames[j]["x"];
						int y = json_frames[j]["y"];

						//SpriteDrawObject* spritedrawobject = new SpriteDrawObject(t, x + t.getPixelWidth()/2 , y + t.getPixelHeight()/2 , t.getPixelWidth(), t.getPixelHeight());
						
						if (spritedrawobject && json_frames[j]["depth"].is_null()) {
							spritedrawobject->setDrawLayer(depth);
						} else if (spritedrawobject) {
							spritedrawobject->setDrawLayer(json_frames[j]["depth"]);
						}
						
						frames.push_back(new BodyPartDrawObject::Frame(spritedrawobject, glm::vec2(x, y)));
					}
					
				}	
				animation_map.insert(std::pair<std::string, std::vector<BodyPartDrawObject::Frame*>>(name, frames));
			}
			return new BodyPartDrawObject(animation_map);
		} catch (std::exception& e) {

			std::cout << key << e.what() << std::endl;
		}
	}
	return NULL;
}

void Resources::readItemDirectory() {
	items_.insert(std::pair<std::string, Item*>("test", new StatusItem("TestItem", AgentStatus())));

	std::vector<std::string> keys;
	keys.push_back("human.sword_simple");

	std::vector<std::string> keys2;
	keys2.push_back("human.shield_wood");

	items_.insert(std::pair<std::string, Item*>("weapontest", new MeleeWeaponItem("Sword", keys, Weapon::Timing(0.1f, 0.2f, 0.1f))));
	items_.insert(std::pair<std::string, Item*>("shieldtest", new ShieldItem("Shield", keys2)));
}

const Item* Resources::findItem(const std::string& key, bool show_error) {
	return findResource(key, "item", items_, show_error);
}

void Resources::readLevelDirectory() {
	addResources("res/level/dialogue", dialogue_data_, readToStringPointer);
}

GameDialogue* Resources::getDialogue(const std::string& key) {
	std::string* data_string = findResource(key, "dialogue", dialogue_data_);
	if (data_string) {
		try {
			json data_json = json::parse(*data_string);
			std::vector<json> nodes_data = data_json["nodes"];
			std::vector<DialogueNode*> nodes;
			for (int i = 0; i < nodes_data.size(); ++i) {
				std::string node_type = nodes_data[i]["type"];
				if (node_type == "text") {
					nodes.push_back(new DialogueTextNode(nodes_data[i]["text"], nodes_data[i]["next"]));
				} else if (node_type == "option") {
					std::vector<DialogueOption> options;
					std::vector<json> options_data = nodes_data[i]["options"];
					for (int j = 0; j < options_data.size(); ++j) {
						options.push_back(DialogueOption(options_data[j]["text"], options_data[j]["next"]));
					}
					nodes.push_back(new DialogueOptionNode(nodes_data[i]["text"], options));
				} else if (node_type == "condition") {
					nodes.push_back(new DialogueConditionNode(nodes_data[i]["next_success"], nodes_data[i]["next_fail"], DialogueCondition(nodes_data[i]["condition"], nodes_data[i]["arguments"])));
				} else {
					// TODO invalid type
				}
			}

			return new GameDialogue(nodes);
		} catch (std::exception& e) {
			std::cout << key << e.what() << std::endl;
		}
	}
	return 0;
}

Resources::Resources() {
	readShaderDirectory();
	readImageDirectory();
	readSpriteSheetDirectory();
	readAnimationDirectory();
	readItemDirectory();
	readLevelDirectory();
}

Resources::~Resources() {
	deleteResources(shaders_);
	deleteResources(textures_);
	deleteResources(body_drawobject_data_);
	deleteResources(bodypart_drawobject_data_);
	deleteResources(items_);
	deleteResources(dialogue_data_);
}