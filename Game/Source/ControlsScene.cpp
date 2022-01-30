#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "ControlsScene.h"
#include "GameScene.h"
#include "MainMenu.h"
#include "ModuleFadeToBlack.h"
#include "GuiManager.h"

#include <iostream>

#include "Defs.h"
#include "Log.h"

ControlsScene::ControlsScene() : Module()
{
}

// Destructor
ControlsScene::~ControlsScene()
{}

// Called before render is available
bool ControlsScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool ControlsScene::Start()
{
	background = app->tex->Load("Assets/Textures/Controlscreen.png");

	SDL_Texture* btn_tex = app->tex->Load("Assets/Textures/UI/back.png");
	app->guiManager->CreateButton(0, { 500,560,152,23 }, btn_tex, this);

	return true;
}

// Called each loop iteration
bool ControlsScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ControlsScene::Update(float dt)
{

	return true;
}

// Called each loop iteration
bool ControlsScene::PostUpdate(float dt)
{
	bool ret = true;

	app->render->DrawTexture(background, 0, 0);

	app->guiManager->Draw();

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool ControlsScene::CleanUp()
{
	LOG("Freeing scene");
	app->guiManager->CleanUp();

	return true;
}

bool ControlsScene::OnGuiMouseClickEvent(GuiControl* control)
{
	app->fade->FadeToBlack(this, app->menu);

	return true;
}