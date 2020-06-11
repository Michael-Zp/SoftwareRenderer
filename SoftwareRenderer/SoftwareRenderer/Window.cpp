#include "Window.h"

#include <Windows.h>
#include <ObjIdl.h>
#include <gdiplus.h>
#include <vector>
#include <math.h>
#include <string>

using namespace Gdiplus;

#pragma comment (lib, "Gdiplus.lib")

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID OnPaint(const HWND hwnd, const HDC hdc, Window* window);




void RedrawWindowTimerFunction(const HWND hWnd, const Window* window)
{
	if (window->IsLastFrameRendered())
	{
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}
}

int Window::Init(const HINSTANCE hInstance, const int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	HWND hWnd;
	GdiplusStartupInput gdiplusStartupInput;
	WNDCLASS wc = { };

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.

	hWnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Learn to Program Windows",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		this        // Additional application data
	);

	if (hWnd == NULL)
	{
		return 0;
	}

	SetTimer(hWnd, 1, 1, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
}

void Window::Run()
{
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static Window* window;
	switch (uMsg)
	{
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;

	case WM_NCCREATE:
		window = reinterpret_cast<Window*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		break;

	case WM_SIZE:
		window->SetSize(GetDC(hWnd), LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_TIMER:
		RedrawWindowTimerFunction(hWnd, window);
		return 0;

	case WM_ERASEBKGND:
		return TRUE;

	case WM_PAINT:
		window->BeginFrame();
		if (!window->GetTimer().IsInitialized())
		{
			window->InitializeTimer();
		}
		else
		{
			window->UpdateTimer();
		}

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		OnPaint(hWnd, hdc, window);
		EndPaint(hWnd, &ps);
		window->EndFrame();
		return 0;

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



unsigned inline toBitmapPixel(char red, char green, char blue, char alpha)
{
	return (alpha << 24) + (red << 16) + (green << 8) + blue;
}



VOID OnPaint(const HWND hwnd, const HDC hdc, Window* window)
{
	window->GetDoubleBuffer().ClearBuffer(WHITE_BRUSH);

	auto* pixels = window->GetDoubleBuffer().GetPixelBuffer();
	{
		int height = window->GetHeight();
		int width = window->GetWidth();
		int length = height * width;
		float totalTime = window->GetTimer().GetTotalTime();
		for (int i = 0; i < length; i++)
		{
			int x = i % width;
			int y = i / width;
			if ((float)x / width > (sin(totalTime) + 1) / 2.0)
			{
				pixels[y * width + x] = toBitmapPixel(255, 0, 0, 255);
			}
			else
			{
				pixels[y * width + x] = toBitmapPixel(255, 255, 255, 255);
			}
		}
	}

	window->GetDoubleBuffer().WriteBuffer();

	RECT fullRect;
	fullRect.left = 0;
	fullRect.top = 0;
	fullRect.right = window->GetWidth();
	fullRect.bottom = window->GetHeight();

	std::string text = std::to_string(window->GetTimer().GetDeltaTime());
	DrawTextA(window->GetDoubleBuffer().GetNextFrame(), text.c_str(), text.length(), &fullRect, DT_LEFT | DT_TOP);

	BitBlt(hdc, 0, 0, window->GetWidth(), window->GetHeight(), window->GetDoubleBuffer().GetNextFrame(), 0, 0, SRCCOPY);
}