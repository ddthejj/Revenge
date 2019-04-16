#include "defines.h"
#include "InputManager.h"


bool InputManager::keys[KEY_MAX];
bool InputManager::previousKeys[KEY_MAX];
char InputManager::keyOptions[KEY_MAX];

bool InputManager::IsKeyPressed(KEYS index)
{
	if (keys[index] == true && previousKeys[index] == false)
		return true;

	return false;
}


void InputManager::PressKey(WPARAM wParam)
{
	for (int key = 0; key < KEY_MAX; key++)
	{
		if (wParam == keyOptions[key])
		{
			keys[key] = true;
			break;
		}
	}
}

void InputManager::ReleaseKey(WPARAM wParam)
{
	for (int key = 0; key < KEY_MAX; key++)
	{
		if (wParam == keyOptions[key])
		{
			keys[key] = false;
			break;
		}
	}
}


void InputManager::Init()
{
	// key options
	keyOptions[KEY_UP] = 'W';
	keyOptions[KEY_DOWN] = 'S';
	keyOptions[KEY_LEFT] = 'A';
	keyOptions[KEY_RIGHT] = 'D';
	keyOptions[KEY_MENU] = 'E';
	keyOptions[KEY_INTERACT] = VK_SPACE;
}

void InputManager::Update(float delta_time)
{

	for (int i = 0; i < KEY_MAX; i++)
		previousKeys[i] = keys[i];
}