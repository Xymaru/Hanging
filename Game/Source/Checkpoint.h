#pragma once

#include "Entity.h"
#include "DynArray.h"

class Checkpoint : public Entity
{
public:
	enum CheckpointState {
		S_OFF,
		S_ON
	};
private:
	SDL_RendererFlip flip;

	iPoint origin;

public:
	Checkpoint();
	~Checkpoint();

	void SetOrigin(iPoint position) { origin = position; }

	void Init(Module* module);
	void Update(float dt);
	void Render();

	void Check();
	void Cleanup();
};