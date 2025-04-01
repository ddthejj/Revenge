#pragma once
#include "Component.h"

class TriggerComponent : public Component
{
public:

	TriggerComponent(ComponentType _type, bool _isUnique);
	virtual void Trigger() = 0;
};

class DoorComponent : public TriggerComponent
{
private:

	int destination = 0;
	int x = 0;
	int y = 0;

public:

	DoorComponent(int _destination, int _x, int _y);
	virtual void Trigger();
};