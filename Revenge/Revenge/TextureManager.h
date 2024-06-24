#pragma once
#include <map>
#include <string>

class Texture;
class SpriteBatch;

class TextureManager
{
	static Texture** textures;
	static std::map<std::string, int> texNames;

public:
	static bool LoadTextures(const wchar_t* filepath, SpriteBatch* spriteBatch);
	static bool UnloadTextures();
	static const Texture* GetTexture(unsigned int index);
	static const Texture* GetTexture(const char* name);
	static void Clean();
};

