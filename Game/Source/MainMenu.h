#ifndef __MAIN_MENU_H__  // ADRIAN: .H DE TITLESCREEN
#define __MAIN_MENU_H__

#include "Module.h"
//#include "Animation.h"

struct SDL_Texture;

class MainMenu : public Module
{
public:

	MainMenu();

	virtual ~MainMenu();

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


public:
	int p_x = 0;
	int p_y = 0;

	//SDL_Rect p_pos;

	// Font index
	int menuFont = -1;

	//Menu Buttons
	const int B_Play = 2;
	const int B_Coop = 1;
	const int B_Exit = 0;

	int M_Index = 2;

	SDL_Texture* gamemenu;
	// Music and FX
	uint titleMenuMusic = 0;
	uint selectFx = 0;
	uint nextFx = 0;
	uint backFx = 0;
};

#endif