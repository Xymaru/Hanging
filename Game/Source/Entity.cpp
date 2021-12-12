#include "Entity.h"

Entity::Entity()
{
	entityBody = NULL;
	remove = false;
	state = -1;
}

Entity::~Entity()
{
}

void Entity::Cleanup()
{
	if (entityBody) {
		entityBody->remove = true;
	}
}