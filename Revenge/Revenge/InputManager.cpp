#include "defines.h"
#include "InputManager.h"
#include <Windowsx.h>


bool InputManager::keys[(int)KEYS::KEY_MAX];
bool InputManager::previousKeys[(int)KEYS::KEY_MAX];
char InputManager::keyOptions[(int)KEYS::KEY_MAX];
bool InputManager::mouseKeys[(int)MOUSE_KEYS::MOUSE_KEY_MAX];
bool InputManager::previousMouseKeys[(int)MOUSE_KEYS::MOUSE_KEY_MAX];
float InputManager::mouseX = 0.f, InputManager::mouseY = 0.f;
char InputManager::charPressed = '\0', InputManager::previousCharPressed = '\0';

bool InputManager::IsKeyPressed(KEYS index)
{
	return (keys[(int)index] == true && previousKeys[(int)index] == false);
}

bool InputManager::IsMouseKeyPressed(MOUSE_KEYS index)
{
	return mouseKeys[(int)index]== true && previousMouseKeys[(int)index] == false;
}


void InputManager::PressKey(WPARAM wParam)
{
	for (int key = 0; key < (int)KEYS::KEY_MAX; key++)
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
	for (int key = 0; key < (int)KEYS::KEY_MAX; key++)
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
	mouseKeys[(int)key] = true;
}

void InputManager::ReleaseMouseKey(MOUSE_KEYS key)
{
	mouseKeys[(int)key] = false;
}


void InputManager::Init()
{
	// key options
	keyOptions[(int)KEYS::KEY_UP] = 'W';
	keyOptions[(int)KEYS::KEY_DOWN] = 'S';
	keyOptions[(int)KEYS::KEY_LEFT] = 'A';
	keyOptions[(int)KEYS::KEY_RIGHT] = 'D';
	keyOptions[(int)KEYS::KEY_MENU] = 'E';
	keyOptions[(int)KEYS::KEY_INTERACT] = VK_SPACE;
}

void InputManager::Update(float delta_time)
{
	for (int i = 0; i < (int)KEYS::KEY_MAX; i++)
		previousKeys[i] = keys[i];

	for (int i = 0; i < (int)MOUSE_KEYS::MOUSE_KEY_MAX; i++)
		previousMouseKeys[i] = mouseKeys[i];

	previousCharPressed = charPressed;
}