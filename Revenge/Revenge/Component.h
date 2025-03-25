#pragma once

enum ComponentType : int
{
	Invalid = -1,
	None,
	Interaction,
	Max
};

class Component
{
public:
#pragma region Properties

	ComponentType type = None;

#pragma endregion Properties

#pragma region Methods

	Component();
	~Component();

	virtual void Register();
	virtual void Unregister();

#pragma endregion Methods
};

