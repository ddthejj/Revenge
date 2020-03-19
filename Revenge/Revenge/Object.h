#pragma once
#include "Rectangle.h"
#include "defines.h"

class SpriteBatch;

// the base class for all gameobjects
class Object
{
protected:

#pragma region Properties

	bool frozen = false;	// whether or not the object is frozen
	bool active = false;	// whether or not the object is active

#pragma endregion

public:

#pragma region Methods

	Object() {}
	~Object() { if (active) Deactivate(); }

	// activate the object
	virtual void Activate() { active = true; frozen = false; }
	// deactivate the object
	virtual void Deactivate() { active = false; frozen = false; }
	// freeze the object
	virtual void Freeze() { frozen = true; }
	// unfreeze the object
	virtual void Unfreeze() { frozen = false; }

	// returns whether or not the object is active 
	bool IsActive() { return active; }
	// returns whether or not the object is frozen
	bool IsFrozen() { return frozen; }

#pragma endregion
};

