#include "EnemyBird.h"
#include "Render.h"
#include "Textures.h"
#include "PlayerModule.h"
#include "Pathfinding.h"
#include "Map.h"

void EnemyBird::Patrol(float dt)
{
	iPoint playerPos = app->playerModule->GetPosition();
	float dist = DISTANCE(position.x, position.y, playerPos.x, playerPos.y);

	if (dist < distanceFollow) {
		state = ES_FOLLOW;
	}
	else {
		int step = moveSpeed * dt;
		if (patrolRight) {
			position.x += step;
		}
		else {
			position.x -= step;
		}

		if (position.x < origin.x - patrolDistance) {
			patrolRight = true;
			flip = SDL_FLIP_HORIZONTAL;
		}
		else if (position.x > origin.x + patrolDistance) {
			patrolRight = false;
			flip = SDL_FLIP_NONE;
		}
	}
}

void EnemyBird::Follow(float dt)
{
	iPoint playerPos = app->playerModule->GetPosition();
	float dist = DISTANCE(position.x, position.y, playerPos.x, playerPos.y);

	if (dist > distanceFollow) {
		state = ES_BACKING;
	}
	else {
		pathUpdateTimer += dt;

		if (pathUpdateTimer >= 1.0f) {
			iPoint origin = app->map->WorldToMap(position);
			iPoint destination = app->map->WorldToMap(app->playerModule->GetPosition());
			int res = app->pathfinding->CreatePath(origin, destination);

			if (res > 0) {
				path = app->pathfinding->GetLastPath();
			}

			pathUpdateTimer = 0.0f;
		}
	}
}

void EnemyBird::Backing(float dt)
{
	
}

EnemyBird::EnemyBird()
{
}

EnemyBird::~EnemyBird()
{
}

void EnemyBird::Init()
{
	texture = app->tex->Load("Assets/Textures/enemy_animation/bluebird.png");

	rect = { 0, 0, 64, 64 };

	collisionWidth = 64;
	collisionHeight = 64;

	Animation fly;
	for (int i = 0; i < 9; i++) {
		fly.PushBack({ i * rect.w, 0, rect.w, rect.h });
	}
	fly.speed = 10.0f;

	animations.Add(fly);

	Animation hurt;
	for (int i = 0; i < 5; i++) {
		hurt.PushBack({ i * rect.w, 64, rect.w, rect.h });
	}
	hurt.speed = 25.0f;

	animations.Add(hurt);

	flip = SDL_FLIP_NONE;
	animState = AS_FLY;
	state = ES_PATROL;

	patrolDistance = 80;
	distanceFollow = 500.0f;
	patrolRight = false;
	moveSpeed = 80;

	pathUpdateTimer = 1.0f;

	type = EntityModule::EntityType::ET_BIRD;
}

void EnemyBird::Update(float dt)
{
	animations[animState].Update(dt);

	switch (state) {
		case ES_PATROL:
			Patrol(dt);
			break;
		case ES_FOLLOW:
			Follow(dt);
			break;
		case ES_BACKING:
			Backing(dt);
			break;
	}
}

void EnemyBird::Render()
{
	SDL_Rect rect = animations[animState].GetCurrentFrame();

	app->render->DrawTexture(texture, position.x, position.y, &rect, flip);
}

void EnemyBird::Cleanup()
{
}