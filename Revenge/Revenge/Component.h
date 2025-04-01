#pragma once

enum ComponentType : int
{
	Invalid = -1,
	None,
	Interaction = 1 << 0, 
	Trigger = 1 << 1,
	DialogueInteraction = (1 << 3) | Interaction,
	DoorTrigger = (1 << 4) | Trigger
};

class Component
{
private:
#pragma region Properties

	ComponentType type = None;
	bool isUnique = false;

#pragma endregion Properties

#pragma region Methods

protected:

	Component(ComponentType _type, bool _isUnique = false);

public:

	virtual ~Component();

	virtual void Register();
	virtual void Unregister();

	ComponentType GetType() { return type; }
	bool IsUnique() { return isUnique; }

#pragma endregion Methods
};

