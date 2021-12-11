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

	PhysBody* entityBody;

	int entityId;

	bool remove;
public:
	Entity();
	~Entity();

	virtual void Init(Module* module){}
	virtual void Update(float dt){}
	virtual void Render(){}

	void SetPosition(iPoint pos) { position = pos; }
	void SetId(int id) { entityId = id; }

	int GetId() { return entityId; }

	EntityModule::EntityType GetType() { return type; }

	virtual void Cleanup();
	virtual void Die(){}

	bool Remove() { return remove; }
};