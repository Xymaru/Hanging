#include "Checkpoint.h"
#include "Render.h"
#include "Textures.h"
#include "PlayerModule.h"
#include "GameScene.h"
#include "Map.h"

Checkpoint::Checkpoint()
{
}

Checkpoint::~Checkpoint()
{
}

void Checkpoint::Init(Module* module)
{
	texture = app->tex->Load("Assets/Textures/Checkpoint/checkpoint.png");

	rect = { 0, 0, 28, 63 };

	collisionWidth = 28;
	collisionHeight = 63;

	Animation off;
	for (int i = 0; i < 4; i++) {
		off.PushBack({ i * rect.w, 0, rect.w, rect.h });
	}
	off.speed = 10.0f;

	animations.Add(off);

	Animation on;
	for (int i = 0; i < 4; i++) {
		on.PushBack({ i * rect.w, 63, rect.w, rect.h });
	}
	on.speed = 10.0f;

	animations.Add(on);

	flip = SDL_FLIP_NONE;
	state = S_OFF;

	type = EntityModule::EntityType::ET_CHECKPOINT;

	entityBody = app->physics->CreateRectangleSensor(position.x + rect.w / 2, position.y + rect.h / 2, rect.w, rect.h, false);
	entityBody->bodyType = PhysBodyType::CHECKPOINT;
	entityBody->listener = module;
	entityBody->id = entityId;
	entityBody->remove = false;
}

void Checkpoint::Update(float dt)
{
	animations[state].Update(dt);
}

void Checkpoint::Render()
{
	SDL_Rect rect = animations[state].GetCurrentFrame();

	app->render->DrawTexture(texture, position.x, position.y, &rect, flip);
}

void Checkpoint::Check()
{
	state = S_ON;
	app->gameScene->SaveGameState();
}

void Checkpoint::Cleanup()
{
}
