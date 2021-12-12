#pragma once

#include "Entity.h"
#include "DynArray.h"

class EnemyBird : public Entity
{
public:
	enum BirdState {
		BS_PATROL,
		BS_FOLLOW,
		BS_BACKING,
		BS_DYING,
		BS_DEAD
	};
private:
	enum AnimState {
		AS_FLY,
		AS_HURT
	};

	float distanceFollow;

	int patrolDistance;
	bool patrolRight;

	int moveSpeed;

	AnimState animState;

	SDL_RendererFlip flip;

	iPoint originPos;

	DynArray<iPoint> path;
	float pathUpdateTimer;
	float pathUpdateTime;

	iPoint activeNode;

	int pathIndex;

	void Patrol(float dt);
	void Follow(float dt);
	void Backing(float dt);
	void Dying(float dt);

	void MoveTo(iPoint destination, float dt);
	void CheckClosestIndex();
public:
	EnemyBird();
	~EnemyBird();

	void SetOrigin(iPoint pos) { originPos = pos; }

	void Init(Module* module);
	void Update(float dt);
	void Render();

	void Cleanup();

	const DynArray<iPoint>* GetActivePath() const { return &path; }

	void SetState(int newState) override;
	void Die() override;
};