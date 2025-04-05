#include "defines.h"
#include "InteractionComponent.h"
#include "MenuManager.h"
#include "Rectangle.h"


InteractionComponent::InteractionComponent(Object* _owner, MyRectangle* _rectangle, COMPONENT_TYPE _type, bool _isUnique) : Component(_owner, _type, _isUnique)
{
	rectangle = _rectangle;
}

bool InteractionComponent::TryInteract(Point<float> interactPoint)
{
	if (rectangle->Contains(interactPoint))
	{
		Interact();
		return true;
	}
	return false;
}

DialogueComponent::DialogueComponent(Object* _owner, MyRectangle* _rectangle, std::vector<std::string> _lines) : InteractionComponent(_owner, _rectangle, COMPONENT_TYPE::DIALOGUE_INTERACTION, true)
{
	lines = _lines;
}

DialogueComponent::~DialogueComponent()
{
	lines.clear();
}

void DialogueComponent::Interact()
{
	MenuManager::StartDialogue(nullptr, lines);
}
