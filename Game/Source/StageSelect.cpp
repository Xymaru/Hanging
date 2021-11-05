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
#include "StageSelect.h"
#include "GameScene.h"
#include "Defs.h"
#include "Defs.h"
#include "Log.h"
#include <iostream>


StageSelect::StageSelect() : Module()
{

}

StageSelect::~StageSelect()
{

}

bool StageSelect::Awake(pugi::xml_node& config)
{
	LOG("Loading StageSelector");
	bool ret = true;

	return ret;
}

// Load assets
bool StageSelect::Start()
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
	nextFx = app->audio->LoadFx("Assets/FX/press.wav");
	backFx = app->audio->LoadFx("Assets/FX/menu3_back.wav");

	return ret;
}

bool StageSelect::PreUpdate(float dt)
{
	return true;
}

bool StageSelect::Update(float dt)
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
			app->gameScene->gameLevel = app->gameScene->Level1;
			app->audio->PlayFx(nextFx);
			app->fade->FadeToBlack(this, app->gameScene);
		}
		if (M_Index == B_Coop) {
			app->gameScene->gameLevel = app->gameScene->Level2;
			app->audio->PlayFx(nextFx);
			app->fade->FadeToBlack(this, app->gameScene);
		}
		if (M_Index == B_Exit)
		{
			app->fade->FadeToBlack(this, app->menu);
			app->audio->PlayFx(backFx);
		}
		else {
			app->audio->PlayFx(backFx);
		}
	}

	return ret;
}



// Update: draw background
bool StageSelect::PostUpdate(float dt)
{
	bool ret = true;
	app->render->DrawTexture(gamemenu, 0, 0);
	app->render->DrawTexture(Pointer, p_x, p_y);
	app->fonts->BlitText(640 / 2 - 79, 612 / 2 + 110, menuFont, "play level1");
	app->fonts->BlitText(640 / 2 - 79, 612 / 2 + 140, menuFont, "play level2");
	app->fonts->BlitText(640 / 2 - 79, 612 / 2 + 170, menuFont, "back to menu");

	return ret;
}

bool StageSelect::CleanUp()
{
	LOG("Freeing StageSelector");

	return true;
}