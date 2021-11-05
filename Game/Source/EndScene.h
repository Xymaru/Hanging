#ifndef __END_SCENE_H__  // ADRIAN: .H DE TITLESCREEN
#define __END_SCENE_H__

#include "Module.h"
//#include "Animation.h"

struct SDL_Texture;

class EndScene : public Module
{
public:

	EndScene();

	virtual ~EndScene();

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

	// called when reactivated
	void ReStart();
public:
	// Player has won the level
	bool win;

	// Font index
	int menuFont = -1;

	//Menu Buttons

	SDL_Texture* gamemenu;
	// Music and FX
	uint titleMenuMusic = 0;
	uint selectFx = 0;
	uint nextFx = 0;
	uint backFx = 0;
};

#endif
