#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "PlayerModule.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>

PlayerModule::PlayerModule() : Module()
{
	name.Create("PlayerModule");
}

// Destructor
PlayerModule::~PlayerModule()
{}

// Called before render is available
bool PlayerModule::Awake()
{
	LOG("Loading PlayerModule");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool PlayerModule::Start()
{
	playerTex = app->tex->Load("Assets/Textures/player.png");
	app->render->camera.x = 0;
	position.x = cameraBound;
	position.y = 0;
	offsetX = 145;
	moveSpeed = 1;
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	return true;
}

// Called each loop iteration
bool PlayerModule::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool PlayerModule::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		position.x -= moveSpeed;

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		position.x += moveSpeed;

	// Camera control
	int diff = position.x - app->render->camera.x;

	/*if (diff < cameraBound) {
		app->render->camera.x -= moveSpeed;
	}
	else if (diff > app->render->camera.w - cameraBound) {
		app->render->camera.x += moveSpeed;
	}*/

	return true;
}

// Called each loop iteration
bool PlayerModule::PostUpdate()
{
	bool ret = true;

	// Draw character
	app->render->DrawTexture(playerTex, position.x - offsetX, position.y, NULL, SDL_FLIP_NONE);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool PlayerModule::CleanUp()
{
	LOG("Freeing playerModule");
	return true;
}
