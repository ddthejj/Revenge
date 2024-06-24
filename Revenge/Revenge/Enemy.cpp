#include "defines.h"
#include "Enemy.h"

Enemy::Enemy(std::string _debugName, float x, float y, float height, float width, const Texture* _texture, float _layer, const char* filepath) : NonPlayer(_debugName, x, y, height, width, _texture, _layer, filepath)
{

}

Enemy::~Enemy()
{

}

void Enemy::Update(float delta_time)
{

}
