#pragma once

#include "Entity.h"
#include "DynArray.h"

class EnemyBird : public Entity
{
private:
	enum EnemyState {
		ES_PATROL,
		ES_FOLLOW,
		ES_BACKING,
		ES_DYING,
		ES_DEAD
	};

	enum AnimState {
		AS_FLY,
		AS_HURT
	};

	float distanceFollow;

	int patrolDistance;
	bool patrolRight;

	int moveSpeed;

	AnimState animState;
	EnemyState state;

	SDL_RendererFlip flip;

	iPoint origin;

	DynArray<iPoint> path;
	float pathUpdateTimer;
	float pathUpdateTime;

	iPoint activeNode;

	int pathIndex;

	void Patrol(float dt);
	void Follow(float dt);
	void Backing(float dt);

	void MoveTo(iPoint destination, float dt);
	void CheckClosestIndex();
public:
	EnemyBird();
	~EnemyBird();

	void SetOrigin(iPoint pos) { origin = pos; }

	void Init(Module* module);
	void Update(float dt);
	void Render();

	void Cleanup();

	const DynArray<iPoint>* GetActivePath() const { return &path; }
};