#ifndef WINDOW_PROC_H
#define WINDOW_PROC_H


#include "input.h"


LRESULT CALLBACK WndProc(HWND hWnd, UINT message_identifier, WPARAM wParam, LPARAM lParam)
{
	switch (message_identifier)
	{
		case WM_KEYDOWN:
			input->HandleKeyboardInput(wParam, lParam, true);
			break;
		case WM_KEYUP:
			input->HandleKeyboardInput(wParam, lParam, false);
			break;


		case WM_LBUTTONDOWN:
			input->HandleMouseInput(wParam, lParam, true, LBUTTON);
			break;
		case WM_MBUTTONDOWN:
			input->HandleMouseInput(wParam, lParam, true, MBUTTON);
			break;
		case WM_RBUTTONDOWN:
			input->HandleMouseInput(wParam, lParam, true, RBUTTON);
			break;


		case WM_LBUTTONUP:
			input->HandleMouseInput(wParam, lParam, false, LBUTTON);
			break;
		case WM_MBUTTONUP:
			input->HandleMouseInput(wParam, lParam, false, MBUTTON);
			break;
		case WM_RBUTTONUP:
			input->HandleMouseInput(wParam, lParam, false, RBUTTON);
			break;

		case WM_MOUSEWHEEL:
			input->HandleMouseInput(wParam, lParam, false, MWHEEL);
			break;


		case WM_MOUSEMOVE:
			input->UpdateMousePosition(lParam);
			break;


			// The order of messages after receiving user's will to close the program is: WM_CLOSE -> WM_DESTROY -> WM_QUIT
		case WM_DESTROY:
			PostQuitMessage(0);			// the arg is passed to ReleaseMain()
			break_main = true;
			break;
		default:
			return DefWindowProc(hWnd, message_identifier, wParam, lParam);
	}

	return 0;
}


#endif
