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
#include "ModuleFadeToBlack.h"
#include "Map.h"
#include "Pathfinding.h"
#include "EntityModule.h"

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

	gameLevel = GameLevel::Level1;
	
	return ret;
}

// Called before the first frame
bool GameScene::Start()
{
	//bg_music = app->audio->LoadFx("Assets/Audio/Music/bg.ogg");

	if (fromGameSaved) {
		app->render->camera.x = cameraPosition.x;
		app->render->camera.y = cameraPosition.y;
	}
	else {
		app->render->camera.x = 0;
		app->render->camera.y = 0;
	}

	app->endScene->win = false;

	//app->audio->PlayMusic("Assets/Audio/Music/bg.ogg", 1.0f);
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
	app->audio->PlayFx(bg_music, 2);
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		app->fade->FadeToBlack(this, this);
	}

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		SaveGameState();
	}

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		LoadGameState();
	}

	/*if (app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) {
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);
		iPoint p = app->render->ScreenToWorld(mouseX, mouseY);
		p = app->map->WorldToMap(p.x, p.y);

		iPoint o = app->playerModule->GetPosition();

		iPoint origin = app->map->WorldToMap(o.x, o.y);
		origin.y += 1;
		iPoint destination = p;

		app->pathfinding->CreatePath(origin, destination);
	}*/

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
	app->enemies->Activate();

	InitMapLevel();
}

void GameScene::Deactivate()
{
	Module::Deactivate();

	app->playerModule->Deactivate();
	app->map->Deactivate();
	app->physics->Deactivate();
	app->enemies->Deactivate();
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
	MapData* mapData = &app->map->mapData;

	// Physworld box
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

	for (int y = 0; y < collisionLayer->height; y++)
	{
		for (int x = 0; x < collisionLayer->width; x++)
		{
			ColliderLayerType colType = (ColliderLayerType)collisionLayer->Get(x, y);
			
			switch (colType)
			{
				case ColliderLayerType::SPAWN:
					if (!fromGameSaved)
					{
						app->playerModule->SetPosition(x * tileWidth, y * tileHeight - tileHeight / 2);
					}
					else
					{
						app->playerModule->SetPosition(playerPosition.x, playerPosition.y);
						app->playerModule->SetState(playerState);
					}
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
					pbody = app->physics->CreateRectangle(x * tileWidth + tileWidth / 2, y * tileHeight + tileHeight / 2 + 3, tileWidth, tileHeight - 6, false);
					pbody->bodyType = PhysBodyType::SPIKES;
					break;
				case ColliderLayerType::PLATFORM:
					pbody = app->physics->CreateRectangle(x * tileWidth + tileWidth / 2, y * tileHeight + tileHeight / 4, tileWidth, tileHeight / 2, false);
					pbody->bodyType = PhysBodyType::PLATFORM;
					break;
			}
		}
	}

	// Spawn enemies
	if (!fromGameSaved) {
		uint groupCount = mapData->objectGroups.Count();

		ObjectGroup* enemyGroup = NULL;

		for (uint i = 0; i < groupCount; i++) {
			if (mapData->objectGroups[i]->name == "Entities") {
				enemyGroup = mapData->objectGroups[i];
				break;
			}
		}

		if (enemyGroup) {
			uint enemyCount = enemyGroup->objects.Count();

			for (uint i = 0; i < enemyCount; i++) {
				Object* obj = enemyGroup->objects[i];

				iPoint position = { obj->x, obj->y };

				app->enemies->AddEntity(obj->id, (EntityModule::EntityType)obj->type, position);
			}
		}
	}

	// Init pathfinding map
	int width, height;
	uchar* buffer;

	if (app->map->CreateWalkabilityMap(width, height, &buffer)) {
		app->pathfinding->SetMap(width, height, buffer);
		RELEASE_ARRAY(buffer);
	}

	fromGameSaved = false;
}

void GameScene::SaveGameState()
{
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("save_game.xml");

	pugi::xml_node save_node;
	pugi::xml_node level;
	pugi::xml_node player;
	pugi::xml_node camera;
	pugi::xml_node entities;

	if (result == NULL)
	{
		pugi::xml_node declaration = saveFile.append_child(pugi::node_declaration);
		declaration.append_attribute("version") = "1.0";

		save_node = saveFile.append_child("game_state");

		level = save_node.append_child("level");
		level.append_attribute("value");

		player = save_node.append_child("player");
		player.append_attribute("x");
		player.append_attribute("y");
		player.append_attribute("state");

		camera = save_node.append_child("camera");
		camera.append_attribute("x");
		camera.append_attribute("y");

		entities = save_node.append_child("entities");
	}
	else {
		save_node = saveFile.child("game_state");
		level = save_node.child("level");
		player = save_node.child("player");
		camera = save_node.child("camera");
		entities = save_node.child("entities");
	}

	level.attribute("value") = gameLevel;

	app->enemies->SaveEntities(entities);
	app->playerModule->SavePlayer(player);

	camera.attribute("x") = app->render->camera.x;
	camera.attribute("y") = app->render->camera.y;

	saveFile.save_file("save_game.xml");
}

void GameScene::LoadGameState()
{
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("save_game.xml");

	if (result != NULL)
	{
		pugi::xml_node save_node = saveFile.child("game_state");

		pugi::xml_node level = save_node.child("level");
		gameLevel = (GameScene::GameLevel)level.attribute("value").as_int();

		pugi::xml_node player = save_node.child("player");
		playerPosition.x = player.attribute("x").as_int();
		playerPosition.y = player.attribute("y").as_int();
		playerState = player.attribute("state").as_int();
		
		pugi::xml_node camera = save_node.child("camera");
		cameraPosition.x = camera.attribute("x").as_int();
		cameraPosition.y = camera.attribute("y").as_int();

		fromGameSaved = true;

		app->enemies->LoadEntities(save_node.child("entities"));

		app->fade->FadeToBlack(this, this);
	}
}