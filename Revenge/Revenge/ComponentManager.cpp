#include "defines.h"
#include "ComponentManager.h"
#include "Component.h"

std::vector<Component*> ComponentManager::registeredComponents[ComponentType::Max];


void ComponentManager::Init()
{
	for (int i = 0; i < ComponentType::Max; i++)
	{
		registeredComponents[i] = std::vector<Component*>();
	}
}

void ComponentManager::Clean()
{
	for (int i = 0; i < ComponentType::Max; i++)
	{
		registeredComponents[i].clear();
	}
}

void ComponentManager::RegisterComponent(Component* componentToRegister)
{
	registeredComponents[componentToRegister->type].push_back(componentToRegister);
}

void ComponentManager::UnregisterComponent(Component* componentToUnregister)
{
	std::vector<Component*> componentList = registeredComponents[componentToUnregister->type];

	for (auto it = componentList.begin(); it != componentList.end(); it++)
	{
		if (*it == componentToUnregister)
		{
			registeredComponents[componentToUnregister->type].erase(it);
		}
	}
}

std::vector<Component*> ComponentManager::GetComponentsByType(ComponentType type)
{
	return registeredComponents[type];
}