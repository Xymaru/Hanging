#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "GameScene.h"
#include "PlayerModule.h"
#include "Map.h"
#include "ModulePhysics.h"

#include "Defs.h"
#include "Log.h"

GameScene::GameScene() : Module()
{
	name.Create("gameScene");
}

// Destructor
GameScene::~GameScene()
{}

// Called before render is available
bool GameScene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool GameScene::Start()
{
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	app->map->Load("hanging.tmx");
	return true;
}

// Called each loop iteration
bool GameScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool GameScene::Update(float dt)
{
	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 1;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 1;

	return true;
}

// Called each loop iteration
bool GameScene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool GameScene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void GameScene::Activate()
{
	Module::Activate();

	app->playerModule->Activate();
	app->map->Activate();
	app->physics->Activate();
}
