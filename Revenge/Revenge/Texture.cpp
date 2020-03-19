#pragma once 
#include "Texture.h"
#include "Rectangle.h"
#include "defines.h"

Texture::Texture(int id, float _width, float _height)
{
	textureID = id; width = _width; height = _height; source = new MyRectangle(0, 0, _width, _height);
}

Texture::Texture(int id, float _width, float _height, float sourceX, float sourceY, float sourceWidth, float sourceHeight)
{
	textureID = id; height = _height; width = _width; source = new MyRectangle(sourceX, sourceY, sourceWidth, sourceHeight);
}

Texture::~Texture()
{
	SafeDelete(source);
}

Texture::Texture(Texture* that) : Texture(that->textureID, that->width, that->height, that->source->X(), that->source->Y(), that->source->Width(), that->source->Height())
{

}

Texture::Texture(Texture* that, MyRectangle source) : Texture(that->textureID, that->width, that->height, source.X(), source.Y(), source.Width(), source.Height())
{

}