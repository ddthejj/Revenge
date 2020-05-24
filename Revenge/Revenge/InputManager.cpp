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

std::vector<InputManager::Callback<void (*)(char, void*)>> InputManager::anyKeyPressedCallback = std::vector<InputManager::Callback<void (*)(char, void*)>>();
std::vector<InputManager::Callback<void (*)(char, void*)>> InputManager::anyKeyReleasedCallback = std::vector<InputManager::Callback<void (*)(char, void*)>>();
std::vector<InputManager::Callback<void (*)(void*)>> InputManager::keyPressedCallbacks[(int)KEYS::KEY_MAX];
std::vector<InputManager::Callback<void (*)(void*)>> InputManager::keyReleasedCallbacks[(int)KEYS::KEY_MAX];
std::vector<InputManager::Callback<void (*)(void*)>> InputManager::mouseKeyPressedCallbacks[(int)MOUSE_KEYS::MOUSE_KEY_MAX];
std::vector<InputManager::Callback<void (*)(void*)>> InputManager::mouseKeyReleasedCallbacks[(int)MOUSE_KEYS::MOUSE_KEY_MAX];

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
			for(int j = 0; j < keyPressedCallbacks[key].size(); j++)
			{
				keyPressedCallbacks[key][j].func(keyPressedCallbacks[key][j].object);
			}
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
			for (int j = 0; j < keyReleasedCallbacks[key].size(); j++)
			{
				keyReleasedCallbacks[key][j].func(keyReleasedCallbacks[key][j].object);
			}
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

	for (int j = 0; j < mouseKeyPressedCallbacks[(int)key].size(); j++)
	{
		mouseKeyPressedCallbacks[(int)key][j].func(mouseKeyPressedCallbacks[(int)key][j].object);
	}
}

void InputManager::ReleaseMouseKey(MOUSE_KEYS key)
{
	mouseKeys[(int)key] = false;

	for (int j = 0; j < mouseKeyReleasedCallbacks[(int)key].size(); j++)
	{
		mouseKeyReleasedCallbacks[(int)key][j].func(mouseKeyReleasedCallbacks[(int)key][j].object);
	}
}

void InputManager::PressChar(WPARAM wParam)
{
	charPressed = (char)wParam; 

	for (int j = 0; j < anyKeyPressedCallback.size(); j++)
	{
		anyKeyPressedCallback[j].func((char)wParam, anyKeyPressedCallback[j].object);
	}

}


bool InputManager::AnyKeyPressedCallback_Attatch(void(*func)(char, void*), void* this_ptr)
{
	return AddFunctionToCallbackList(&anyKeyPressedCallback, func, this_ptr);
}

bool InputManager::AnyKeyPressedCallback_Remove(void(*func)(char, void*), void* this_ptr)
{
	return RemoveFunctionFromCallbackList(&anyKeyPressedCallback, func, this_ptr);
}

bool InputManager::KeyPressedCallbacks_Attatch(KEYS whichKey, void(*func)(void*), void* this_ptr)
{
	return AddFunctionToCallbackList(&keyPressedCallbacks[(int)whichKey], func, this_ptr);
}

bool InputManager::KeyPressedCallbacks_Remove(KEYS whichKey, void(*func)(void*), void* this_ptr)
{
	return RemoveFunctionFromCallbackList(&keyPressedCallbacks[(int)whichKey], func, this_ptr);
}

bool InputManager::MouseKeyPressedCallbacks_Attatch(MOUSE_KEYS whichKey, void(*func)(void*), void* this_ptr)
{
	return AddFunctionToCallbackList(&mouseKeyPressedCallbacks[(int)whichKey], func, this_ptr);
}

bool InputManager::MouseKeyPressedCallbacks_Remove(MOUSE_KEYS whichKey, void(*func)(void*), void* this_ptr)
{
	return RemoveFunctionFromCallbackList(&mouseKeyPressedCallbacks[(int)whichKey], func, this_ptr);
}

bool InputManager::AnyKeyReleasedCallback_Attatch(void(*func)(char, void*), void* this_ptr)
{
	return AddFunctionToCallbackList(&anyKeyReleasedCallback, func, this_ptr);
}

