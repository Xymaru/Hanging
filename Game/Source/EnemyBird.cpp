#include "EnemyBird.h"
#include "Render.h"
#include "Textures.h"
#include "PlayerModule.h"
#include "Pathfinding.h"
#include "Map.h"
#include <iostream>

void EnemyBird::Patrol(float dt)
{
	iPoint playerPos = app->playerModule->GetPosition();
	float dist = DISTANCE(position.x, position.y, playerPos.x, playerPos.y);

	if (dist < distanceFollow) {
		state = BS_FOLLOW;
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
		state = BS_BACKING;
	}
	else {
		pathUpdateTimer += dt;

		if (pathUpdateTimer >= pathUpdateTime) {
			pathUpdateTimer = 0.0f;
			pathIndex = 0;

			iPoint origin = app->map->WorldToMap(position);
			iPoint destination = app->map->WorldToMap(app->playerModule->GetPosition());
			int res = app->pathfinding->CreatePath(origin, destination);

			if (res > 0) {
				path = app->pathfinding->GetLastPath();

				//CheckClosestIndex();
				activeNode = app->map->MapToWorld(path[pathIndex].x, path[pathIndex].y);
			}
		}

		if (path.Count() > 0) {
			if (position == activeNode) {
				pathIndex++;

				if (pathIndex < path.Count()) {
					activeNode = app->map->MapToWorld(path[pathIndex].x, path[pathIndex].y);
				}
			}

			if (pathIndex < path.Count()) {
				MoveTo(activeNode, dt);
			}
		}
	}
}

void EnemyBird::Backing(float dt)
{
	state = BS_PATROL;
	path.Clear();
}

void EnemyBird::MoveTo(iPoint destination, float dt)
{
	iPoint diff = destination - position;

	fPoint dir = { (float)diff.x, (float)diff.y };
	dir.Normalize();
	dir *= moveSpeed * 2;

	iPoint step = { int(dir.x * dt), int(dir.y * dt) };

	//std::cout << step.x << "," << step.y << std::endl;

	position.x += step.x;
	position.y += step.y;
}

void EnemyBird::CheckClosestIndex()
{
	uint size = path.Count();

	float indexDist;
	float activeDist;

	iPoint dest = app->map->MapToWorld(path[size - 1]);

	for (uint i = 0; i < size; i++) {
		iPoint indexO = app->map->MapToWorld(path[i]);
		
		indexDist = DISTANCE(indexO.x, indexO.y, dest.x, dest.y);
		activeDist = DISTANCE(position.x, position.y, dest.x, dest.y);

		if (indexDist < activeDist) {
			pathIndex = i;
			break;
		}
	}
}

EnemyBird::EnemyBird()
{
}

EnemyBird::~EnemyBird()
{
}

void EnemyBird::Init(Module* module)
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
	state = BS_PATROL;

	patrolDistance = 80;
	distanceFollow = 500.0f;
	patrolRight = false;
	moveSpeed = 80;

	pathUpdateTime = 800.0f;
	pathUpdateTimer = pathUpdateTime;

	type = EntityModule::EntityType::ET_BIRD;

	pathIndex = 0;
}

void EnemyBird::Update(float dt)
{
	animations[animState].Update(dt);

	switch (state) {
		case BS_PATROL:
			Patrol(dt);
			break;
		case BS_FOLLOW:
			Follow(dt);
			break;
		case BS_BACKING:
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

void EnemyBird::SetState(int newState)
{
	Entity::SetState(newState);

	switch (newState) {
	case BS_PATROL:
	case BS_FOLLOW:
	case BS_BACKING:
		animState = AS_FLY;
		break;
	case BS_DYING:
	case BS_DEAD:
		animState = AS_HURT;
		break;
	}
}