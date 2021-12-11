#include "Entity.h"

Entity::Entity()
{
	entityBody = NULL;
	remove = false;
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