#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "CreditsScene.h"
#include "GameScene.h"
#include "MainMenu.h"
#include "ModuleFadeToBlack.h"
#include "GuiManager.h"

#include <iostream>

#include "Defs.h"
#include "Log.h"

CreditsScene::CreditsScene() : Module()
{
	name.Create("creditsscene");
}

// Destructor
CreditsScene::~CreditsScene()
{}

// Called before render is available
bool CreditsScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool CreditsScene::Start()
{
	background = app->tex->Load("Assets/Textures/Creditscreen.png");

	SDL_Texture* btn_tex = app->tex->Load("Assets/Textures/UI/back.png");
	app->guiManager->CreateButton(0, { 500,560,152,23 }, btn_tex, this);

	return true;
}

// Called each loop iteration
bool CreditsScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool CreditsScene::Update(float dt)
{
	return true;
}

// Called each loop iteration
bool CreditsScene::PostUpdate(float dt)
{
	bool ret = true;

	app->render->DrawTexture(background, 0, 0);

	app->guiManager->Draw();

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool CreditsScene::CleanUp()
{
	LOG("Freeing scene");
	app->guiManager->CleanUp();

	return true;
}

bool CreditsScene::OnGuiMouseClickEvent(GuiControl* control)
{
	app->fade->FadeToBlack(this, app->menu);

	return true;
}
