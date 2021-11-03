#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "PlayerModule.h"

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
	app->render->camera.x = 0;
	position.x = cameraBound;
	position.y = 0;
	moveSpeed = 1;

	player_state = PlayerState::IDLE;
	player_flip = SDL_FLIP_NONE;
	sprite_offset = 0;
	anim_speed = 35.0f;
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

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
		player_state = WALK;
		player_flip = SDL_FLIP_HORIZONTAL;
		sprite_offset = SPRITE_OFFSET;
		position.x -= moveSpeed;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		player_flip = SDL_FLIP_NONE;
		sprite_offset = 0;
		position.x += moveSpeed;
		player_state = WALK;
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
	app->render->DrawTexture(playerTex, position.x + sprite_offset, position.y, &active_anim, player_flip);

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