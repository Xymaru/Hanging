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

	player_width = 38;
	player_height = 62;

	app->render->camera.x = 0;
	position.x = sprite_offset_xleft;
	position.y = 0;
	moveSpeed = 5;

	playerBody = app->physics->CreateRectangle(0, 0, player_width, player_height, false);

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
	player_state = IDLE;
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		player_flip = SDL_FLIP_HORIZONTAL;
		sprite_offset_x = sprite_offset_xright;
		position.x -= moveSpeed;
		player_state = WALK;

		if (position.x < sprite_offset_xleft) {
			position.x = sprite_offset_xleft;
		}

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

		player_state = WALK;

		if (position.x > app->map->mapData.map_width - sprite_offset_xleft - player_width) {
			position.x = app->map->mapData.map_width - sprite_offset_xleft - player_width;
		}

		int diff = position.x - app->render->camera.x;

		if (diff > cameraBound) {
			app->render->camera.x += moveSpeed;
			
			if (app->render->camera.x > app->map->mapData.map_width - app->win->GetWindowWidth()) {
				app->render->camera.x = app->map->mapData.map_width - app->win->GetWindowWidth();
			}
		}
	}
	
	animations[player_state].Update(dt);

	// Camera control
	
	/*int diff = position.x - app->render->camera.x;

	if (diff < cameraBound) {
		app->render->camera.x -= moveSpeed;
	}
	else if (diff > app->render->camera.w - cameraBound) {
		app->render->camera.x += moveSpeed;
	}*/

	return true;
}

// Called each loop iteration
bool PlayerModule::PostUpdate(float dt)
{
	bool ret = true;

	// Draw character
	SDL_Rect active_anim = animations[player_state].GetCurrentFrame();
	app->render->DrawTexture(playerTex, position.x - sprite_offset_x, position.y, &active_anim, player_flip);

	std::cout << position.x - sprite_offset_x << std::endl;

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

void PlayerModule::InitAnimations()
{
	int player_w = 89;
	int player_h = 76;

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
			animations[i].PushBack({ j * player_w, i * player_h,player_w,player_h });
			animations[i].speed = anim_speed;
		}
	}
}