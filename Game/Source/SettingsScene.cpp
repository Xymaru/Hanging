#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SettingsScene.h"
#include "GameScene.h"
#include "MainMenu.h"
#include "ModuleFadeToBlack.h"
#include "GuiManager.h"
#include "Map.h"

#include <iostream>

#include "Defs.h"
#include "Log.h"

SettingsScene::SettingsScene() : Module()
{
}

// Destructor
SettingsScene::~SettingsScene()
{}

// Called before render is available
bool SettingsScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SettingsScene::Start()
{
	app->guiManager->CleanUp();

	background = app->tex->Load("Assets/Textures/Settingscreen.png");

	SDL_Texture* btn_tex = app->tex->Load("Assets/Textures/UI/back.png");
	app->guiManager->CreateButton(BACK, { 500,560,152,23 }, btn_tex, this);

	btn_tex = app->tex->Load("Assets/Textures/UI/checkbox.png");
	fscreen = app->guiManager->CreateCheckbox(FULLSCREEN, { 200,262,24,24 }, btn_tex, this);
	fscreen->activated = app->render->fullScreen;
	vsync = app->guiManager->CreateCheckbox(VSYNC, { 200,296,24,24 }, btn_tex, this);
	vsync->activated = app->render->vsync;

	btn_tex = app->tex->Load("Assets/Textures/UI/slider.png");
	music = app->guiManager->CreateSlider(MUSIC, { 260,200,180,16 }, { 260, 198, 10, 18 }, btn_tex, this);
	fx = app->guiManager->CreateSlider(FX, { 260,230,180,16 }, { 260, 229, 10, 18 }, btn_tex, this);

	btn_tex = app->tex->Load("Assets/Textures/UI/save.png");
	app->guiManager->CreateButton(SAVE, { 274,350,92,23 }, btn_tex, this);

	return true;
}

// Called each loop iteration
bool SettingsScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SettingsScene::Update(float dt)
{

	return true;
}

// Called each loop iteration
bool SettingsScene::PostUpdate(float dt)
{
	bool ret = true;

	app->render->DrawTexture(background, 0, 0);

	app->guiManager->Draw();

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool SettingsScene::CleanUp()
{
	LOG("Freeing scene");
	app->guiManager->clear = true;

	return true;
}

bool SettingsScene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id) {
	case BACK:
		if (fromGame) {
			app->gameScene->active = true;
			app->map->active = false;

			DisableAndDeactivate();
		}
		else {
			app->fade->FadeToBlack(this, app->menu);
		}
		break;
	case SAVE:
		if (fscreen->activated) {
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			app->render->fullScreen = true;
		}else{
			SDL_SetWindowFullscreen(app->win->window, 0);
			app->render->fullScreen = false;
		}

		if (vsync->activated) {
			app->render->vsync = true;
			SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
		}
		else {
			app->render->vsync = false;
			SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
		}
		break;
	}

	return true;
}