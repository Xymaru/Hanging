#pragma once

#include "Entity.h"
#include "DynArray.h"

class Heart : public Entity
{
private:
	enum AnimState {
		AS_FLIP,
	};

	AnimState animState;

	SDL_RendererFlip flip;

public:
	Heart();
	~Heart();

	void Init(Module* module);
	void Update(float dt);
	void Render();
};

