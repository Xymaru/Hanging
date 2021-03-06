#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "ModulePhysics.h"

struct SDL_Texture;

class PlayerModule : public Module
{
public:

	PlayerModule();

	// Destructor
	virtual ~PlayerModule();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	void Activate() override;

	// Called before all Updates
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// Set position of the player
	void SetPosition(int x, int y);

	iPoint GetPosition() { return position; }

	// Set player state
	void SetState(int state);

	// Called when player collides with an object
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void SavePlayer(pugi::xml_node& node);

	void Render();

	int playerhealth;
	int playerscore;

	float playingTime;
private:
	enum PlayerState {
		DEAD, HURT, IDLE, JUMP, WALK, LAST
	};

	float anim_speed;

	int spriteOffsetX;

	int spriteOffsetXLeft;
	int spriteOffsetXRight;

	int playerWidth;
	int playerHeight;

	uint jump;
	uint lose;
	uint win;

	bool fx_on;
	// Sprite width and height
	int playerSpriteWidth;
	int playerSpriteHeight;

	float playerJumpForce;

	PlayerState playerState;
	SDL_RendererFlip playerFlip;

	Animation animations[PlayerState::LAST];

	SDL_Texture* playerTex;
	iPoint position;

	int moveSpeed;

	int healthFont = -1;
	char livesText[10] = { "\0" };

	int cameraBound;

	PhysBody* playerBody;

	float player_deadForce;

	bool playerGodmode;

	void InitAnimations();
	void PlayerControl();
};

#endif // __PLAYER_H__