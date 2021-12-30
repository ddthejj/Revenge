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
	virtual void Activate() { active = true; frozen = false; BindCallbacks(); }
	// deactivate the object
	virtual void Deactivate() { active = false; frozen = false; UnbindCallbacks(); }
	// freeze the object
	virtual void Freeze() { frozen = true; UnbindCallbacks(); }
	// unfreeze the object
	virtual void Unfreeze() { frozen = false; BindCallbacks(); }

	virtual void Draw(SpriteBatch* spriteBatch) = 0;
	virtual void Update(float delta_time) = 0;

	// returns whether or not the object is active 
	bool IsActive() { return active; }
	// returns whether or not the object is frozen
	bool IsFrozen() { return frozen; }


	virtual void BindCallbacks() {}
	virtual void UnbindCallbacks() {}

#pragma endregion
};

