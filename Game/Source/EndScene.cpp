#include "EndScene.h"
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

EndScene::EndScene() : Module()
{

}

EndScene::~EndScene()
{

}

bool EndScene::Awake()
{
	LOG("Loading Menu");
	bool ret = true;

	return ret;
}

// Load assets
bool EndScene::Start()
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

	// music and fx
	titleMenuMusic = app->audio->PlayMusic("Assets/Music/Title Screen.ogg", 1.0f);
	selectFx = app->audio->LoadFx("Assets/FX/choose.wav");
	nextFx = app->audio->LoadFx("Assets/FX/press.wav");
	backFx = app->audio->LoadFx("Assets/FX/menu3_back.wav");

	win = false;

	return ret;
}

bool EndScene::PreUpdate(float dt)
{
	return true;
}

bool EndScene::Update(float dt)
{
	bool ret = true;

	//key commands 
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {		// ESC to close the game
		ret = false;
	}
	
	return ret;
}



// Update: draw background
bool EndScene::PostUpdate(float dt)
{
	bool ret = true;

	app->render->DrawTexture(gamemenu, 0, 0);

	if (win) {
		app->fonts->BlitText(640 / 2 - 79, 612 / 2 + 110, menuFont, "You won!");
	}
	else {
		app->fonts->BlitText(640 / 2 - 79, 612 / 2 + 110, menuFont, "You lost!");
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		app->fade->FadeToBlack(this, app->menu);
	}

	return ret;
}

bool EndScene::CleanUp()
{
	LOG("Freeing menu");

	return true;
}

void EndScene::ReStart()
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;
}
