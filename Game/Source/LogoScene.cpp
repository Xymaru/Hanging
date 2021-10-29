#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "LogoScene.h"
#include "GameScene.h"

#include <iostream>

#include "Defs.h"
#include "Log.h"

LogoScene::LogoScene() : Module()
{
	name.Create("scene");
}

// Destructor
LogoScene::~LogoScene()
{}

// Called before render is available
bool LogoScene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool LogoScene::Start()
{
	logo = app->tex->Load("Assets/Textures/logo.png");
	SDL_SetTextureAlphaMod(logo, 0.0f);

	time = 1.0f;
	timer = 0.0f;
	fadeState = FADE_IN;

	return true;
}

// Called each loop iteration
bool LogoScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool LogoScene::Update(float dt)
{
	app->render->DrawTexture(logo, 0, 0);
	timer += dt;

	switch (fadeState) {
	case FADE_IN:
		SDL_SetTextureAlphaMod(logo, timer * 255);
		if (timer >= time) {
			timer = 0.0f;
			fadeState = FADE_STAY;
		}
		break;
	case FADE_STAY:
		if (timer >= time) {
			timer = 0.0f;
			fadeState = FADE_OUT;
		}
		break;
	case FADE_OUT:
		SDL_SetTextureAlphaMod(logo, time - timer * 255);
		if (timer >= time) {
			timer = 0.0f;

			Deactivate();
			app->gameScene->Activate();
		}
		break;
	}

	return true;
}

// Called each loop iteration
bool LogoScene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool LogoScene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
