#include "EnemyChicken.h"
#include "Render.h"
#include "Textures.h"
#include "PlayerModule.h"
#include "Pathfinding.h"
#include "Map.h"
#include "Window.h"
#include <iostream>

void EnemyChicken::Patrol(float dt)
{
	iPoint playerPos = app->playerModule->GetPosition();
	float dist = DISTANCE(position.x, position.y, playerPos.x, playerPos.y);

	if (dist < distanceAware) {
		state = ChickenState::CS_FOLLOW;
	}
	else {
		int step = moveSpeed * dt;
		
		if (patrolRight) {
			position.x += step * 2;
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
		state = ChickenState::CS_PATROL;
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

void EnemyChicken::Hurt(float dt)
{
	if (position.y >= app->win->getWindowHeight()) {
		state = ChickenState::CS_DEAD;
		remove = true;
	}
}

EnemyChicken::EnemyChicken()
{
}

EnemyChicken::~EnemyChicken()
{
}

void EnemyChicken::Init(Module* module)
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
	state = ChickenState::CS_PATROL;

	patrolDistance = 90;
	distanceFollow = 300.0f;
	distanceAware = 200.0f;
	patrolRight = false;
	moveSpeed = 70;

	entityBody = app->physics->CreateRectangle(position.x + rect.w / 2, position.y + rect.h / 2, rect.w, rect.h, true);
	entityBody->body->SetFixedRotation(true);
	entityBody->body->SetSleepingAllowed(false);
	entityBody->bodyType = PhysBodyType::CHICKEN;
	entityBody->listener = module;
	entityBody->id = entityId;
	entityBody->remove = false;

	type = EntityModule::EntityType::ET_CHICKEN;
}

void EnemyChicken::Update(float dt)
{
	b2Vec2 box_pos = entityBody->body->GetPosition();
	position.x = METERS_TO_PIXELS(box_pos.x) - rect.w / 2;
	position.y = METERS_TO_PIXELS(box_pos.y) - rect.h / 2;

	animations[animState].Update(dt);

	switch (state) {
	case ChickenState::CS_PATROL:
		Patrol(dt);
		break;
	case ChickenState::CS_FOLLOW:
		Follow(dt);
		break;
	case ChickenState::CS_HURT:
		Hurt(dt);
		break;
	}

	box_pos.x = PIXEL_TO_METERS(position.x + rect.w / 2);
	box_pos.y = PIXEL_TO_METERS(position.y + rect.h / 2);

	entityBody->body->SetTransform(box_pos, 0);
}

void EnemyChicken::Render()
{
	SDL_Rect rect = animations[animState].GetCurrentFrame();

	app->render->DrawTexture(texture, position.x, position.y, &rect, flip);
}

void EnemyChicken::Die()
{
	state = ChickenState::CS_HURT;
	animState = AS_HURT;

	entityBody->body->SetLinearVelocity(b2Vec2_zero);

	if (flip == SDL_FLIP_NONE) {
		entityBody->body->ApplyForceToCenter(b2Vec2(150.0f, -150.0f), true);
	}
	else {
		entityBody->body->ApplyForceToCenter(b2Vec2(-150.0f, -150.0f), true);
	}

	entityBody->body->GetFixtureList()->SetSensor(true);
}

void EnemyChicken::SetState(int newState)
{
	Entity::SetState(newState);

	switch (newState) {
		case CS_PATROL:
		case CS_FOLLOW:
			animState = AS_WALK;
			break;
		case CS_HURT:
		case CS_DEAD:
			animState = AS_HURT;
			break;
	}
}
