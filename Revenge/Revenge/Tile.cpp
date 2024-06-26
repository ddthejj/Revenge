#include "defines.h"
#include "Tile.h"
#include "SpriteBatch.h"
#include "Manager.h"
#include "OverworldManager.h"
#include "Character.h"
#include "Texture.h"
#include "MenuManager.h"


#pragma region ProtoTile

ProtoTile::ProtoTile(ProtoTile* that) : ProtoTile(that->texture, that->height, that->width, that->collidable, that->door, that->interactable)
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

#pragma endregion

#pragma region Door

Door::Door(std::string _debugName, const ProtoTile* prototype, float _x, float _y, float _layer, int _destination, float _destX, float _destY) : Tile(_debugName, prototype, _x, _y, _layer)
{
	destination = _destination;
	destX = _destX; destY = _destY;
}

Door::~Door()
{
	if (active) Deactivate();
}


void Door::Update(float delta_time)
{
	if (rectangle->Intersects(*(OverworldManager::GetCurrentPlayer()->GetRectangle())))
	{
		Manager::HitDoor(this);
	}
}

#pragma endregion

#pragma region Interactable

Interactable::Interactable(std::string _debugName, const ProtoTile* prototype, float _x, float _y, float _layer, std::vector<std::string> _lines) : Tile(_debugName, prototype, _x, _y, _layer)
{
	lines = _lines;
}

Interactable::~Interactable()
{
	if (active) Deactivate();
}


void Interactable::Interact() const
{
	MenuManager::StartDialogue(nullptr, lines);
}

void Interactable::Update(float delta_time)
{

}

#pragma endregion