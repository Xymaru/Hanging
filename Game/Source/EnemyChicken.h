#pragma once

#include "Entity.h"
#include "DynArray.h"

class EnemyChicken : public Entity
{
public:
	enum ChickenState {
		CS_PATROL,
		CS_FOLLOW,
		CS_HURT,
		CS_DEAD
	};
private:
	enum AnimState {
		AS_IDLE,
		AS_WALK,
		AS_HURT
	};

	float distanceAware;
	float distanceFollow;

	int patrolDistance;
	bool patrolRight;

	int moveSpeed;

	AnimState animState;

	SDL_RendererFlip flip;

	iPoint origin;

	void Patrol(float dt);
	void Follow(float dt);
	void Hurt(float dt);
public:
	EnemyChicken();
	~EnemyChicken();

	void SetOrigin(iPoint pos) { origin = pos; }

	void Init(Module* module);
	void Update(float dt);
	void Render();

	void Die();
	void SetState(int newState) override;
};