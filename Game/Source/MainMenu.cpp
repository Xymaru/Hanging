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

	checkBox = (GuiCheckbox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 0, "yep", { 0,0,24,24 }, this);
	checkBox->color = { 128,128,0,255 };

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
			app->fade->FadeToBlack(this, app->stage);
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
		app->fonts->BlitText(640 / 2 - 100, 612 / 2 - 100, menuFont, "a d < move space < jump");
		app->fonts->BlitText(640 / 2 - 100, 612 / 2 - 80, menuFont, "f1 f2 < play level1 and 2");
		app->fonts->BlitText(640 / 2 - 100, 612 / 2 - 60, menuFont, "f5 < save game f6 < load game");
		app->fonts->BlitText(640 / 2 - 100, 612 / 2 - 40, menuFont, "f9 < view colliders");
		app->fonts->BlitText(640 / 2 - 100, 612 / 2 - 20, menuFont, "f10 < god mod");
		app->fonts->BlitText(640 / 2 - 100, 612 / 2 , menuFont, "press esc < back to menu");
	}

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
	printf("test\n");

	return true;
}
