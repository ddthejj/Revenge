#pragma once
#include "Object.h"

enum COMPONENT_TYPE : int
{
	INVALID = 0,
	INTERACTION = 1 << 0,
	TRIGGER = 1 << 1,
	COLLISION = 1 << 2,
	DIALOGUE_INTERACTION = (1 << 3) | INTERACTION,
	DOOR_TRIGGER = (1 << 4) | TRIGGER,
	STATIC_COLLISION = (1 << 5) | COLLISION,
	DYNAMIC_COLLISION = (1 << 6) | COLLISION,
	MOVEMENT = (1 << 7),
	ANIMATION = (1 << 8),
};

class Component : public Object
{

#pragma region Properties
private:
	
	COMPONENT_TYPE type = COMPONENT_TYPE::INVALID;
	bool isUnique = false;

protected:

	Object* owner;

#pragma endregion Properties

#pragma region Methods

protected:

	Component(Object* _owner, COMPONENT_TYPE _type, bool _isUnique = false);

public:

	virtual ~Component();

	virtual void Activate() override;
	virtual void Deactivate() override;

	virtual void Update(float delta_time) override {}
	virtual void Draw(SpriteBatch* spriteBatch) override {}

	COMPONENT_TYPE GetType() { return type; }
	bool IsUnique() { return isUnique; }

#pragma endregion Methods
};

