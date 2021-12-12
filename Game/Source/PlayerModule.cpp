#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Fonts.h"
#include "PlayerModule.h"
#include "EntityModule.h"
#include "Map.h"
#include "ModuleFadeToBlack.h"
#include "EndScene.h"
#include "GameScene.h"

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
bool PlayerModule::Awake(pugi::xml_node& config)
{
	LOG("Loading PlayerModule");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool PlayerModule::Start()
{
	playerTex = app->tex->Load("Assets/Textures/player.png");
	jump = app->audio->LoadFx("Assets/Audio/Fx/jump.wav");

	char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	healthFont = app->fonts->Load("Assets/Fonts/rtype_font3.png", lookupTable, 2);

	playerState = PlayerState::IDLE;
	playerFlip = SDL_FLIP_NONE;
	spriteOffsetXLeft = 33;
	spriteOffsetXRight = 18;
	spriteOffsetX = spriteOffsetXLeft;
	anim_speed = 35.0f;
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	playerSpriteWidth = 89;
	playerSpriteHeight = 76;

	playerJumpForce = 450.0f;
	player_deadForce = 150.0f;

	playerWidth = 38;
	playerHeight = 62;

	app->render->camera.x = 0;
	position.x = 0;
	position.y = 0;
	moveSpeed = 3;

	playerGodmode = false;

	cameraBound = app->win->GetWindowWidth() / 2 - playerWidth / 2;

	InitAnimations();
	return true;
}

void PlayerModule::Activate()
{
	Module::Activate();

	playerBody = app->physics->CreateRectangle(position.x + playerWidth / 2, position.y + playerHeight / 2 + (playerSpriteHeight - playerHeight) / 2, playerWidth, playerHeight, true);
	playerBody->body->SetFixedRotation(true);
	playerBody->body->SetSleepingAllowed(false);
	playerBody->bodyType = PhysBodyType::PLAYER;
	playerBody->listener = this;
	playerBody->remove = false;

	playerState = IDLE;
	playerFlip = SDL_FLIP_NONE;
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
	position.x = METERS_TO_PIXELS(box_pos.x) - playerWidth / 2;
	position.y = METERS_TO_PIXELS(box_pos.y) - (playerHeight / 2 + (playerSpriteHeight - playerHeight) / 2);

	if (playerState != HURT) {
		PlayerControl();
	}
	else {
		if (position.y >= app->win->getWindowHeight()) {
			if (playerhealth <= 0) {
				app->fade->FadeToBlack(app->gameScene, app->endScene);
			}
			else if (app->enemies->checkpoint_active == true) {
				app->gameScene->LoadGameState();
			}
			else
			{
				app->fade->FadeToBlack(app->gameScene, app->gameScene);
			}
		}
	}
	
	animations[playerState].Update(dt);

	return true;
}

// Called each loop iteration
bool PlayerModule::PostUpdate(float dt)
{
	bool ret = true;

	// Draw character
	SDL_Rect active_anim = animations[playerState].GetCurrentFrame();
	app->render->DrawTexture(playerTex, position.x - spriteOffsetX, position.y, &active_anim, playerFlip);

	app->fonts->BlitText(10 + app->render->camera.x, 10, healthFont, "player life ");
	sprintf_s(livesText, 10, "%3d", playerhealth);
	app->fonts->BlitText(105 + app->render->camera.x, 10, healthFont, livesText);

	app->fonts->BlitText(10 + app->render->camera.x, 20, healthFont, "player score ");
	sprintf_s(livesText, 10, "%5d", playerscore);
	app->fonts->BlitText(105 + app->render->camera.x, 20, healthFont, livesText);

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

	playerBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x + playerWidth / 2), PIXEL_TO_METERS(position.y + playerHeight / 2 + (playerSpriteHeight - playerHeight) / 2)), 0);
}

void PlayerModule::SetState(int state)
{
	playerState = (PlayerState)state;
}

