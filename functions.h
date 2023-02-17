#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include "operations.h"
#include "color_bytes.h"
#include "input.h"
#include "framerate.h"
#include <windows.h>


typedef unsigned char byte_type;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

struct _sys
{
	HINSTANCE hMainInstance;							// current instance
	HWND hMainWindow;



	unsigned int uScreenWidth;
	unsigned int uScreenHeight;


	unsigned int uWindowWidth;
	unsigned int uWindowHeight;

	unsigned int uWindowPosX;
	unsigned int uWindowPosY;

	solid background;



	// buffers
	HDC frontbuffer_dc;
	HDC backbuffer_dc;
	HBITMAP backbuffer_bitmap;
	HBITMAP cleanbuffer_bitmap;

	size_t			backbuffer_memory_block_size;
	byte_type* backbuffer_bytes;
	BITMAPINFOHEADER	backbuffer_bitmapinfo;

	// fonts & text
	HFONT font;






	void			SetWindow(unsigned int window_width, unsigned int window_height, solid _background);
	bool			InitWindow(HINSTANCE hInstance, int nCmdShow);
	unsigned short	RegisterWindowClass(HINSTANCE hInstance);

	void			SetMainWindowTitle(const wchar_t* title);
	bool			within_window(int x, int y);

	void			InitializeBuffers();
	void			ReleaseBuffers();
	void			update_buffer(HDC destination, HDC source);

	void Print_left(std::string output);
	void Print_right(std::string output);
	void Print(std::string output, unsigned int x, unsigned int y);

	int	 SetOnBackBuffer(int x, int y, int width, int height, color* bytes);
	void ClearBackBuffer();
	void UpdateFrontBuffer();
};






// Main window styles:
//#define MAIN_WINDOW_STYLES  		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
#define MAIN_WINDOW_STYLES  		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
#define MAIN_WINDOW_STYLES_EX		WS_EX_LEFT
//#define MAIN_WINDOW_STYLES_EX		WS_EX_CLIENTEDGE

// Window functions:
#define CreateMainWindow(window_x, window_y, window_width, window_height, window_title, window_class, window_styles, window_styles_ex) CreateWindowEx(window_styles_ex, window_class, window_title, window_styles, window_x, window_y, window_width, window_height, nullptr, nullptr, hInstance, nullptr)

#endif
