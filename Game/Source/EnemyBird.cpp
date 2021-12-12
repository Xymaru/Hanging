#include "EnemyBird.h"
#include "Render.h"
#include "Textures.h"
#include "PlayerModule.h"
#include "Pathfinding.h"
#include "Map.h"
#include "Window.h"
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

		if (position.x < originPos.x - patrolDistance) {
			patrolRight = true;
			flip = SDL_FLIP_HORIZONTAL;
		}
		else if (position.x > originPos.x + patrolDistance) {
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
		pathIndex = 0;

		iPoint origin = app->map->WorldToMap(position);
		iPoint destination = app->map->WorldToMap(originPos);
		int res = app->pathfinding->CreatePath(origin, destination);

		if (res > 0) {
			path = app->pathfinding->GetLastPath();
			activeNode = app->map->MapToWorld(path[pathIndex].x, path[pathIndex].y);
		}

		pathUpdateTimer = pathUpdateTime;
		
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

				CheckClosestIndex();
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
	iPoint playerPos = app->playerModule->GetPosition();
	float dist = DISTANCE(position.x, position.y, playerPos.x, playerPos.y);

	if (dist < distanceFollow) {
		pathIndex = 0;
		state = BS_FOLLOW;
	}
	else {
		if (position == activeNode) {
			pathIndex++;

			if (pathIndex < path.Count()) {
				activeNode = app->map->MapToWorld(path[pathIndex].x, path[pathIndex].y);
			}
		}

		if (pathIndex < path.Count()) {
			MoveTo(activeNode, dt);
		}
		else {
			state = BS_PATROL;
			path.Clear();
		}
	}
}

void EnemyBird::Dying(float dt)
{
	b2Vec2 box_pos = entityBody->body->GetPosition();
	position.x = METERS_TO_PIXELS(box_pos.x) - rect.w / 2;
	position.y = METERS_TO_PIXELS(box_pos.y) - rect.h / 2;

	if (position.y >= app->win->getWindowHeight()) {
		state = BirdState::BS_DEAD;
		remove = true;
	}
}

void EnemyBird::MoveTo(iPoint destination, float dt)
{
	iPoint diff = destination - position;

	fPoint dir = { (float)diff.x, (float)diff.y };
	dir.Normalize();
	dir *= moveSpeed * dt * 2;

	iPoint step = { int(dir.x), int(dir.y) };

	position.x += step.x;
	position.y += step.y;

	// FIX HORIZONTAL
	if (step.x < 0) {
		if (position.x < destination.x) {
			position.x = destination.x;
		}

		flip = SDL_FLIP_NONE;
	}
	else if (step.x > 0) {
		if (position.x > destination.x) {
			position.x = destination.x;
		}

		flip = SDL_FLIP_HORIZONTAL;
	}

	//FIX VERTICAL
	if (step.y < 0) {
		if (position.y < destination.y) {
			position.y = destination.y;
		}
	}
	else if (step.y > 0) {
		if (position.y > destination.y) {
			position.y = destination.y;
		}
	}
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
	distanceFollow = 200.0f;
	patrolRight = false;
	moveSpeed = 80;

	pathUpdateTime = 0.98f;
	pathUpdateTimer = pathUpdateTime;

	type = EntityModule::EntityType::ET_BIRD;

	pathIndex = 0;

	entityBody = app->physics->CreateRectangleSensor(position.x + rect.w / 2, position.y + rect.h / 2, rect.w, rect.h, true);
	entityBody->bodyType = PhysBodyType::BIRD;
	entityBody->listener = module;
	entityBody->id = entityId;
	entityBody->remove = false;
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
		case BS_DYING:
			Dying(dt);
			break;
	}

	b2Vec2 box_pos;
	box_pos.x = PIXEL_TO_METERS(position.x + rect.w / 2);
	box_pos.y = PIXEL_TO_METERS(position.y + rect.h / 2);

	entityBody->body->SetTransform(box_pos, 0);
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

void EnemyBird::Die()
{
	state = BirdState::BS_DYING;
	animState = AS_HURT;

	entityBody->body->SetLinearVelocity(b2Vec2_zero);

	if (flip == SDL_FLIP_NONE) {
		entityBody->body->ApplyForceToCenter(b2Vec2(150.0f, -150.0f), true);
	}
	else {
		entityBody->body->ApplyForceToCenter(b2Vec2(-150.0f, -150.0f), true);
	}
}