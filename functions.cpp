#include "functions.h"


static wchar_t szTitle[]	   = L"Main";					// L"literal" for 16-bit character string literal
static wchar_t szWindowClass[] = L"MainWinClass";			// L"literal" for 16-bit character string literal

static const unsigned int bytes_per_pixel = 3;

static unsigned int uScreenWidth = GetSystemMetrics(SM_CXSCREEN);
static unsigned int uScreenHeight = GetSystemMetrics(SM_CYSCREEN);


void _sys::SetWindow(unsigned int window_width, unsigned int window_height, solid _background)
{
	
	uWindowWidth = window_width;
	uWindowHeight = window_height;
	background = _background;
}

bool _sys::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	hMainInstance = hInstance;
	
	uWindowPosX = 0;
	uWindowPosY = 0;


	hMainWindow = CreateMainWindow(uWindowPosX, uWindowPosY, uWindowWidth, uWindowHeight, szTitle, szWindowClass, MAIN_WINDOW_STYLES, MAIN_WINDOW_STYLES_EX);

	if (!hMainWindow)
	{
		return false;
	}

	ShowWindow(hMainWindow, nCmdShow);
	UpdateWindow(hMainWindow);

	return true;
}

unsigned short _sys::RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	//wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(background.R, background.G, background.B));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex);
}

void _sys::SetMainWindowTitle(const wchar_t* title)
{
	SetWindowText(hMainWindow, title);
}



void _sys::InitializeBuffers()
{
	frontbuffer_dc = GetDC(hMainWindow);

	backbuffer_dc = CreateCompatibleDC(frontbuffer_dc);
	backbuffer_bitmap = CreateCompatibleBitmap(frontbuffer_dc, uWindowWidth, uWindowHeight);
	SelectObject(backbuffer_dc, backbuffer_bitmap);

	cleanbuffer_bitmap = CreateCompatibleBitmap(frontbuffer_dc, uWindowWidth, uWindowHeight);

	update_buffer(backbuffer_dc, frontbuffer_dc);
	

	backbuffer_bytes = NULL;

	backbuffer_bitmapinfo.biSize			= sizeof(BITMAPINFOHEADER);
	backbuffer_bitmapinfo.biWidth			= uWindowWidth;
	backbuffer_bitmapinfo.biHeight			= uWindowHeight * (-1);
	backbuffer_bitmapinfo.biPlanes			= 1;
	backbuffer_bitmapinfo.biBitCount		= 8 * bytes_per_pixel;
	backbuffer_bitmapinfo.biCompression	= BI_RGB;

	backbuffer_memory_block_size = (size_t)uWindowWidth * bytes_per_pixel * (size_t)uWindowHeight;
	backbuffer_bytes = (byte_type*)calloc(backbuffer_memory_block_size, sizeof(byte_type));
	
	GetDIBits(frontbuffer_dc, backbuffer_bitmap, 0, uWindowHeight, backbuffer_bytes,	(BITMAPINFO*)&backbuffer_bitmapinfo, DIB_RGB_COLORS);
	SetDIBits(NULL,				   cleanbuffer_bitmap, 0, uWindowHeight, backbuffer_bytes,	(BITMAPINFO*)&backbuffer_bitmapinfo, DIB_RGB_COLORS);


	// Fonts & text
	font = NULL;
	font = CreateFont(18, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, (DEFAULT_PITCH | FF_DONTCARE), TEXT("Consolas"));
	if(SetTextColor(backbuffer_dc, RGB(255, 255, 255)) == CLR_INVALID) DisplayMessage("Failed to invoke SetTextColor()");
	SetBkMode(backbuffer_dc, RGB(background.R, background.G, background.B));

	if (font != NULL)		SelectObject(backbuffer_dc, font);
}

void _sys::Print_left(std::string output)
{
	unsigned int offset_x = 20;
	unsigned int offset_y = 20;
	RECT rect = { offset_x, offset_y, uWindowWidth - offset_x, uWindowHeight - offset_y };
	
	// wstring() method is allocating a memory block to which it returns a pointer - it must be released afterwards
	wchar_t* w_output = wstring(output);
	DrawText(backbuffer_dc, w_output, output.size(), &rect, DT_LEFT);
	free(w_output);	
}

void _sys::Print_right(std::string output)
{
	unsigned int offset_x = 240;
	unsigned int offset_y = 20;
	RECT rect = { 0, offset_y, offset_x, uWindowHeight - offset_y };

	// wstring() method is allocating a memory block to which it returns a pointer - it must be released afterwards
	wchar_t* w_output = wstring(output);
	DrawText(backbuffer_dc, w_output, output.size(), &rect, DT_RIGHT);
	free(w_output);
}

void _sys::Print(std::string output, unsigned int x, unsigned int y)
{
	unsigned int offset_x = x;
	unsigned int offset_y = y;
	RECT rect = { x, y, uWindowWidth, uWindowHeight };

	// wstring() method is allocating a memory block to which it returns a pointer - it must be released afterwards
	wchar_t* w_output = wstring(output);
	DrawText(backbuffer_dc, w_output, output.size(), &rect, DT_LEFT);
	free(w_output);
}

int _sys::SetOnBackBuffer(int x, int y, int width, int height, color* bytes)
{
	if (bytes == nullptr) return -1;


	for (int i = y; i < y + height; i++)
	{
		for (int j = x; j < x + width; j++)
		{
			if (within_window(j, i))
			{
				if ((j - x) >= 0 && (j - x) < width		&&		(i - y) >= 0 && (i - y) < height)
				{
					if (bytes[(i - y) * width + (j - x)].A != 0)
					{
						backbuffer_bytes[i * (uWindowWidth * bytes_per_pixel) + (j * bytes_per_pixel) + 0] = bytes[(i - y) * width + (j - x)].B;
						backbuffer_bytes[i * (uWindowWidth * bytes_per_pixel) + (j * bytes_per_pixel) + 1] = bytes[(i - y) * width + (j - x)].G;
						backbuffer_bytes[i * (uWindowWidth * bytes_per_pixel) + (j * bytes_per_pixel) + 2] = bytes[(i - y) * width + (j - x)].R;
					}
				}
				else break;
			}
		}
	}

	return SetDIBits(NULL, backbuffer_bitmap, 0, uWindowHeight, backbuffer_bytes, (BITMAPINFO*)&backbuffer_bitmapinfo, DIB_RGB_COLORS);
}

void _sys::ClearBackBuffer()
{
	GetDIBits(frontbuffer_dc, cleanbuffer_bitmap, 0, uWindowHeight, backbuffer_bytes, (BITMAPINFO*)&backbuffer_bitmapinfo, DIB_RGB_COLORS);
	SetDIBits(NULL, backbuffer_bitmap, 0, uWindowHeight, backbuffer_bytes, (BITMAPINFO*)&backbuffer_bitmapinfo, DIB_RGB_COLORS);
}

void _sys::UpdateFrontBuffer()
{
	update_buffer(frontbuffer_dc, backbuffer_dc);
}

void _sys::ReleaseBuffers()
{
	free(backbuffer_bytes);

	if (font != NULL) DeleteObject(font);
	DeleteObject(cleanbuffer_bitmap);
	DeleteObject(backbuffer_bitmap);
	DeleteDC(backbuffer_dc);
}

void _sys::update_buffer(HDC destination, HDC source)
{
	BitBlt(destination, 0, 0, uWindowWidth, uWindowHeight, source, 0, 0, SRCCOPY);
}



bool _sys::within_window(int x, int y)
{
	if (x >= 0 && x < uWindowWidth && y >= 0 && y < uWindowHeight) return true;
	else return false;
}

