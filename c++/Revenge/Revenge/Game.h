#pragma once

class SpriteBatch;

class Game
{
	SpriteBatch* spriteBatch;

public:
	Game(SpriteBatch* gfx);
	~Game();
	void Update();
	void Draw();
};

