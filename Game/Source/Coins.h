#pragma once

#include "Entity.h"
#include "DynArray.h"

class Coins : public Entity
{
private:
	enum AnimState {
		AS_FLIP,
	};

	AnimState animState;

	SDL_RendererFlip flip;

	iPoint origin;
public:
	Coins();
	~Coins();

	void SetOrigin(iPoint position) { origin = position; }

	void Init(Module* module);
	void Update(float dt);
	void Render();
};