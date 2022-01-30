#include "MainMenu.h"  
#include "MainMenu.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "StageSelect.h"
#include "ModuleFadeToBlack.h"
#include "Fonts.h"
#include "PlayerModule.h"
#include "GameScene.h"
#include "Defs.h"
#include "Defs.h"
#include "Log.h"
#include "GuiManager.h"
#include "ControlsScene.h"
#include "CreditsScene.h"
#include "SettingsScene.h"

#include <iostream>

MainMenu::MainMenu() : Module()
{

}

MainMenu::~MainMenu()
{

}

bool MainMenu::Awake(pugi::xml_node& config)
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

	app->playerModule->playerscore = 0;
	app->playerModule->playerhealth = 3;

	/*test = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, "potato", { 10,20,100,50 }, this);
	test->color = { 128,128,0,255 };*/

	/*slider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 0, "yes", { 0,0,100,50 }, this);
	slider->color = { 128,128,0,255 };*/

	/*checkBox = (GuiCheckbox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 0, "yep", { 0,0,24,24 }, this);
	checkBox->color = { 128,128,0,255 };*/

	GuiButton* btn;

	SDL_Texture* btn_tex = app->tex->Load("Assets/Textures/UI/start.png");
	btn = app->guiManager->CreateButton(PLAY, { 260,350,120,23 }, btn_tex, this);

	btn_tex = app->tex->Load("Assets/Textures/UI/continue.png");
	btn = app->guiManager->CreateButton(CONTINUE, { 244,390,152,23 }, btn_tex, this);
	if (!app->gameScene->ExistsSaved()) btn->Disable();

	btn_tex = app->tex->Load("Assets/Textures/UI/settings.png");
	btn = app->guiManager->CreateButton(SETTINGS, { 253,430,134,23 }, btn_tex, this);

	btn_tex = app->tex->Load("Assets/Textures/UI/controls.png");
	btn = app->guiManager->CreateButton(CONTROLS, { 255,470,130,23 }, btn_tex, this);

	btn_tex = app->tex->Load("Assets/Textures/UI/credits.png");
	btn = app->guiManager->CreateButton(CREDITS, { 251,510,138,23 }, btn_tex, this);

	btn_tex = app->tex->Load("Assets/Textures/UI/exit.png");
	btn = app->guiManager->CreateButton(EXIT, { 500,560,120,23 }, btn_tex, this);

	return ret;
}

bool MainMenu::PreUpdate(float dt)
{
	return true;
}

bool MainMenu::Update(float dt)
{
	bool ret = !exit;

	return ret;
}



// Update: draw background
bool MainMenu::PostUpdate(float dt)
{
	bool ret = true;

	app->render->DrawTexture(gamemenu, 0, 0);

	app->guiManager->Draw();

	return ret;
}

bool MainMenu::CleanUp()
{
	LOG("Freeing menu");
	app->guiManager->CleanUp();

	return true;
}

bool MainMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id) {
	case PLAY:
		app->fade->FadeToBlack(this, app->gameScene);
		break;
	case CONTINUE:
		app->gameScene->LoadGameState(this);
		break;
	case CONTROLS:
		app->fade->FadeToBlack(this, app->controlScene);
		break;
	case SETTINGS:
		app->fade->FadeToBlack(this, app->settingsScene);
		break;
	case CREDITS:
		app->fade->FadeToBlack(this, app->creditsScene);
		break;
	case EXIT:
		exit = true;
		break;
	}

	return true;
}
