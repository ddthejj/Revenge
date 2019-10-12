#include "defines.h"
#include "Tile.h"
#include "SpriteBatch.h"
#include "Manager.h"
#include "Character.h"
#include "Texture.h"


#pragma region ProtoTile

ProtoTile::ProtoTile(ProtoTile* that) : ProtoTile(that->texture, that->height, that->width, that->collidable, that->door, that->interactable)
{

}


#pragma endregion

#pragma region Tile

Tile::Tile(ProtoTile* prototype, float _x, float _y, float _layer) : Sprite(_x, _y, prototype->height, prototype->width, prototype->texture, _layer)
{	
	collidable = prototype->collidable;
}

Tile::~Tile()
{

}


void Tile::Update()
{

}

#pragma endregion

#pragma region Door

Door::Door(ProtoTile* prototype, float _x, float _y, float _layer, int _destination, float _destX, float _destY) : Tile(prototype, _x, _y, _layer)
{
	destination = _destination;
	destX = _destX; destY = _destY;
}

Door::~Door()
{

}


void Door::Update()
{
	if (rectangle->Intersects(*(Manager::GetCurrentPlayer()->GetRectangle())))
	{
		Manager::HitDoor(this);
	}
}
#pragma endregion

#pragma region Interactable

Interactable::Interactable(ProtoTile* prototype, float _x, float _y, float _layer, std::string* _lines) : Tile(prototype, _x, _y, _layer)
{
	lines = _lines;
}

Interactable::~Interactable()
{
	delete[] lines;
}


void Interactable::Interact()
{

}

void Interactable::Update()
{

}

#pragma endregion