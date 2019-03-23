#include "defines.h"
#include "InputManager.h"


bool InputManager::keys[Manager::KEY_MAX];
bool InputManager::previousKeys[Manager::KEY_MAX];
char InputManager::keyOptions[Manager::KEY_MAX];

bool InputManager::IsKeyPressed(Manager::KEYS index)
{
	if (keys[index] == true && previousKeys[index] == false)
		return true;

	return false;
}


void InputManager::PressKey(WPARAM wParam)
{
	for (int key = 0; key < Manager::KEY_MAX; key++)
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
	for (int key = 0; key < Manager::KEY_MAX; key++)
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
	keyOptions[Manager::KEY_UP] = 'W';
	keyOptions[Manager::KEY_DOWN] = 'S';
	keyOptions[Manager::KEY_LEFT] = 'A';
	keyOptions[Manager::KEY_RIGHT] = 'D';
	keyOptions[Manager::KEY_MENU] = 'E';
	keyOptions[Manager::KEY_INTERACT] = VK_SPACE;
}

void InputManager::Update(float delta_time)
{

	for (int i = 0; i < Manager::KEY_MAX; i++)
		previousKeys[i] = keys[i];
}