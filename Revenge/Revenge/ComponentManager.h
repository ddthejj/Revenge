#pragma once
#include <map>

class Component;
enum COMPONENT_TYPE : int;

class ComponentManager
{

#pragma region Properties

	static std::map<COMPONENT_TYPE, std::vector<Component*>> registeredComponents;

#pragma endregion

public:

#pragma region Methods

	static void Init();

	static void Clean();

	static void RegisterComponent(Component* componentToRegister);

	static void UnregisterComponent(Component* componentToUnregister);

	static std::vector<Component*> GetComponentsByType(COMPONENT_TYPE type);

#pragma endregion

};

