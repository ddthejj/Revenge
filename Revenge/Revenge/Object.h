#pragma once
#include <string>
#include <vector>

class SpriteBatch;
class Component;
enum COMPONENT_TYPE : int;

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

	Object(std::string _debugName);
	~Object();

	// activate the object
	virtual void Activate();
	// deactivate the object
	virtual void Deactivate();
	// freeze the object
	virtual void Freeze();
	// unfreeze the object
	virtual void Unfreeze();

	virtual void Draw(SpriteBatch* spriteBatch) = 0;
	virtual void Update(float delta_time) = 0;

	// returns whether or not the object is active 
	bool IsActive() const { return active; }
	// returns whether or not the object is frozen
	bool IsFrozen() const { return frozen; }

	virtual void BindCallbacks() {}
	virtual void UnbindCallbacks() {}

	Component* GetComponentOfType(COMPONENT_TYPE type);
	virtual bool AddComponent(Component* _component);

#pragma endregion
};

