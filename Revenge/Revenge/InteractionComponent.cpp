#include "defines.h"
#include "InteractionComponent.h"

void InteractionComponent::Register()
{
}

void InteractionComponent::Unregister()
{
}

void InteractionComponent::OnInteract()
{
	if (interactFunc)
	{
		interactFunc();
	}
}
