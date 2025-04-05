#pragma once
#include "Component.h"

class CollisionComponent;

class TriggerComponent : public Component
{
	MyRectangle* rectangle;

public:

	TriggerComponent(Object* _owner, COMPONENT_TYPE _type, bool _isUnique, MyRectangle* _rectangle);

	MyRectangle* GetRectangle() { return rectangle; }

	virtual void Trigger(CollisionComponent* component) = 0;
};

class DoorComponent : public TriggerComponent
{
private:

	int destination = 0;
	int x = 0;
	int y = 0;

public:

	DoorComponent(Object* _owner, MyRectangle* _rectangle, int _destination, int _x, int _y);
	virtual void Trigger(CollisionComponent* component);
};