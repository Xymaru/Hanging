#include "App.h"
#include "EntityModule.h"
#include "EnemyBird.h"
#include "Coins.h"
#include "Heart.h"
#include "Checkpoint.h"
#include "EnemyChicken.h"
#include "Map.h"
#include "Render.h"
#include "PlayerModule.h"

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

		if (entity->data->Remove()) {
			ListItem<Entity*>* prev = entity->prev;

			entity->data->Cleanup();
			entities.Del(entity);

			entity = prev;
		}
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

void EntityModule::AddEntity(int id, EntityType type, iPoint position)
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
		case ET_CHICKEN:
		{
			EnemyChicken* chicken = new EnemyChicken();
			chicken->SetOrigin(position);

			entity = chicken;
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
		case ET_CHECKPOINT:
		{
			Checkpoint* checkpoint = new Checkpoint();

			entity = checkpoint;
		}
		break;
	}

	entity->SetPosition(position);
	entity->SetId(id);
	entity->Init(this);
	entities.Add(entity);
}

void EntityModule::OnCollision(PhysBody * bodyA, PhysBody * bodyB)
{
	if (bodyB->bodyType == PhysBodyType::PLAYER) {
		if (bodyA->bodyType == PhysBodyType::COIN) {
			app->playerModule->playerscore += 100;

			ListItem<Entity*>* entity;
			entity = entities.start;

			while (entity != NULL)
			{
				if (entity->data->GetId() == bodyA->id) {
					entity->data->Cleanup();
					entities.Del(entity);
					break;
				}
				else {
					entity = entity->next;
				}
			}
		}
		if (bodyA->bodyType == PhysBodyType::HEART) {
			app->playerModule->playerhealth++;

			ListItem<Entity*>* entity;
			entity = entities.start;

			while (entity != NULL)
			{
				if (entity->data->GetId() == bodyA->id) {
					entity->data->Cleanup();
					entities.Del(entity);
					break;
				}
				else {
					entity = entity->next;
				}
			}
		}
		if (bodyA->bodyType == PhysBodyType::CHECKPOINT) {
			ListItem<Entity*>* entity;
			entity = entities.start;
			while (entity != NULL)
			{
				if (entity->data->GetId() == bodyA->id) {
					Checkpoint* check = (Checkpoint*)entity->data;
					check->Check();
					break;
				}
				else {
					entity = entity->next;
				}
			}
		}
	}

	if (bodyA->bodyType == PhysBodyType::CHICKEN) {
		if (bodyB->bodyType == PhysBodyType::SPIKES) {
			app->playerModule->playerscore += 200;

			ListItem<Entity*>* entity;
			entity = entities.start;

			while (entity != NULL)
			{
				if (entity->data->GetId() == bodyA->id) {
					EnemyChicken* chicken = (EnemyChicken*)entity->data;
					if (chicken->GetState() != EnemyChicken::EnemyState::ES_HURT && chicken->GetState() != EnemyChicken::EnemyState::ES_DEAD) {
						chicken->Die();
					}

					break;
				}
				else {
					entity = entity->next;
				}
			}
		}
	}
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