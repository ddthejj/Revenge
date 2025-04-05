#include "defines.h"
#include "Component.h"
#include "ComponentManager.h"
#include "Object.h"

Component::Component(Object* _owner, COMPONENT_TYPE _type, bool _isUnique)
{
	owner = _owner;
	type = _type;
	isUnique = _isUnique;
}

Component::~Component()
{
	Unregister();
}

void Component::Register()
{
	ComponentManager::RegisterComponent(this);
}

void Component::Unregister()
{
	ComponentManager::UnregisterComponent(this);
}