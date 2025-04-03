#pragma once
#include "Component.h"

class InteractionComponent : public Component
{
#pragma region Methods

public:

	InteractionComponent(COMPONENT_TYPE _type, bool _isUnique);
	virtual void Interact() = 0;

#pragma endregion Methods
};

class DialogueComponent : public InteractionComponent
{
protected:

	std::vector<std::string> lines;

public:

	DialogueComponent(std::vector<std::string> _lines);
	virtual ~DialogueComponent() override;
	virtual void Interact();
};