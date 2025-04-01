#include "defines.h"
#include "ComponentManager.h"
#include "Component.h"

std::map<ComponentType, std::vector<Component*>> ComponentManager::registeredComponents = std::map<ComponentType, std::vector<Component*>>();


void ComponentManager::Init()
{
	registeredComponents.insert(std::pair(ComponentType::Interaction, std::vector<Component*>()));
	registeredComponents.insert(std::pair(ComponentType::Trigger, std::vector<Component*>()));
	registeredComponents.insert(std::pair(ComponentType::DialogueInteraction, std::vector<Component*>()));
	registeredComponents.insert(std::pair(ComponentType::DoorTrigger, std::vector<Component*>()));
}

void ComponentManager::Clean()
{
	for (auto it = registeredComponents.begin(); it != registeredComponents.end(); it++)
	{
		(*it).second.clear();
	}
}

void ComponentManager::RegisterComponent(Component* componentToRegister)
{
	for (auto it = registeredComponents.begin(); it != registeredComponents.end(); it++)
	{
		if (it->first & componentToRegister->GetType())
		{
			it->second.push_back(componentToRegister);
		}
	}
}

void ComponentManager::UnregisterComponent(Component* componentToUnregister)
{
	for (auto it = registeredComponents.begin(); it != registeredComponents.end(); it++)
	{
		for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{
			if (*it2 == componentToUnregister)
			{
				it->second.erase(it2);
				break;
			}
		}
	}
}

std::vector<Component*> ComponentManager::GetComponentsByType(ComponentType type)
{
	return registeredComponents[type];
}