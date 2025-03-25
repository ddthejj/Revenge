#include "defines.h"
#include "Component.h"
#include "ComponentManager.h"

Component::Component()
{
	Register();
}

Component::~Component()
{
	Unregister();
}

void Component::Register()
{
	ComponentManager::RegisterComponent(this);
}

void Component::Unregister()
{
	ComponentManager::UnregisterComponent(this);                                                                                                                                                                                                                                      Component();
}