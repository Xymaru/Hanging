#include "MainMenu.h"  
#include "MainMenu.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "ModuleFadeToBlack.h"
#include "Fonts.h"
#include "PlayerModule.h"
#include "Defs.h"
#include "Defs.h"
#include "Log.h"
#include <iostream>

MainMenu::MainMenu() : Module()
{

}

MainMenu::~MainMenu()
{

}

bool MainMenu::Awake()
{
	LOG("Loading Menu");
	bool ret = true;

	return ret;
}

// Load assets
bool MainMenu::Start()
{
	LOG("Loading background assets");

	bool ret = true;
	//title 

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	//p_pos.x = p_x;
	//p_pos.y = p_y;

	char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	menuFont = app->fonts->Load("Assets/Fonts/rtype_font3.png", lookupTable, 2);

	menu = app->tex->Load("Assets/Textures/MainMenu.png");
	// music and fx
	titleMenuMusic = app->audio->PlayMusic("Assets/Music/Title Screen.ogg", 1.0f);
	selectFx = app->audio->LoadFx("Assets/FX/choose.wav");
	nextFx =app->audio->LoadFx("Assets/FX/press.wav");
	backFx = app->audio->LoadFx("Assets/FX/menu3_back.wav");

	return ret;
}

bool MainMenu::PreUpdate()
{
	return true;
}

bool MainMenu::Update(float dt)
{
	bool ret = true;

	//key commands 
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		if (M_Index < 2)
		{
			app->audio->PlayFx(selectFx);
			M_Index++;
			p_y -= 13;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		if (M_Index > 0)
		{
			app->audio->PlayFx(selectFx);
			M_Index--;
			p_y += 13;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (M_Index == B_Play)
		{
			app->audio->PlayFx(nextFx);
			app->fade->FadeToBlack(this, (Module*)app->gameScene, 90);
		}
		if (M_Index == B_Exit)
		{
			ret = false;
		}
		else {
			app->audio->PlayFx(backFx);
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {		// ESC to close the game
		ret = false;
	}

	return ret;
}



// Update: draw background
bool MainMenu::PostUpdate()
{
	bool ret = true;
	// Draw everything --------------------------------------
	app->render->DrawTexture(menu, 0, 0);
	//app->render->Blit(Pointer, p_x, p_y, NULL);

	/*app->fonts->BlitText(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 + 33, menuFont, "play game");
	app->fonts->BlitText(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 + 46, menuFont, "coop mode");
	app->fonts->BlitText(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 + 59, menuFont, "exit game");*/

	return ret;
}

bool MainMenu::CleanUp()
{
	LOG("Freeing menu");

	return true;
}

void MainMenu::Activate()
{
	Module::Activate();

	
}