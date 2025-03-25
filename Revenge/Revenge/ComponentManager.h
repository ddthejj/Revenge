#pragma once


class Component;
enum ComponentType : int;

class ComponentManager
{

#pragma region Properties

	static std::vector<Component*> registeredComponents[];

#pragma endregion

public:

#pragma region Methods

	static void Init();

	static void Clean();

	static void RegisterComponent(Component* componentToRegister);

	static void UnregisterComponent(Component* componentToUnregister);

	static std::vector<Component*> GetComponentsByType(ComponentType type);

#pragma endregion

};

