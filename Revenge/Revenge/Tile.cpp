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
#include "CollisionComponent.h"


#pragma region ProtoTile

ProtoTile::ProtoTile(ProtoTile* that) : ProtoTile(that->texture, that->height, that->width, that->collidable, that->door, that->dialogueinteractable)
{

}


#pragma endregion

#pragma region Tile

Tile::Tile(std::string _debugName, const ProtoTile* prototype, float _x, float _y, float _layer) : Sprite(_debugName, _x, _y, prototype->height, prototype->width, prototype->texture, _layer)
{	
	if (prototype->collidable)
	{
		AddComponent(new StaticCollisionComponent(this, rectangle));
	}
}

Tile::~Tile()
{
	if (active) Deactivate();
}


void Tile::Update(float delta_time)
{

}

#pragma endregion
