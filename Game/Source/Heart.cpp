#include "Heart.h"
#include "Render.h"
#include "Textures.h"
#include "PlayerModule.h"
#include "Map.h"


Heart::Heart()
{
}

Heart::~Heart()
{
}

void Heart::Init(Module* module)
{
	texture = app->tex->Load("Assets/Textures/heart/heart.png");

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

	type = EntityModule::EntityType::ET_HEART;

	entityBody = app->physics->CreateRectangleSensor(position.x + rect.w / 2, position.y + rect.h / 2, rect.w, rect.h, false);
	entityBody->bodyType = PhysBodyType::HEART;
	entityBody->listener = module;
	entityBody->id = entityId;
	entityBody->remove = false;
}

void Heart::Update(float dt)
{
	animations[animState].Update(dt);
}

void Heart::Render()
{
	SDL_Rect rect = animations[animState].GetCurrentFrame();

	app->render->DrawTexture(texture, position.x, position.y, &rect, flip);
}


