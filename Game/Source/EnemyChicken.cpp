#include "EnemyChicken.h"
#include "Render.h"
#include "Textures.h"
#include "PlayerModule.h"
#include "Pathfinding.h"
#include "Map.h"
#include <iostream>

void EnemyChicken::Patrol(float dt)
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

void EnemyChicken::Follow(float dt)
{
	iPoint playerPos = app->playerModule->GetPosition();
	float dist = DISTANCE(position.x, position.y, playerPos.x, playerPos.y);

	if (dist > distanceFollow) {
		state = ES_PATROL;
		origin = position;
	}
	else {
		int directionX = playerPos.x - position.x;
		if (directionX != 0) {
			directionX /= abs(directionX);
			int step = moveSpeed * dt;
			step *= directionX;
			position.x += step;
			if (directionX < 0) {
				flip = SDL_FLIP_NONE;
			}
			else {
				flip = SDL_FLIP_HORIZONTAL;
			}
		}
	}
}

void EnemyChicken::Backing(float dt)
{
	
}

void EnemyChicken::MoveTo(iPoint destination, float dt)
{
	
}

void EnemyChicken::CheckClosestIndex()
{
	
}

EnemyChicken::EnemyChicken()
{
}

EnemyChicken::~EnemyChicken()
{
}

void EnemyChicken::Init()
{
	texture = app->tex->Load("Assets/Textures/enemy_animation/chicken.png");

	rect = { 0, 0, 64, 68 };

	collisionWidth = 64;
	collisionHeight = 68;

	Animation idle;
	for (int i = 0; i < 13; i++) {
		idle.PushBack({ i * rect.w, 0, rect.w, rect.h });
	}
	idle.speed = 10.0f;

	animations.Add(idle);

	Animation walk;
	for (int i = 0; i < 14; i++) {
		walk.PushBack({ i * rect.w, 68, rect.w, rect.h });
	}
	walk.speed = 10.0f;

	animations.Add(walk);

	Animation hurt;
	for (int i = 0; i < 5; i++) {
		hurt.PushBack({ i * rect.w, 136, rect.w, rect.h });
	}
	hurt.speed = 25.0f;

	animations.Add(hurt);

	flip = SDL_FLIP_NONE;
	animState = AS_WALK;
	state = ES_PATROL;

	patrolDistance = 100;
	distanceFollow = 200.0f;
	patrolRight = false;
	moveSpeed = 70;

	pathUpdateTime = 1.5f;
	pathUpdateTimer = pathUpdateTime;

	type = EntityModule::EntityType::ET_CHICKEN;

	pathIndex = 0;
}

void EnemyChicken::Update(float dt)
{
	animations[animState].Update(dt);

	switch (state) {
	case ES_PATROL:
		Patrol(dt);
		break;
	case ES_FOLLOW:
		Follow(dt);
		break;
	}
}

void EnemyChicken::Render()
{
	SDL_Rect rect = animations[animState].GetCurrentFrame();

	app->render->DrawTexture(texture, position.x, position.y, &rect, flip);
}

void EnemyChicken::Cleanup()
{
}