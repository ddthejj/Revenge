#include "defines.h"
#include "Manager.h"
#include "TextureManager.h"
#include "Texture.h"
#include "SpriteBatch.h"
#include "Rectangle.h"

#include <string>
#include <fstream>
#include <sstream>

Texture** TextureManager::textures;
std::map<std::string, int> TextureManager::texNames;

bool TextureManager::LoadTextures(const wchar_t* filepath, SpriteBatch* spriteBatch)
{
	std::ifstream texfile(filepath, std::ios::in);

	if (!texfile.is_open())
		return false;

	std::vector<std::string> words;

	std::string line;
	int whichChar = 0;
	int texAt = 0;

	// get the number of textures
	std::getline(texfile, line);
	int texCount = atoi(line.c_str());
	textures = new Texture * [texCount];
	for (int i = 0; i < texCount; i++)
		textures[i] = nullptr;

	// get each line
	while (!texfile.eof())
	{
		std::getline(texfile, line);

		std::stringstream ss(line);
		std::string word;
		while (std::getline(ss, word, ','))
		{
			words.push_back(word);
		}

		if (words.size() == 4)
		{
			size_t size = words[1].length() + 1;
			wchar_t* wword = new wchar_t[size];
			size_t outsize;
			mbstowcs_s(&outsize, wword, size, words[1].c_str(), size - 1);
			textures[texAt] = spriteBatch->Load(wword, (float)atof(words[2].c_str()), (float)atof(words[3].c_str()));
			texNames[words[0]] = texAt;
			delete[] wword;
			words.clear();
		}
		else if (words.size() == 6)
		{
			textures[texAt] = (new Texture((textures[atoi(words[1].c_str())]),
				MyRectangle((float)atof(words[2].c_str()), (float)atof(words[3].c_str()), (float)atof(words[4].c_str()), (float)atof(words[5].c_str()))));
			texNames[words[0]] = texAt;
			words.clear();
		}
		texAt++;
	}
	texfile.close();
	return true;
}

Texture* TextureManager::GetTexture(unsigned int index)
{
	if (texNames.size() > index)
		return (textures[index]);
	else
		return nullptr;
}

Texture* TextureManager::GetTexture(const char* name)
{
	if (texNames.find(name) != texNames.end())
		return textures[texNames.at(name)];
	else
		return nullptr;
}

void TextureManager::Clean()
{
	for (int i = 0; i < texNames.size(); i++)
	{
		delete textures[i];
	}
	delete[] textures;

	texNames.clear();
}
