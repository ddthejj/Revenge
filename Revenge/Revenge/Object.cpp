#include "defines.h"
#include "Object.h"
#include "Component.h"

Object::Object(std::string _debugName)
{
	debugName = _debugName;
}

Object::~Object() 
{
	if (active) Deactivate();

	if (components.size() > 0)
	{
		for (auto it = components.begin(); it != components.end(); it++)
		{
			delete (*it);
		}
	}
	components.clear();
}

void Object::Activate()
{
	active = true; frozen = false; BindCallbacks();
}

void Object::Deactivate()
{
	active = false; frozen = false; UnbindCallbacks();
}

Component* Object::GetComponentOfType(ComponentType type)
{
	for (auto it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->GetType() & type)
		{
			return *it;
		}
	}

	return nullptr;
}

bool Object::AddComponent(Component* _component)
{
	if (_component->IsUnique())
	{
		if (GetComponentOfType(_component->GetType()))
		{
			return false;
		}
	}

	components.push_back(_component);
	return true;
}
