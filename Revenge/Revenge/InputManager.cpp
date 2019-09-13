#include "defines.h"
#include "InputManager.h"
#include <Windowsx.h>


bool InputManager::keys[KEY_MAX];
bool InputManager::previousKeys[KEY_MAX];
char InputManager::keyOptions[KEY_MAX];
bool InputManager::mouseKeys[MOUSE_KEY_MAX];
bool InputManager::previousMouseKeys[MOUSE_KEY_MAX];
float InputManager::mouseX = 0.f, InputManager::mouseY = 0.f;
char InputManager::charPressed = '\0', InputManager::previousCharPressed = '\0';

bool InputManager::IsKeyPressed(KEYS index)
{
	return (keys[index] == true && previousKeys[index] == false);
}

bool InputManager::IsMouseKeyPressed(MOUSE_KEYS index)
{
	return mouseKeys[index]== true && previousMouseKeys[index] == false;
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

void InputManager::MoveMouse(HWND hwnd, LPARAM lParam)
{
	mouseX = (float)(GET_X_LPARAM(lParam));
	mouseY = (float)(GET_Y_LPARAM(lParam));
}

void InputManager::PressMouseKey(MOUSE_KEYS key)
{
	mouseKeys[key] = true;
}

void InputManager::ReleaseMouseKey(MOUSE_KEYS key)
{
	mouseKeys[key] = false;
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

	for (int i = 0; i < MOUSE_KEY_MAX; i++)
		previousMouseKeys[i] = mouseKeys[i];

	previousCharPressed = charPressed;
}