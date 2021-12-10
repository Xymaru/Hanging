#include "App.h"
#include "EntityModule.h"
#include "EnemyBird.h"
#include "Coins.h"
#include "Heart.h"
#include "Map.h"
#include "Render.h"

#include <iostream>

#include "Defs.h"
#include "Log.h"

EntityModule::EntityModule() : Module()
{
	name.Create("enemies");
}

// Destructor
EntityModule::~EntityModule()
{}

// Called before render is available
bool EntityModule::Awake(pugi::xml_node& config)
{
	LOG("Loading Enemies");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool EntityModule::Start()
{
	return true;
}

// Called each loop iteration
bool EntityModule::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool EntityModule::Update(float dt)
{
	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next) {
		entity->data->Update(dt);
	}

	return true;
}

// Called each loop iteration
bool EntityModule::PostUpdate(float dt)
{
	bool ret = true;

	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next) {
		entity->data->Render();
	}

	if (app->IsDebugMode()) {
		for (entity = entities.start; entity != NULL; entity = entity->next) {
			EntityType type = entity->data->GetType();
			switch (type) {
				case ET_BIRD:
					EnemyBird* bird = (EnemyBird*)entity->data;
					DrawPath(bird->GetActivePath());
					break;
			}
		}
	}

	return ret;
}

void EntityModule::Deactivate()
{
	Module::Deactivate();

	CleanUp();
}

// Called before quitting
bool EntityModule::CleanUp()
{
	LOG("Freeing enemies");

	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next) {
		entity->data->Cleanup();
		delete entity->data;
	}

	entities.Clear();

	return true;
}

void EntityModule::SaveEntities(pugi::xml_node & entities)
{

}

void EntityModule::LoadEntities(pugi::xml_node & entities)
{
	
}

void EntityModule::AddEntity(EntityType type, iPoint position)
{
	Entity* entity = nullptr;

	switch (type) {
		case ET_BIRD:
		{
			EnemyBird* bird = new EnemyBird();
			bird->SetOrigin(position);

			entity = bird;
		}
		break;
		case ET_COIN:
		{
			Coins* coins = new Coins();

			entity = coins;
		}
		break;
		case ET_HEART:
		{
			Heart* heart = new Heart();

			entity = heart;
		}
		break;
	}

	entity->Init();
	entity->SetPosition(position);
	entities.Add(entity);
}

void EntityModule::DrawPath(const DynArray<iPoint>* path)
{
	uint count = path->Count();

	SDL_Rect rect;

	int tWidth = app->map->mapData.tileWidth;
	int tHeight = app->map->mapData.tileHeight;

	rect.w = tWidth / 2;
	rect.h = tHeight / 2;

	for (uint i = 0; i < count; ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		rect.x = pos.x + rect.w;
		rect.y = pos.y + rect.h;
		app->render->DrawRectangle(rect, 255, 0, 0, 255);
	}
}