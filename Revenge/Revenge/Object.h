#pragma once
#include "Rectangle.h"
#include "defines.h"

class SpriteBatch;
class Component;

// the base class for all gameobjects
class Object
{
protected:

#pragma region Properties

	// whether or not the object is frozen
	bool frozen = false;	
	// whether or not the object is active
	bool active = false;	
	// debug name for help identifying the object while debugging
	std::string debugName; 
	// list of components attached to this object. This can be things like interaction components.
	std::vector<Component*> components;

#pragma endregion

public:

#pragma region Methods

	Object(std::string _debugName) { debugName = _debugName; }
	~Object() { if (active) Deactivate(); }

	// activate the object
	virtual void Activate() { active = true; frozen = false; BindCallbacks(); }
	// deactivate the object
	virtual void Deactivate() { active = false; frozen = false; UnbindCallbacks(); }
	// freeze the object
	virtual void Freeze() { frozen = true; }
	// unfreeze the object
	virtual void Unfreeze() { frozen = false; }

	virtual void Draw(SpriteBatch* spriteBatch) = 0;
	virtual void Update(float delta_time) = 0;

	// returns whether or not the object is active 
	bool IsActive() const { return active; }
	// returns whether or not the object is frozen
	bool IsFrozen() const { return frozen; }


	virtual void BindCallbacks() {}
	virtual void UnbindCallbacks() {}

#pragma endregion
};

