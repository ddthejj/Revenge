#include "defines.h"
#include "ComponentManager.h"
#include "Component.h"

std::map<COMPONENT_TYPE, std::vector<Component*>> ComponentManager::registeredComponents = std::map<COMPONENT_TYPE, std::vector<Component*>>();


void ComponentManager::Init()
{
	registeredComponents.insert(std::pair(COMPONENT_TYPE::INTERACTION, std::vector<Component*>()));
	registeredComponents.insert(std::pair(COMPONENT_TYPE::TRIGGER, std::vector<Component*>()));
	registeredComponents.insert(std::pair(COMPONENT_TYPE::DIALOGUE_INTERACTION, std::vector<Component*>()));
	registeredComponents.insert(std::pair(COMPONENT_TYPE::DOOR_TRIGGER, std::vector<Component*>()));
	// we don't differentiate between collision types here, the physics manager handles that.
	registeredComponents.insert(std::pair(COMPONENT_TYPE::COLLISION, std::vector<Component*>())); 
	registeredComponents.insert(std::pair(COMPONENT_TYPE::COLLISION, std::vector<Component*>())); 
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

std::vector<Component*> ComponentManager::GetComponentsByType(COMPONENT_TYPE type)
{
	return registeredComponents[type];
}