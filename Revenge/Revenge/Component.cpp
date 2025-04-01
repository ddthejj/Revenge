#include "defines.h"
#include "Component.h"
#include "ComponentManager.h"

Component::Component(ComponentType _type, bool _isUnique)
{
	type = _type;
	isUnique = _isUnique;
	Register();
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