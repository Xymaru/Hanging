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
#include "GuiManager.h"
#include "GuiButton.h"
#include <iostream>

EndScene::EndScene() : Module()
{

}

EndScene::~EndScene()
{

}

bool EndScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Menu");
	bool ret = true;

	win = false;

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

	SDL_Texture* btn_tex = app->tex->Load("Assets/Textures/UI/menu.png");
	app->guiManager->CreateButton(0, { 251,350,138,23 }, btn_tex, this);

	btn_tex = app->tex->Load("Assets/Textures/UI/exit.png");
	app->guiManager->CreateButton(1, { 260,380,120,23 }, btn_tex, this);

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

	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->render->DrawTexture(gamemenu, 0, 0);

	if (win) {
		app->fonts->BlitText(640 / 2 - 79, 612 / 2 + 110, menuFont, "you won!");
		app->fonts->BlitText(640 / 2 - 79, 612 / 2 + 130, menuFont, "your score ");
		sprintf_s(livesText, 10, "%5d", app->playerModule->playerscore);
		app->fonts->BlitText(640 / 2 , 612 / 2 + 130, menuFont, livesText);
	}
	else {
		app->fonts->BlitText(640 / 2 - 79, 612 / 2 + 110, menuFont, "you lost!");
		app->fonts->BlitText(640 / 2 - 79, 612 / 2 + 130, menuFont, "your score ");
		sprintf_s(livesText, 10, "%5d", app->playerModule->playerscore);
		app->fonts->BlitText(640 / 2 , 612 / 2 + 130, menuFont, livesText);
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		app->fade->FadeToBlack(this, app->menu);
	}

	app->guiManager->Draw();

	if (exit) return false;

	return ret;
}

bool EndScene::CleanUp()
{
	LOG("Freeing menu");
	app->guiManager->CleanUp();

	return true;
}

void EndScene::ReStart()
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;
}

bool EndScene::OnGuiMouseClickEvent(GuiControl* control) {
	if (control->id == 0) {
		app->fade->FadeToBlack(this, app->menu);
	}
	else {
		exit = true;
	}

	return true;
}