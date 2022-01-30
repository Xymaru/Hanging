#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "Module.h"

struct SDL_Texture;

class GameScene : public Module
{
public:

	GameScene();

	enum GameLevel {
		Level1,
		Level2,
		None
	};

	// Destructor
	virtual ~GameScene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// Called to activate module
	void Activate();

	// called to deactivate module
	void Deactivate();

	void InitMapLevel();

	GameLevel gameLevel;

	uint bg_music;

	bool loadGame;

	void LoadGameState(Module* caller=nullptr);
	void SaveGameState();

	bool ExistsSaved();

	bool OnGuiMouseClickEvent(GuiControl* control);
private:
	enum GuiID {
		RESUME,
		SETTINGS,
		MENU,
		EXIT
	};

	bool fromGameSaved = false;

	iPoint playerPosition;
	int playerState;
	iPoint cameraPosition;

	bool paused = false;

	SDL_Texture* pause_screen;

	bool exit = false;
	bool unpause = false;

	bool goSettings = false;
	bool reloadPause = false;

	void LoadPauseScreen();
	void UnloadPauseScreen();
};

#endif // __GAME_SCENE_H__