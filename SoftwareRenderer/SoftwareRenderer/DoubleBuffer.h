#pragma once

#include <Windows.h>
#include <gdiplus.h>

class BitmapBuffer
{
public:

	BitmapBuffer(HDC hdc, WORD width, WORD height) : width(width), height(height)
	{
		hdcBuffer = CreateCompatibleDC(hdc);
		bitmapBuffer = CreateCompatibleBitmap(hdc, width, height);
		bitmapBufferOld = (HBITMAP)SelectObject(hdcBuffer, bitmapBuffer);

		buffer = new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
		bitmapData = new Gdiplus::BitmapData;
		bitmapRect = Gdiplus::Rect(0, 0, width, height);

		bufferGrapics = new Gdiplus::Graphics(hdcBuffer);

		fullRect.left = 0;
		fullRect.top = 0;
		fullRect.right = width;
		fullRect.bottom = height;
	}

	~BitmapBuffer()
	{
		delete bufferGrapics;
		delete buffer;
		DeleteDC(hdcBuffer);
		DeleteObject(bitmapBuffer);
		DeleteObject(bitmapBufferOld);
	}

	unsigned* GetPixelBuffer()
	{
		buffer->LockBits(&bitmapRect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, bitmapData);

		return static_cast<unsigned*>(bitmapData->Scan0);
	}

	void ClearBuffer(HBRUSH brush)
	{
		FillRect(hdcBuffer, &fullRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	}

	void WriteBuffer()
	{
		buffer->UnlockBits(bitmapData);

		bufferGrapics->DrawImage(buffer, 0, 0);
	}

	HDC GetNextFrame() { return hdcBuffer; };

private:
	int width = 0;
	int height = 0;

	HDC hdcBuffer;
	HBITMAP bitmapBuffer;
	HBITMAP bitmapBufferOld;

	RECT fullRect;

	Gdiplus::Bitmap* buffer;
	Gdiplus::BitmapData* bitmapData;
	Gdiplus::Rect bitmapRect;
	Gdiplus::Graphics* bufferGrapics;
};