#include "defines.h"

std::map<std::string, TILES>				EnumParser::tilesMap =
{
	{ "TILE_BROWNFLOOR",	TILES::TILE_BROWNFLOOR},
	{ "TILE_REDWALL",		TILES::TILE_REDWALL},
	{ "TILE_GREENDOOR",		TILES::TILE_GREENDOOR},
	{ "TILE_BLUETEXT",		TILES::TILE_BLUETEXT},
	{ "TILE_MAX",			TILES::TILE_MAX}
};
std::map<std::string, TEXTURES_TEST>		EnumParser::testTexturesMap =
{
	{ "TEX_T_BLACK",		TEXTURES_TEST::TEX_T_BLACK},
	{ "TEX_T_MENU",			TEXTURES_TEST::TEX_T_MENU},
	{ "TEX_T_ARROW",		TEXTURES_TEST::TEX_T_ARROW},
	{ "TEX_T_TESTROOM",		TEXTURES_TEST::TEX_T_TESTROOM},
	{ "TEX_T_BROWNFLOOR",	TEXTURES_TEST::TEX_T_BROWNFLOOR},
	{ "TEX_T_REDWALL",		TEXTURES_TEST::TEX_T_REDWALL},
	{ "TEX_T_GREENDOOR",	TEXTURES_TEST::TEX_T_GREENDOOR},
	{ "TEX_T_BLUETEXT",		TEXTURES_TEST::TEX_T_BLUETEXT},
	{ "TEX_T_PLAYER",		TEXTURES_TEST::TEX_T_PLAYER},
	{ "TEX_T_MAX",			TEXTURES_TEST::TEX_T_MAX},
};
std::map<std::string, TEXTURES_TITLE>		EnumParser::titleTexturesMap =
{
	{ "TEX_TITLE_BLACK",		TEXTURES_TITLE::TEX_TITLE_BLACK},
	{ "TEX_TITLE_LOGO",			TEXTURES_TITLE::TEX_TITLE_LOGO},
	{ "TEX_TITLE_TITLE",		TEXTURES_TITLE::TEX_TITLE_TITLE},
	{ "TEX_TITLE_BACKGROUND",	TEXTURES_TITLE::TEX_TITLE_BACKGROUND},
	{ "TEX_TITLE_MENU",			TEXTURES_TITLE::TEX_TITLE_MENU},
	{ "TEX_TITLE_ARROW",		TEXTURES_TITLE::TEX_TITLE_ARROW},
};
std::map<std::string, KEYS>					EnumParser::keysMap =
{
	{ "KEY_UP",			KEYS::KEY_UP},
	{ "KEY_DOWN",		KEYS::KEY_DOWN},
	{ "KEY_LEFT",		KEYS::KEY_LEFT},
	{ "KEY_RIGHT",		KEYS::KEY_RIGHT},
	{ "KEY_MENU",		KEYS::KEY_MENU},
	{ "KEY_INTERACT",	KEYS::KEY_INTERACT},
	{ "KEY_MAX",		KEYS::KEY_MAX}
};
std::map<std::string, MOUSE_KEYS>			EnumParser::mouseKeysMap =
{
	{ "MOUSE_KEY_LEFT",		MOUSE_KEYS::MOUSE_KEY_LEFT},
	{ "MOUSE_KEY_RIGHT",	MOUSE_KEYS::MOUSE_KEY_RIGHT},
	{ "MOUSE_KEY_MIDDLE",	MOUSE_KEYS::MOUSE_KEY_MIDDLE},
	{ "MOUSE_KEY_MAX",		MOUSE_KEYS::MOUSE_KEY_MAX},
};
std::map<std::string, FADE_STATUS>			EnumParser::fadeStatusMap =
{
	{ "FADE_START",		FADE_STATUS::FADE_START },
	{ "FADE_OUT",		FADE_STATUS::FADE_OUT},
	{ "FADE_SWITCH",	FADE_STATUS::FADE_SWITCH},
	{ "FADE_IN",		FADE_STATUS::FADE_IN},
	{ "FADE_DONE",		FADE_STATUS::FADE_DONE},
	{ "FADE_MAX",		FADE_STATUS::FADE_MAX},
};
std::map<std::string, GAME_STATE>			EnumParser::gameStateMap =
{

	{ "STATE_TITLE",		GAME_STATE::STATE_TITLE },
	{ "STATE_LOADING",		GAME_STATE::STATE_LOADING},
	{ "STATE_OVERWORLD",	GAME_STATE::STATE_OVERWORLD},
	{ "STATE_BATTLE",		GAME_STATE::STATE_BATTLE},
	{ "STATE_CUTSCENE",		GAME_STATE::STATE_CUTSCENE},
	{ "STATE_DIALOGUE",		GAME_STATE::STATE_DIALOGUE},
	{ "STATE_MAX",			GAME_STATE::STATE_MAX},
};
std::map<std::string, ANCHOR_POINT>			EnumParser::anchorPointMap =
{
	{ "ANCHOR_TOP_LEFT",		ANCHOR_POINT::ANCHOR_TOP_LEFT },
	{ "ANCHOR_TOP_CENTER",		ANCHOR_POINT::ANCHOR_TOP_CENTER},
	{ "ANCHOR_TOP_RIGHT",		ANCHOR_POINT::ANCHOR_TOP_RIGHT},
	{ "ANCHOR_CENTER_LEFT",		ANCHOR_POINT::ANCHOR_CENTER_LEFT},
	{ "ANCHOR_CENTER",			ANCHOR_POINT::ANCHOR_CENTER},
	{ "ANCHOR_CENTER_RIGHT",	ANCHOR_POINT::ANCHOR_CENTER_RIGHT},
	{ "ANCHOR_BOTTOM_LEFT",		ANCHOR_POINT::ANCHOR_BOTTOM_LEFT},
	{ "ANCHOR_BOTTOM_CENTER",	ANCHOR_POINT::ANCHOR_BOTTOM_CENTER},
	{ "ANCHOR_BOTTOM_RIGHT",	ANCHOR_POINT::ANCHOR_BOTTOM_RIGHT},
};
std::map<std::string, SOUND_CHANNEL>		EnumParser::soundChannelMap =
{
	{ "MASTER",					SOUND_CHANNEL::MASTER },
	{ "MUSIC",					SOUND_CHANNEL::MUSIC},
	{ "SFX",					SOUND_CHANNEL::SFX},
	{ "SOUND_CHANNEL_MAX",		SOUND_CHANNEL::SOUND_CHANNEL_MAX},
};

TILES EnumParser::ParseTiles(const std::string& name)
{
	return tilesMap[name];
}

TEXTURES_TEST EnumParser::ParseTestTexture(const std::string& name)
{
	return testTexturesMap[name];
}

TEXTURES_TITLE EnumParser::ParseTitleTexture(const std::string& name)
{
	return titleTexturesMap[name];
}

KEYS EnumParser::ParseKeys(const std::string& name)
{
	return keysMap[name];
}

MOUSE_KEYS EnumParser::ParseMouseKeys(const std::string& name)
{
	return mouseKeysMap[name];
}

FADE_STATUS EnumParser::ParseFadeStatus(const std::string& name)
{
	return fadeStatusMap[name];
}

GAME_STATE EnumParser::ParseGameState(const std::string& name)
{
	return gameStateMap[name];
}

ANCHOR_POINT EnumParser::ParseAnchorPoint(const std::string& name)
{
	return anchorPointMap[name];
}

SOUND_CHANNEL EnumParser::ParseSoundChannel(const std::string& name)
{
	return soundChannelMap[name];
}