void PlayerModule::OnCollision(PhysBody * bodyA, PhysBody * bodyB)
{
	if (playerGodmode) return;

	if (playerState == HURT) {
		return;
	}

	if (bodyB->bodyType == PhysBodyType::GROUND || bodyB->bodyType == PhysBodyType::PLATFORM) {
		if (playerState == JUMP) {
			// Check if it collided from above
			float32 diffY = bodyB->body->GetPosition().y - bodyA->body->GetPosition().y;

			if (diffY >= 0.8) {
				playerState = IDLE;
				animations[JUMP].Reset();
			}
		}
	}

	if (bodyB->bodyType == PhysBodyType::END) {
		if(app->gameScene->gameLevel == GameScene::GameLevel::Level1){
			app->gameScene->gameLevel = GameScene::GameLevel::Level2;
			app->fade->FadeToBlack(app->gameScene, app->gameScene);

			app->playerModule->playerscore += 500;
		}
		else {
			app->endScene->win = true;
			app->fade->FadeToBlack(app->gameScene, app->endScene);
		}
	}

	if (bodyB->bodyType == PhysBodyType::SPIKES || bodyB->bodyType == PhysBodyType::CHICKEN || bodyB->bodyType == PhysBodyType::BIRD) {
		playerhealth--;
		playerState = HURT;

		playerBody->body->SetLinearVelocity(b2Vec2_zero);

		if (playerFlip == SDL_FLIP_NONE) {
			playerBody->body->ApplyForceToCenter(b2Vec2(-player_deadForce, -player_deadForce), true);
		}
		else {
			playerBody->body->ApplyForceToCenter(b2Vec2(player_deadForce, -player_deadForce), true);
		}

		playerBody->body->GetFixtureList()->SetSensor(true);
	}
}

void PlayerModule::SavePlayer(pugi::xml_node & save)
{
	save.attribute("x") = position.x;
	save.attribute("y") = position.y;
	save.attribute("score") = playerscore;
	save.attribute("health") = playerhealth;
	save.attribute("state") = playerState;
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
			animations[i].PushBack({ j * playerSpriteWidth, i * playerSpriteHeight,playerSpriteWidth,playerSpriteHeight });
			animations[i].speed = anim_speed;
		}
	}

	animations[JUMP].loop = false;
	animations[HURT].loop = false;
}

void PlayerModule::PlayerControl()
{
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		playerGodmode = !playerGodmode;
		playerBody->body->SetActive(!playerGodmode);
	}

	b2Vec2 box_pos = playerBody->body->GetPosition();

	if (playerState == WALK) {
		playerState = IDLE;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		playerFlip = SDL_FLIP_HORIZONTAL;
		spriteOffsetX = spriteOffsetXRight;
		position.x -= moveSpeed;
		if (playerState == IDLE) {
			playerState = WALK;
		}

		if (position.x < 0) {
			position.x = 0;
		}

		box_pos.x = PIXEL_TO_METERS(position.x + playerWidth / 2);

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
		playerFlip = SDL_FLIP_NONE;
		spriteOffsetX = spriteOffsetXLeft;
		position.x += moveSpeed;

		if (playerState == IDLE) {
			playerState = WALK;
		}

		if (position.x > app->map->mapData.map_width - playerWidth) {
			position.x = app->map->mapData.map_width - playerWidth;
		}

		box_pos.x = PIXEL_TO_METERS(position.x + playerWidth / 2);

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

	if (playerGodmode) {
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			position.y -= moveSpeed;

			box_pos.y = PIXEL_TO_METERS(position.y + playerHeight / 2 + (playerSpriteHeight - playerHeight) / 2);

			playerBody->body->SetTransform(box_pos, 0);
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			position.y += moveSpeed;

			box_pos.y = PIXEL_TO_METERS(position.y + playerHeight / 2 + (playerSpriteHeight - playerHeight) / 2);

			playerBody->body->SetTransform(box_pos, 0);
		}
	}
	else {
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			if (playerState != JUMP && playerState != DEAD) {
				playerState = JUMP;
				playerBody->body->ApplyForceToCenter(b2Vec2(0, -playerJumpForce), true);
				if (fx_on)
				{
					app->audio->PlayFx(jump);
					fx_on = false;
				}
			}
		}
	}
}
