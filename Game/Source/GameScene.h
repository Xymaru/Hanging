#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "Module.h"

struct SDL_Texture;

class GameScene : public Module
{
public:

	GameScene();

	// Destructor
	virtual ~GameScene();

	// Called before render is available
	bool Awake();

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

	void InitMapLevel();

private:

};

#endif // __GAME_SCENE_H__