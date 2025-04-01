#include "defines.h"
#include "Tile.h"
#include "SpriteBatch.h"
#include "Manager.h"
#include "OverworldManager.h"
#include "Character.h"
#include "Texture.h"
#include "MenuManager.h"
#include "Rectangle.h"
#include "Component.h"
#include "TriggerComponent.h"


#pragma region ProtoTile

ProtoTile::ProtoTile(ProtoTile* that) : ProtoTile(that->texture, that->height, that->width, that->collidable, that->door, that->dialogueinteractable)
{

}


#pragma endregion

#pragma region Tile

Tile::Tile(std::string _debugName, const ProtoTile* prototype, float _x, float _y, float _layer) : Sprite(_debugName, _x, _y, prototype->height, prototype->width, prototype->texture, _layer)
{	
	collidable = prototype->collidable;
}

Tile::~Tile()
{
	if (active) Deactivate();
}


void Tile::Update(float delta_time)
{

}

bool Tile::AddComponent(Component* _component)
{
	bool retVal = Object::AddComponent(_component);

	if (retVal)
	{
		switch (_component->GetType())
		{
		case ComponentType::DoorTrigger:
		{
			BindToPlayerCollided(std::bind(&DoorComponent::Trigger, (DoorComponent*)_component));
			break;
		}
		}
	}

	return retVal;
}

#pragma endregion
