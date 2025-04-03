#include "defines.h"
#include "Component.h"
#include "ComponentManager.h"

Component::Component(COMPONENT_TYPE _type, bool _isUnique)
{
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