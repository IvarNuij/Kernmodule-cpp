#pragma once

#include <string>

#include "BaseClass.h"

class GameManager;

using std::string;

class SceneManager : public BaseClass {
public:
	SceneManager(GameManager* gameManager);
	void update() override;
	void loadScene(string scene);
	
	string currentScene;

private:
	void unloadCurrentScene();
	void loadMainMenu();
	void updateMainMenu();
	void loadGameOver();
	void loadLvl01();
	void updateLvl01();

	float lvl01EnemyTimer;

	GameManager* gameManager;
};