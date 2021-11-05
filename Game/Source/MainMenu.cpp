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
#include "GameScene.h"
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

	gamemenu = app->tex->Load("Assets/Textures/MainMenu.png");
	gamecontrol = app->tex->Load("Assets/Textures/gamecontrol.png");
	Pointer = app->tex->Load("Assets/Textures/main_menu_selector.png");
	
	// music and fx
	titleMenuMusic = app->audio->PlayMusic("Assets/Music/Title Screen.ogg", 1.0f);
	selectFx = app->audio->LoadFx("Assets/FX/choose.wav");
	nextFx =app->audio->LoadFx("Assets/FX/press.wav");
	backFx = app->audio->LoadFx("Assets/FX/menu3_back.wav");

	return ret;
}

bool MainMenu::PreUpdate(float dt)
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
			p_y -= 30;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		if (M_Index > 0)
		{
			app->audio->PlayFx(selectFx);
			M_Index--;
			p_y += 30;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (M_Index == B_Play)
		{
			app->audio->PlayFx(nextFx);
			app->fade->FadeToBlack(this, app->gameScene, 2.0f);
		}
		if (M_Index == B_Coop) {
			display = true;
		}
		if (M_Index == B_Exit)
		{
			ret = false;
		}
		else {
			app->audio->PlayFx(backFx);
		}
	}
	if (display == false) {
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {		// ESC to close the game
			ret = false;
		}
	}
	if (display == true) {
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {		// ESC to close the game
			display = false;
		}
	}

	return ret;
}



// Update: draw background
bool MainMenu::PostUpdate(float dt)
{
	bool ret = true;
	if (display == false) {
		app->render->DrawTexture(gamemenu, 0, 0);
		app->render->DrawTexture(Pointer, p_x, p_y);
		app->fonts->BlitText(640 / 2 - 79, 612 / 2 + 110, menuFont, "play game");
		app->fonts->BlitText(640 / 2 - 79, 612 / 2 + 140, menuFont, "game control");
		app->fonts->BlitText(640 / 2 - 79, 612 / 2 + 170, menuFont, "exit game");
	}
	if (display == true) {
		app->render->DrawTexture(gamecontrol, 0, 0);
		app->fonts->BlitText(640 / 2 - 79, 612 / 2 - 100, menuFont, "a < go left");
		app->fonts->BlitText(640 / 2 - 79, 612 / 2 - 80, menuFont, "d < go right");
		app->fonts->BlitText(640 / 2 - 79, 612 / 2 - 60, menuFont, "space < jump");
		app->fonts->BlitText(640 / 2 - 79, 612 / 2 - 40, menuFont, "f1 < debug key");
		app->fonts->BlitText(640 / 2 - 79, 612 / 2 - 20, menuFont, "f2 < god mod");
		app->fonts->BlitText(640 / 2 - 79, 612 / 2 , menuFont, "press esc < back to menu");
	}

	return ret;
}

bool MainMenu::CleanUp()
{
	LOG("Freeing menu");

	return true;
}