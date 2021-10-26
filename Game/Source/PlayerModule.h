#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"

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
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* playerTex;
	iPoint position;
	int offsetX;

	int moveSpeed;

	int cameraBound = 150;
};

#endif // __PLAYER_H__