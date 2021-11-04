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
	bool Awake();

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
private:
	enum PlayerState {
		DEAD, HURT, IDLE, JUMP, WALK, LAST
	};

	float anim_speed;

	int sprite_offset_x;

	int sprite_offset_xleft;
	int sprite_offset_xright;

	int player_width;
	int player_height;

	// Sprite width and height
	int player_sprite_w;
	int player_sprite_h;

	float player_jumpForce;

	PlayerState player_state;
	SDL_RendererFlip player_flip;

	Animation animations[PlayerState::LAST];

	SDL_Texture* playerTex;
	iPoint position;

	int moveSpeed;

	int cameraBound;

	PhysBody* playerBody;

	void InitAnimations();
};

#endif // __PLAYER_H__