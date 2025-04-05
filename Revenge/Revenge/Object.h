#pragma once
#include <string>
#include <vector>

class SpriteBatch;
class Component;
enum COMPONENT_TYPE : int;

enum class ANCHOR_POINT : unsigned char
{
	TOP = (0) << 2,
	VCENTER = (1) << 2,
	BOTTOM = (1 << 1) << 2,
	LEFT = 0,
	HCENTER = 1,
	RIGHT = 1 << 1,

	ANCHOR_TOP_LEFT = TOP | LEFT,
	ANCHOR_TOP_CENTER = TOP | HCENTER,
	ANCHOR_TOP_RIGHT = TOP | RIGHT,
	ANCHOR_CENTER_LEFT = VCENTER | LEFT,
	ANCHOR_CENTER = VCENTER | HCENTER,
	ANCHOR_CENTER_RIGHT = VCENTER | RIGHT,
	ANCHOR_BOTTOM_LEFT = BOTTOM | LEFT,
	ANCHOR_BOTTOM_CENTER = BOTTOM | HCENTER,
	ANCHOR_BOTTOM_RIGHT = BOTTOM | RIGHT
};

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

