#include "Coins.h"
#include "Render.h"
#include "Textures.h"
#include "PlayerModule.h"
#include "Pathfinding.h"
#include "Map.h"


Coins::Coins()
{
}

Coins::~Coins()
{
}

void Coins::Init()
{
	texture = app->tex->Load("Assets/Textures/Coins/coins.png");

	rect = { 0, 0, 32, 32 };

	collisionWidth = 32;
	collisionHeight = 32;

	Animation stay;
	for (int i = 0; i < 8; i++) {
		stay.PushBack({ i * rect.w, 0, rect.w, rect.h });
	}
	stay.speed = 10.0f;

	animations.Add(stay);

	flip = SDL_FLIP_NONE;
	animState = AS_FLIP;
	
	type = EntityModule::EntityType::ET_COIN;
}

void Coins::Update(float dt)
{
	animations[animState].Update(dt);
}

void Coins::Render()
{
	SDL_Rect rect = animations[animState].GetCurrentFrame();

	app->render->DrawTexture(texture, position.x, position.y, &rect, flip);
}

void Coins::Cleanup()
{
}