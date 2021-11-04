#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "PlayerModule.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>

PlayerModule::PlayerModule() : Module()
{
	name.Create("PlayerModule");
}

// Destructor
PlayerModule::~PlayerModule()
{}

// Called before render is available
bool PlayerModule::Awake()
{
	LOG("Loading PlayerModule");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool PlayerModule::Start()
{
	playerTex = app->tex->Load("Assets/Textures/player.png");

	player_state = PlayerState::IDLE;
	player_flip = SDL_FLIP_NONE;
	sprite_offset_xleft = 33;
	sprite_offset_xright = 18;
	sprite_offset_x = sprite_offset_xleft;
	anim_speed = 35.0f;
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	player_sprite_w = 89;
	player_sprite_h = 76;

	player_jumpForce = 400.0f;

	player_width = 38;
	player_height = 62;

	app->render->camera.x = 0;
	position.x = sprite_offset_xleft;
	position.y = 0;
	moveSpeed = 5;

	playerBody = app->physics->CreateRectangle(position.x + player_width/2, position.y + player_height/2 + (player_sprite_h - player_height) / 2, player_width, player_height, true);
	playerBody->body->SetFixedRotation(true);
	playerBody->bodyType = PhysBodyType::PLAYER;
	playerBody->listener = this;

	cameraBound = app->win->GetWindowWidth() / 2 - player_width / 2;

	InitAnimations();
	return true;
}

// Called each loop iteration
bool PlayerModule::PreUpdate(float dt)
{
	return true;
}

// Called each loop iteration
bool PlayerModule::Update(float dt)
{
	b2Vec2 box_pos = playerBody->body->GetPosition();
	position.x = METERS_TO_PIXELS(box_pos.x) - player_width/2;
	position.y = METERS_TO_PIXELS(box_pos.y) - (player_height / 2 + (player_sprite_h - player_height) / 2);

	if (player_state == WALK) {
		player_state = IDLE;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		player_flip = SDL_FLIP_HORIZONTAL;
		sprite_offset_x = sprite_offset_xright;
		position.x -= moveSpeed;
		if (player_state == IDLE) {
			player_state = WALK;
		}

		if (position.x < sprite_offset_xleft) {
			position.x = sprite_offset_xleft;
		}

		box_pos.x = PIXEL_TO_METERS(position.x + player_width / 2);

		playerBody->body->SetTransform(box_pos, 0);

		// Camera control
		int diff = position.x - app->render->camera.x;

		if (diff < cameraBound) {
			app->render->camera.x -= moveSpeed;

			if (app->render->camera.x < 0) {
				app->render->camera.x = 0;
			}
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		player_flip = SDL_FLIP_NONE;
		sprite_offset_x = sprite_offset_xleft;
		position.x += moveSpeed;

		if (player_state == IDLE) {
			player_state = WALK;
		}

		if (position.x > app->map->mapData.map_width - sprite_offset_xleft - player_width) {
			position.x = app->map->mapData.map_width - sprite_offset_xleft - player_width;
		}

		box_pos.x = PIXEL_TO_METERS(position.x + player_width / 2);

		playerBody->body->SetTransform(box_pos, 0);

		// Camera control
		int diff = position.x - app->render->camera.x;

		if (diff > cameraBound) {
			app->render->camera.x += moveSpeed;
			
			if (app->render->camera.x > app->map->mapData.map_width - app->win->GetWindowWidth()) {
				app->render->camera.x = app->map->mapData.map_width - app->win->GetWindowWidth();
			}
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		if (player_state != JUMP) {
			player_state = JUMP;
			playerBody->body->ApplyForceToCenter(b2Vec2(0, -player_jumpForce), true);
		}
	}
	
	animations[player_state].Update(dt);

	return true;
}

// Called each loop iteration
bool PlayerModule::PostUpdate(float dt)
{
	bool ret = true;

	// Draw character
	SDL_Rect active_anim = animations[player_state].GetCurrentFrame();
	app->render->DrawTexture(playerTex, position.x - sprite_offset_x, position.y, &active_anim, player_flip);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool PlayerModule::CleanUp()
{
	LOG("Freeing playerModule");
	return true;
}

void PlayerModule::SetPosition(int x, int y)
{
	position.x = x;
	position.y = y;

	playerBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x + player_width / 2), PIXEL_TO_METERS(position.y + player_height / 2 + (player_sprite_h - player_height) / 2)), 0);
}

void PlayerModule::OnCollision(PhysBody * bodyA, PhysBody * bodyB)
{
	if (bodyB->bodyType == PhysBodyType::GROUND) {
		if (player_state == JUMP) {
			// Check if it collided from above
			float32 diffY = bodyB->body->GetPosition().y - bodyA->body->GetPosition().y;

			std::cout << diffY << std::endl;

			if (diffY >= 1) {
				player_state = IDLE;
				animations[JUMP].Reset();
			}
		}
	}
}

void PlayerModule::InitAnimations()
{
	// SPRITE COUNT PER ANIMATION
	int spriteCount[]{
		11,
		8,
		10,
		12,
		8
	};

	// LOAD ALL ANIMATIONS
	for (int i = 0; i < LAST; i++) {
		for (int j = 0; j < spriteCount[i]; j++) {
			animations[i].PushBack({ j * player_sprite_w, i * player_sprite_h,player_sprite_w,player_sprite_h });
			animations[i].speed = anim_speed;
		}
	}

	animations[JUMP].loop = false;
}