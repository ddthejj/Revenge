#pragma once
#include "Character.h"

class Enemy : public NonPlayer
{
	Enemy(std::string _debugName, float x, float y, float height, float width, const Texture* _texture, float _layer, const char* filepath);
	~Enemy();

	// update the NPC
	void Update(float delta_time);
};

