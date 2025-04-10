#include "defines.h"
#include "Component.h"
#include "ComponentManager.h"
#include "Manager.h"

Component::Component(Object* _owner, COMPONENT_TYPE _type, bool _isUnique) : Object("Component")
{
	owner = _owner;
	type = _type;
	isUnique = _isUnique;
}

Component::~Component()
{
	Deactivate();
}

void Component::Activate()
{
	Object::Activate();
	Manager::AddUpdate(this);
	ComponentManager::RegisterComponent(this);
}

void Component::Deactivate()
{
	Object::Deactivate();
	Manager::RemoveUpdate(this);
	ComponentManager::UnregisterComponent(this);
}