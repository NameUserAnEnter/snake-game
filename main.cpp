#include "definitions.h"
#include "functions.h"


bool		break_main;
_sys*		sys;
_input*		input;
_framerate* framerate;


#include "snake.h"
#include "output.h"
#include "window_proc.h"

int main()
{
	framerate	= new _framerate;
	input		= new _input;
	sys			= new _sys;

	sys->SetWindow(1280, 900, solid(50, 0, 255));

	InitializeMain(sys);
	sys->SetMainWindowTitle(L"Snake");
	sys->InitializeBuffers();

	
	Snake snake_game = Snake(sys, input, framerate, break_main);
	init_output();

	break_main = false;
	while (!break_main)
	{
		sys->ClearBackBuffer();

		snake_game.Update();
		if (snake_game.output) print_output<Snake>(&snake_game, sys, input, framerate);

		sys->UpdateFrontBuffer();

		while (get_sys_msg())
		{
			UpdateMain();		// ===> invoke WndProc()
		}
	}

	// free allocated memory & return any possible system error code
	sys->ReleaseBuffers();
	delete sys;
	delete input;
	delete framerate;
	return ReleaseMain();
}
