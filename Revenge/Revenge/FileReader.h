#pragma once

#include <string>

enum class ANCHOR_POINT : unsigned char;
class NonPlayer;
enum class MAGIC_TYPE : unsigned int;
class Ability;
enum class MOVE_MODE;

class FileReader
{
public:

	std::vector<std::string> lines = std::vector<std::string>();

	virtual bool Open(const char* filepath);
	virtual void Close();

	std::vector<std::string> ParseLine(std::string line, char delim) const;
};

class SoundReader : public FileReader
{
	// layout:
};

class SaveReader : public FileReader
{

	// layout: 

	// 8 unsigned long long flags

public:

	// returns the 8 ull flags in an array
	std::vector<unsigned long long> GetFlags() const;
};

class SaveWriter
{

	// layout: 

	// 8 unsigned long long flags

public:

	bool WriteSave(const char* filepath);
};