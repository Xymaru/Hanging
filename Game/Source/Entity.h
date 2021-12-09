#pragma once

#include "Point.h"
#include "Animation.h"
#include "ModulePhysics.h"
#include "EntityModule.h"

#include "SDL/include/SDL_render.h"

#include "Defs.h"

class Entity
{
protected:
	EntityModule::EntityType type;

	iPoint position;
	SDL_Texture* texture;
	
	List<Animation> animations;
	SDL_Rect rect;

	int collisionWidth, collisionHeight;

	PhysBody* pbody;
public:
	Entity();
	~Entity();

	virtual void Init(){}
	virtual void Update(float dt){}
	virtual void Render(){}

	void SetPosition(iPoint pos) { position = pos; }

	EntityModule::EntityType GetType() { return type; }

	void Cleanup();
};