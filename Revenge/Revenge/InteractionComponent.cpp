#include "defines.h"
#include "InteractionComponent.h"
#include "MenuManager.h"


InteractionComponent::InteractionComponent(ComponentType _type, bool _isUnique) : Component(_type, _isUnique)
{

}

DialogueComponent::DialogueComponent(std::vector<std::string> _lines) : InteractionComponent(ComponentType::DialogueInteraction, true)
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
