#pragma once

#include "Entity.h"
#include "DynArray.h"

class EnemyChicken : public Entity
{
public:
	enum class EnemyState {
		ES_PATROL,
		ES_FOLLOW,
		ES_HURT,
		ES_DEAD
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
	EnemyState state;

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
	EnemyState GetState() { return state; }
};