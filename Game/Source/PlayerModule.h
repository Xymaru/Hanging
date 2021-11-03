#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"

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

private:
	enum PlayerState {
		DEAD, HURT, IDLE, JUMP, WALK, LAST
	};

	float anim_speed;
	
	const int SPRITE_OFFSET = 32;
	int sprite_offset;
	PlayerState player_state;
	SDL_RendererFlip player_flip;

	Animation animations[PlayerState::LAST];

	SDL_Texture* playerTex;
	iPoint position;

	int moveSpeed;

	int cameraBound = 150;

	void InitAnimations();
};

#endif // __PLAYER_H__