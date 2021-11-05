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
#include "EndScene.h"

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
bool GameScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool GameScene::Start()
{
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	gameLevel = GameLevel::Level1;

	return true;
}

// Called each loop iteration
bool GameScene::PreUpdate(float dt)
{
	return true;
}

// Called each loop iteration
bool GameScene::Update(float dt)
{
	return true;
}

// Called each loop iteration
bool GameScene::PostUpdate(float dt)
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

	app->physics->Activate();
	app->playerModule->Activate();
	app->map->Activate();

	InitMapLevel();
}

void GameScene::Deactivate()
{
	Module::Deactivate();

	app->playerModule->Deactivate();
	app->map->Deactivate();
	app->physics->Deactivate();
}

void GameScene::InitMapLevel()
{
	app->map->mapData.Clear();

	switch (gameLevel) {
		case GameLevel::Level1:
			app->map->Load("level_1.tmx");
			break;
		case GameLevel::Level2:
			app->map->Load("level_2.tmx");
			break;
	}

	// Physworld box
	MapData* mapData = &app->map->mapData;
	uint size = mapData->layers.Count();

	MapLayer* collisionLayer = NULL;

	for (uint i = 0; i < size; i++) {
		if (mapData->layers[i]->name == "Collisions") {
			collisionLayer = mapData->layers[i];
			break;
		}
	}

	int tileWidth = app->map->mapData.tileWidth;
	int tileHeight = app->map->mapData.tileHeight;

	PhysBody* pbody;

	for (int y = 0; y < collisionLayer->height; y++) {
		for (int x = 0; x < collisionLayer->width; x++) {
			ColliderLayerType colType = (ColliderLayerType)collisionLayer->Get(x, y);
			
			switch (colType) {
				case ColliderLayerType::SPAWN:
					app->playerModule->SetPosition(x * tileWidth, y * tileHeight  - tileHeight / 2);
					break;
				case ColliderLayerType::END:
					pbody = app->physics->CreateRectangle(x * tileWidth + tileWidth / 2, y * tileHeight + tileHeight / 2, tileWidth, tileHeight, false);
					pbody->bodyType = PhysBodyType::END;
					break;
				case ColliderLayerType::NORMAL:
					pbody = app->physics->CreateRectangle(x * tileWidth + tileWidth / 2, y * tileHeight + tileHeight / 2, tileWidth, tileHeight, false);
					pbody->bodyType = PhysBodyType::GROUND;
					break;
				case ColliderLayerType::SPIKE:
					pbody = app->physics->CreateRectangle(x * tileWidth + tileWidth / 2, y * tileHeight + tileHeight / 2, tileWidth, tileHeight, false);
					pbody->bodyType = PhysBodyType::SPIKES;
					break;
				case ColliderLayerType::PLATFORM:
					pbody = app->physics->CreateRectangle(x * tileWidth + tileWidth / 2, y * tileHeight + tileHeight / 4, tileWidth, tileHeight / 2, false);
					pbody->bodyType = PhysBodyType::PLATFORM;
					break;
			}
		}
	}
}

void GameScene::ReStart()
{
	app->endScene->win = false;
}
