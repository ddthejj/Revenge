#pragma once
#include "defines.h"
#include "vld.h"
#include "Manager.h"
#include <time.h>


int CALLBACK WinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPSTR lpCmdLine,_In_ int nShowCmd);
LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);


int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// Target framerate
	const float FPS = 60.f;

#ifdef _DEBUG
	// Sets debug flag to have Windows dump memory leaks (not 100% reliable, trust Visual Leak Detector more with this)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// Breaks on allocation of certain memory number. Useful when dealing with hard to find memory leaks
	_CrtSetBreakAlloc(-1);
#endif

	// Create the window
	HWND hWnd;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass1";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, (LONG)WIDTH, (LONG)HEIGHT };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(
		NULL,
		L"WindowClass1",
		L"Revenge",
		WS_OVERLAPPEDWINDOW,
		300, 150, wr.left + wr.right, wr.top + wr.bottom,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nShowCmd);

	MSG msg;

	Manager::Init(hWnd);

	clock_t lastUpdate = clock();

	//Game Loop
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			clock_t ts = clock() - lastUpdate;
			if (((float)ts / CLOCKS_PER_SEC) >= (1.f / FPS))
			{
				lastUpdate = clock();

				Manager::Update((float)ts);
				Manager::Draw();
			}
		}
	}
	Manager::Clean();
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (message)
	{

#pragma region Window Handling

		//Window is activated or deactivated
	case WM_ACTIVATE:
		switch (wParam)
		{
			// Window is activated
		case WA_ACTIVE:
			break;
			// Window is deactivated
		case WA_INACTIVE:
			break;
		}
		break;

		// Window is destroyed
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		// Window is resized
	case WM_SIZE:
		Manager::ResizeWindow(hWnd);
		break;

	//	// Window is moved
	//case WM_MOVE:
	//	Manager::MoveWindow(hWnd);
	//	break;

#pragma endregion

#pragma region Mouse Handling

		//Mouse movement is detected
	case WM_MOUSEMOVE:
		if (hWnd == GetForegroundWindow())
			Manager::MoveMouse(hWnd, lParam);
		break;

		// Left mouse button clicked
	case WM_LBUTTONDOWN:
		if (hWnd == GetForegroundWindow())
			Manager::PressMouseKey(MOUSE_KEYS::MOUSE_KEY_LEFT);
		break;

		// Left mouse button released
	case WM_LBUTTONUP:
		if (hWnd == GetForegroundWindow())
			Manager::ReleaseMouseKey(MOUSE_KEYS::MOUSE_KEY_LEFT);
		break;

		// Right mouse button clicked
	case WM_RBUTTONDOWN:
		if (hWnd == GetForegroundWindow())
			Manager::PressMouseKey(MOUSE_KEYS::MOUSE_KEY_RIGHT);
		break;

		// Right mouse button released
	case WM_RBUTTONUP:
		if (hWnd == GetForegroundWindow())
			Manager::ReleaseMouseKey(MOUSE_KEYS::MOUSE_KEY_RIGHT);
		break;

		// Middle mouse button clicked
	case WM_MBUTTONDOWN:
		if (hWnd == GetForegroundWindow())
			Manager::PressMouseKey(MOUSE_KEYS::MOUSE_KEY_MIDDLE);
		break;

		// Middle mouse button released
	case WM_MBUTTONUP:
		if (hWnd == GetForegroundWindow())
			Manager::ReleaseMouseKey(MOUSE_KEYS::MOUSE_KEY_MIDDLE);
		break;

#pragma endregion

#pragma region Keyboard Handling

		//Key is pressed down
	case WM_KEYDOWN:
		if (hWnd != GetForegroundWindow())
			break;

		
		// Switch on which key is pressed (maybe want to do special actions for certain keys? like handling alt+f4
		switch (wParam)
		{
			//case VK_ESCAPE:
			//	DestroyWindow(hWnd);
			//	break;


		case 0:

			

		default:
			// the 31st bit of this lParam is whether or not the key was pressed the previous frame
			if (!(lParam & (1 << 30)))
				Manager::PressKey(wParam);
			else
				// every 15 frames, repeat the key that was held down (the first 16 bytes are the repeat count)
				if (lParam & 15U)
					Manager::RepeatKey();
			break;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
		break;

		//Key is released
	case WM_KEYUP:
		switch (wParam)
		{

		case 0:

		default:
			Manager::ReleaseKey(wParam);
			break;
		}
		break;

		// System key is pressed
	case WM_SYSKEYDOWN:

		if (lParam & 1 << 29)
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		break;

		// System key is released
	case WM_SYSKEYUP:

		break;

		// If key is pressed, send what character that represents
	case WM_CHAR:
		Manager::PressChar(wParam);
		break;		

#pragma endregion

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}
