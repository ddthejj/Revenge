#pragma once
#include "Component.h"

class InteractionComponent : public Component
{
public:

#pragma region Properties

	void (*interactFunc) () = nullptr;

#pragma endregion

#pragma region Methods

	virtual void Register() override;
	virtual void Unregister() override;

	void OnInteract();

#pragma endregion Methods
};