bool InputManager::AnyKeyReleasedCallback_Remove(void(*func)(char, void*), void* this_ptr)
{
	return RemoveFunctionFromCallbackList(&anyKeyReleasedCallback, func, this_ptr);
}

bool InputManager::KeyReleasedCallbacks_Attatch(KEYS whichKey, void(*func)(void*), void* this_ptr)
{
	return AddFunctionToCallbackList(&keyReleasedCallbacks[(int)whichKey], func, this_ptr);
}

bool InputManager::KeyReleasedCallbacks_Remove(KEYS whichKey, void(*func)(void*), void* this_ptr)
{
	return RemoveFunctionFromCallbackList(&keyReleasedCallbacks[(int)whichKey], func, this_ptr);
}

bool InputManager::MouseKeyReleasedCallbacks_Attatch(MOUSE_KEYS whichKey, void(*func)(void*), void* this_ptr)
{
	return AddFunctionToCallbackList(&mouseKeyReleasedCallbacks[(int)whichKey], func, this_ptr);
}

bool InputManager::MouseKeyReleasedCallbacks_Remove(MOUSE_KEYS whichKey, void(*func)(void*), void* this_ptr)
{
	return RemoveFunctionFromCallbackList(&mouseKeyReleasedCallbacks[(int)whichKey], func, this_ptr);
}


void InputManager::Init()
{
	// initial keybinds (replace with reading from settings file later)
	keyOptions[(int)KEYS::KEY_UP] = 'W';
	keyOptions[(int)KEYS::KEY_DOWN] = 'S';
	keyOptions[(int)KEYS::KEY_LEFT] = 'A';
	keyOptions[(int)KEYS::KEY_RIGHT] = 'D';
	keyOptions[(int)KEYS::KEY_MENU] = 'E';
	keyOptions[(int)KEYS::KEY_INTERACT] = VK_SPACE;


	// initialize callback arrays
	for (int i = 0; i < (int)KEYS::KEY_MAX; i++)
	{
		keyPressedCallbacks[i] = std::vector<Callback<void (*)(void*)>>();
		keyReleasedCallbacks[i] = std::vector<Callback<void (*)(void*)>>();
	}
	for (int i = 0; i < (int)MOUSE_KEYS::MOUSE_KEY_MAX; i++)
	{
		mouseKeyPressedCallbacks[i] = std::vector<Callback<void (*)(void*)>>();
		mouseKeyReleasedCallbacks[i] = std::vector<Callback<void (*)(void*)>>();
	}
}

void InputManager::Clean()
{
	// clear callback arrays
	for (int i = 0; i < (int)KEYS::KEY_MAX; i++)
	{
		keyPressedCallbacks[i].clear();
		keyReleasedCallbacks[i].clear();
	}
	for (int i = 0; i < (int)MOUSE_KEYS::MOUSE_KEY_MAX; i++)
	{
		mouseKeyPressedCallbacks[i].clear();
		mouseKeyReleasedCallbacks[i].clear();
	}
}

void InputManager::Update(float delta_time)
{
	for (int i = 0; i < (int)KEYS::KEY_MAX; i++)
		previousKeys[i] = keys[i];

	for (int i = 0; i < (int)MOUSE_KEYS::MOUSE_KEY_MAX; i++)
		previousMouseKeys[i] = mouseKeys[i];

	previousCharPressed = charPressed;
}


template <class T>
bool InputManager::AddFunctionToCallbackList(std::vector<Callback<T>>* callbackList, T func, void* this_ptr)
{

	// check if we already contain the function
	for (auto it = callbackList->begin(); it != callbackList->end(); it++)
	{
		if ((*it).func == func && (*it).object == this_ptr)
		{
			// this list already contains the callback
			return false;
		}
	}
	// match wasn't found

	// create the callback struct
	Callback<T> newCallback;
	newCallback.func = func;
	newCallback.object = this_ptr;
	// add the new callback to the list
	callbackList->push_back(newCallback);
	return true;
}

template<class T>
bool InputManager::RemoveFunctionFromCallbackList(std::vector<Callback<T>>* callbackList, T func, void* this_ptr)
{
	// make sure we already contain the function
	for (auto it = callbackList->begin(); it != callbackList->end(); it++)
	{
		if ((*it).func == func && (*it).object == this_ptr)
		{
			callbackList->erase(it);
			return true;
		}
	}
	return false;
}