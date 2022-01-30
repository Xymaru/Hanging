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
	checkpoint_active = false;

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
	ListItem<Entity*>* entity = entities.start;

	while (entity) {

		entity->data->Update(dt);

		if (entity->data->Remove()) {
			ListItem<Entity*>* prev = entity->prev;

			entity->data->Cleanup();
			entities.Del(entity);

			entity = prev;
		}

		if (entity) {
			entity = entity->next;
		}
	}

	return true;
}

// Called each loop iteration
bool EntityModule::PostUpdate(float dt)
{
	bool ret = true;

	ListItem<Entity*>* entity;

	Render();

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

void EntityModule::SaveEntities(pugi::xml_node & entities_node)
{
	// Clear previous saved entities
	pugi::xml_node entity_node = entities_node.first_child();

	while (entity_node) {
		entities_node.remove_child(entity_node);
		entity_node = entities_node.first_child();
	}

	// Add new entities
	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next) {
		Entity* ent = entity->data;

		entity_node = entities_node.append_child("entity");

		entity_node.append_attribute("id") = ent->GetId();
		entity_node.append_attribute("type") = (int)ent->GetType();

		iPoint pos = ent->GetPosition();
		entity_node.append_attribute("x") = pos.x;
		entity_node.append_attribute("y") = pos.y;

		entity_node.append_attribute("state") = ent->GetState();
	}
}

void EntityModule::LoadEntities(pugi::xml_node & entities_node)
{
	pugi::xml_node entity_node = entities_node.first_child();

	while (entity_node) {
		int id = entity_node.attribute("id").as_int();
		EntityType type = (EntityType)entity_node.attribute("type").as_int();
		iPoint pos;
		pos.x = entity_node.attribute("x").as_int();
		pos.y = entity_node.attribute("y").as_int();

		int state = entity_node.attribute("state").as_int();

		Entity* ent = AddEntity(id, type, pos);
		ent->SetState(state);

		entity_node = entity_node.next_sibling();
	}
}

Entity* EntityModule::AddEntity(int id, EntityType type, iPoint position)
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

	return entity;
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
					if (check->GetState() == Checkpoint::CheckpointState::S_OFF) {
						check->Check();
						checkpoint_active = true;
					}
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
					if (chicken->GetState() != EnemyChicken::ChickenState::CS_HURT && chicken->GetState() != EnemyChicken::ChickenState::CS_DEAD) {
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

	if (bodyA->bodyType == PhysBodyType::BIRD) {
		if (bodyB->bodyType == PhysBodyType::SPIKES) {
			app->playerModule->playerscore += 200;

			ListItem<Entity*>* entity;
			entity = entities.start;

			while (entity != NULL)
			{
				if (entity->data->GetId() == bodyA->id) {
					EnemyBird* bird = (EnemyBird*)entity->data;
					if (bird->GetState() != EnemyBird::BirdState::BS_DYING && bird->GetState() != EnemyBird::BirdState::BS_DEAD) {
						bird->Die();
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

void EntityModule::Render()
{
	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next) {
		entity->data->Render();
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