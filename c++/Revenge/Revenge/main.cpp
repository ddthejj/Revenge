#include "SpriteBatch.h"
#include "Texture.h"
#include "Game.h"

#define WINDOWWIDTH 800
#define WINDOWHEIGHT 600

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY)
	{
		PostQuitMessage(0); return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmd, int cmdShow)
{
	SpriteBatch* graphics;

#pragma region WindowHandling


	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.hInstance = hInstance;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.lpszClassName = "MainWindow";
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowClass);
	
	RECT rect = { 0, 0, WINDOWWIDTH, WINDOWHEIGHT };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

	HWND windowHandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "MainWindow", "DirectX Test", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		500, 200, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, 0);

	if (!windowHandle) return -1;

	graphics = new SpriteBatch();
	if (!graphics->Init(windowHandle))
	{
		delete graphics; return -1;
	}
	ShowWindow(windowHandle, cmdShow);

#pragma endregion

	MSG message;
	message.message = WM_NULL;

	Texture::SetSpriteBatch(graphics);

	Game game = Game(graphics);

	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, windowHandle, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&message);
		} 
		else
		{
			game.Update();

			game.Draw();
		}
	}


	delete graphics;

	return 0;
}

