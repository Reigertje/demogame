#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>
#include <map>

class Sprite;
class Game;
class Shader;
class Texture;
class BodyDrawObject;
class BodyPartDrawObject;
class Item;
class LevelScene;
class GameDialogue;

class Resources {

friend class Game;

public:

	Shader* findShader(const std::string& key, bool show_error = true);

	Texture* findTexture(const std::string& key, bool show_error = true);

	Sprite getSprite(const std::string& key);

	BodyDrawObject* getBodyDrawObject(const std::string& key);

	BodyPartDrawObject* getBodyPartDrawObject(const std::string& key);

	GameDialogue* getDialogue(const std::string& key);

	LevelScene* getLevelScene(const std::string& key);

	const Item* findItem(const std::string& key, bool show_error = true);


private:

	void readAnimationDirectory();

	void readShaderDirectory();

	void readImageDirectory();

	void readSpriteSheetDirectory();

	void readItemDirectory();

	void readLevelDirectory();

	void readSpriteSheet(const std::string& path);

	void addShader(const std::string& key, Shader* shader);

	void addTexture(const std::string& key, Texture* texture);

	void addBodyDrawObjectData(const std::string& key, std::string* data);

	void addBodyPartObjectData(const std::string& key, std::string* data);

	void addDialogueData(const std::string& key, std::string* data);

	std::map<std::string, Shader*> shaders_;

	std::map<std::string, Texture*> textures_;

	std::map<std::string, Item*> items_;

	std::map<std::string, std::string*> body_drawobject_data_;

	std::map<std::string, std::string*> bodypart_drawobject_data_;

	std::map<std::string, std::string*> dialogue_data_;

	Resources();

	~Resources();

};


#endif