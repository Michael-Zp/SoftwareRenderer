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
VOID OnPaint(const HWND hwnd, const HDC hdc, const Window* window);




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
		window->SetSize(LOWORD(lParam), HIWORD(lParam));
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



VOID OnPaint(const HWND hwnd, const HDC hdc, const Window* window)
{


	Graphics graphics(hdc);

	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP bitmapBuffer = CreateCompatibleBitmap(hdc, window->GetWidth(), window->GetHeight());
	HBITMAP bitmapBufferOld = (HBITMAP)SelectObject(hdcBuffer, bitmapBuffer);
	//DeleteDC(hdcBuffer);
	//DeleteObject(bitmapBuffer);

	//FillRect(hdcBuffer, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	//BitBlt(hdc, -rect.left, -rect.top, window->GetWidth(), window->GetHeight(), hdcBuffer, 0, 0, SRCCOPY);
	//SelectObject(hdcBuffer, bitmapBufferOld);
	//DeleteDC(hdcBuffer);
	//DeleteObject(bitmapBuffer);



	//Gdiplus::Bitmap bitmap(window->GetWidth(), window->GetHeight(), PixelFormat32bppARGB);
	//auto* bitmapData = new Gdiplus::BitmapData;
	//Rect bitmapRect(0, 0, window->GetWidth(), window->GetHeight());
	//bitmap.LockBits(&bitmapRect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, bitmapData);
	//auto* pixels = static_cast<unsigned*>(bitmapData->Scan0);
	//{
	//	int height = window->GetHeight();
	//	int width = window->GetWidth();
	//	int length = height * width;
	//	float totalTime = window->GetTimer().GetTotalTime();
	//	for (int i = 0; i < length; i++)
	//	{
	//		int x = i % width;
	//		int y = i / width;
	//		//if ((float)x / width > (sin(totalTime) + 1) / 2.0)
	//		//{
	//			pixels[y * width + x] = toBitmapPixel(255, 0, 0, 255);
	//		//}
	//	}
	//}
	//bitmap.UnlockBits(bitmapData);

	//graphics.DrawImage(&bitmap, 0, 0);

	RECT fullRect;
	fullRect.left = 0;
	fullRect.top = 0;
	fullRect.right = window->GetWidth();
	fullRect.bottom = window->GetHeight();
	FillRect(hdcBuffer, &fullRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	Graphics bufferGrapics(hdcBuffer);
	//bufferGrapics.DrawImage(&bitmap, 0, 0);
	std::string text = std::to_string(window->GetTimer().GetDeltaTime());
	DrawTextA(hdcBuffer, text.c_str(), text.length(), &fullRect, DT_LEFT | DT_TOP);
	BitBlt(hdc, 0, 0, window->GetWidth(), window->GetHeight(), hdcBuffer, 0, 0, SRCCOPY);
	SelectObject(hdcBuffer, bitmapBufferOld);




	//for (int x = 0; x < window->GetWidth(); x++)
	//{
	//    for (int y = 0; y < window->GetHeight(); y++)
	//    {
	//        if ((float)x / window->GetWidth() > (sin(window->GetTimer().GetDeltaTime()) + 1) / 2.0)
	//        {
	//            SetPixel(hdc, x, y, RGB(255, 0, 0));
	//        }
	//    }
	//}

}