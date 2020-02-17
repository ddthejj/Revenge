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
	static Texture* GetTexture(unsigned int index);
	static Texture* GetTexture(const char* name);
	static void Clean();
};

