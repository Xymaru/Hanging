#pragma once

#include "Entity.h"
#include "DynArray.h"

class Checkpoint : public Entity
{
private:
	enum AnimState {
		AS_OFF,
		AS_ON
	};

	AnimState animState;

	SDL_RendererFlip flip;

	iPoint origin;

public:
	Checkpoint();
	~Checkpoint();

	void SetOrigin(iPoint position) { origin = position; }

	void Init(Module* module);
	void Update(float dt);
	void Render();

	void Cleanup();
};