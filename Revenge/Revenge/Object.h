#pragma once
#include "Rectangle.h"
#include "defines.h"

class SpriteBatch;

class Object
{
protected:
	bool frozen = false;
	bool active = false;
public:
	Object() {}
	~Object() {}

	virtual void Activate() { active = true; frozen = false; }
	virtual void Deactivate() { active = false; frozen = false; }
	virtual void Freeze() { frozen = true; }
	virtual void Unfreeze() { frozen = false; }

	bool IsActive() { return active; }
	bool IsFrozen() { return frozen; }
};

