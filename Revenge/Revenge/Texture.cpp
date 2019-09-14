#pragma once 
#include "Texture.h"
#include "Rectangle.h"
#include "defines.h"

Texture::Texture(int id, float _height, float _width, float sourceX, float sourceY, float sourceWidth, float sourceHeight)
{
	textureID = id; height = _height; width = _width; source = new MyRectangle(sourceX, sourceY, sourceWidth, sourceHeight);
}

Texture::~Texture()
{
	SafeDelete(source);
}