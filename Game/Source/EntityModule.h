#pragma once

#include "Module.h"
#include "List.h"
#include "DynArray.h"
#include "Point.h"

class Entity;

class EntityModule : public Module
{
public:
	enum EntityType {
		ET_BIRD,
		ET_CHICKEN,
		ET_COIN,
		ET_HEART,
		ET_LAST
	};

	EntityModule();

	// Destructor
	virtual ~EntityModule();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt) override;

	// called to deactivate module
	void Deactivate();

	// Called before quitting
	bool CleanUp();

	void SaveEntities(pugi::xml_node& entities);
	void LoadEntities(pugi::xml_node& entities);

	void AddEntity(EntityType type, iPoint position);
private:
	List<Entity*> entities;

	void DrawPath(const DynArray<iPoint>* path);
};