#ifndef INPUT_H
#define INPUT_H

#include <windows.h>
#include <time.h>

enum mouse_button
{
	LBUTTON,
	MBUTTON,
	RBUTTON,
	MWHEEL
};

enum keyboard_letter
{
	A, B, C,
	D, E, F,
	G, H, I,
	J, K, L,
	M, N, O,
	P, Q, R, S,
	T, U, V,
	W, X, Y, Z
};
#define alphabet_length ((int)'Z' - (int)'A' + 1)

struct key
{
	bool pressed;


	bool single_hit;
	bool last_state;

	bool hold_cycle;
	bool held_down;

	key();
	void update_state();

	operator bool();

	void operator=(bool);
	void operator=(key);

	bool operator==(bool);
	bool operator==(key);

	bool operator!=(bool);
	bool operator!=(key);
};


struct _input
{
	key mouse_left;
	key mouse_middle;
	key mouse_right;

	unsigned int mouse_x;
	unsigned int mouse_y;

	key mouse_wheel_up;
	key mouse_wheel_down;

	key k_up;
	key k_down;
	key k_left;
	key k_right;

	key k_space;
	key k_escape;
	key k_backspace;
	key k_tab;
	key k_enter;
	
	key k_lshift;
	key k_rshift;
	key k_lctrl;
	key k_rctrl;
	key k_lalt;
	key k_ralt;

	key k_shift;
	key k_ctrl;
	key k_alt;

	key k_alphabet[(int)'Z' - (int)'A' + 1];
	key k_numeric[10];

	_input();


	void HandleKeyboardInput(long virtual_key_code, long information, bool pressed_down);
	void HandleMouseInput(long mouse_key_code, long position, bool pressed_down, mouse_button mb);
	void UpdateMousePosition(long position);
};

#endif
