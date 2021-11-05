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

	// Called when player collides with an object
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void ReStart();
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

	int cameraBound;

	PhysBody* playerBody;

	float player_deadForce;

	bool playerGodmode;

	void InitAnimations();
	void PlayerControl();
};

#endif // __PLAYER_H__