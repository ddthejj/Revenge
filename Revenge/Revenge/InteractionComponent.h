#pragma once
#include "Component.h"
#include <string>

class InteractionComponent : public Component
{
#pragma region Methods

	MyRectangle* rectangle;

public:

	InteractionComponent(Object* _owner, MyRectangle* _rectangle, COMPONENT_TYPE _type, bool _isUnique);
	bool TryInteract(Point<float> interactPoint);
	virtual void Interact() = 0;

#pragma endregion Methods
};

class DialogueComponent : public InteractionComponent
{
protected:

	std::vector<std::string> lines;

public:

	DialogueComponent(Object* _owner, MyRectangle* _rectangle, std::vector<std::string> _lines);
	virtual ~DialogueComponent() override;
	virtual void Interact();
};