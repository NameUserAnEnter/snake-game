#include "input.h"


_input::_input()
{
	mouse_left = false;
	mouse_middle = false;
	mouse_right = false;

	mouse_x = 0;
	mouse_y = 0;

	mouse_wheel_up = false;
	mouse_wheel_down = false;

	k_up = false;
	k_down = false;
	k_left = false;
	k_right = false;

	k_space = false;
	k_escape = false;
	k_backspace = false;
	k_tab = false;
	k_enter = false;

	k_lshift = false;
	k_rshift = false;
	k_lctrl = false;
	k_rctrl = false;
	k_lalt = false;
	k_ralt = false;

	k_shift = false;
	k_ctrl = false;
	k_alt = false;


	for (int i = 0; i < alphabet_length; i++)
	{
		k_alphabet[i] = false;
	}

	for (int i = 0; i < 10; i++)
	{
		k_numeric[i] = false;
	}
}


void _input::HandleKeyboardInput(long virtual_key_code, long information, bool pressed_down)
{
	switch (virtual_key_code)
	{
		case VK_UP:
			k_up = pressed_down;
			break;
		case VK_DOWN:
			k_down = pressed_down;
			break;
		case VK_LEFT:
			k_left = pressed_down;
			break;
		case VK_RIGHT:
			k_right = pressed_down;
			break;


		case VK_SPACE:
			k_space = pressed_down;
			break;
		case VK_ESCAPE:
			k_escape = pressed_down;
			break;
		case VK_BACK:
			k_backspace = pressed_down;
			break;
		case VK_TAB:
			k_tab = pressed_down;
			break;
		case VK_RETURN:
			k_enter = pressed_down;
			break;


		case VK_LSHIFT:
			k_lshift = pressed_down;
			break;
		case VK_RSHIFT:
			k_rshift = pressed_down;
			break;
		case VK_SHIFT:
			k_shift = pressed_down;
		case VK_LCONTROL:
			k_lctrl = pressed_down;
			break;
		case VK_RCONTROL:
			k_rctrl = pressed_down;
			break;
		case VK_CONTROL:
			k_ctrl = pressed_down;
			break;
		case VK_LMENU:
			k_lalt = pressed_down;
			break;
		case VK_RMENU:
			k_ralt = pressed_down;
			break;
		case VK_MENU:
			k_alt = pressed_down;
			break;

		
		case 0x30:
			k_numeric[0] = pressed_down;
			break;
		case 0x31:
			k_numeric[1] = pressed_down;
			break;
		case 0x32:
			k_numeric[2] = pressed_down;
			break;
		case 0x33:
			k_numeric[3] = pressed_down;
			break;
		case 0x34:
			k_numeric[4] = pressed_down;
			break;
		case 0x35:
			k_numeric[5] = pressed_down;
			break;
		case 0x36:
			k_numeric[6] = pressed_down;
			break;
		case 0x37:
			k_numeric[7] = pressed_down;
			break;
		case 0x38:
			k_numeric[8] = pressed_down;
			break;
		case 0x39:
			k_numeric[9] = pressed_down;
			break;




		case 0x41:
			k_alphabet[keyboard_letter::A] = pressed_down;
			break;
		case 0x42:
			k_alphabet[keyboard_letter::B] = pressed_down;
			break;
		case 0x43:
			k_alphabet[keyboard_letter::C] = pressed_down;
			break;
		
		
		case 0x44:
			k_alphabet[keyboard_letter::D] = pressed_down;
			break;
		case 0x45:
			k_alphabet[keyboard_letter::E] = pressed_down;
			break;
		case 0x46:
			k_alphabet[keyboard_letter::F] = pressed_down;
			break;
		
		
		case 0x47:
			k_alphabet[keyboard_letter::G] = pressed_down;
			break;
		case 0x48:
			k_alphabet[keyboard_letter::H] = pressed_down;
			break;
		case 0x49:
			k_alphabet[keyboard_letter::I] = pressed_down;
			break;
		
		
		case 0x4A:
			k_alphabet[keyboard_letter::J] = pressed_down;
			break;
		case 0x4B:
			k_alphabet[keyboard_letter::K] = pressed_down;
			break;
		case 0x4C:
			k_alphabet[keyboard_letter::L] = pressed_down;
			break;
		
		
		case 0x4D:
			k_alphabet[keyboard_letter::M] = pressed_down;
			break;
		case 0x4E:
			k_alphabet[keyboard_letter::N] = pressed_down;
			break;
		case 0x4F:
			k_alphabet[keyboard_letter::O] = pressed_down;
			break;
		
		
		case 0x50:
			k_alphabet[keyboard_letter::P] = pressed_down;
			break;
		case 0x51:
			k_alphabet[keyboard_letter::Q] = pressed_down;
			break;
		case 0x52:
			k_alphabet[keyboard_letter::R] = pressed_down;
			break;
		case 0x53:
			k_alphabet[keyboard_letter::S] = pressed_down;
			break;
		
		
		case 0x54:
			k_alphabet[keyboard_letter::T] = pressed_down;
			break;
		case 0x55:
			k_alphabet[keyboard_letter::U] = pressed_down;
			break;
		case 0x56:
			k_alphabet[keyboard_letter::V] = pressed_down;
			break;
		
		
		case 0x57:
			k_alphabet[keyboard_letter::W] = pressed_down;
			break;
		case 0x58:
			k_alphabet[keyboard_letter::X] = pressed_down;
			break;
		case 0x59:
			k_alphabet[keyboard_letter::Y] = pressed_down;
			break;
		case 0x5A:
			k_alphabet[keyboard_letter::Z] = pressed_down;
			break;
	}
}

void _input::HandleMouseInput(long mouse_key_code, long position, bool pressed_down, mouse_button mb)
{
	switch (mb)
	{
		case LBUTTON:
			mouse_left = pressed_down;
			break;
		case MBUTTON:
			mouse_middle = pressed_down;
			break;
		case RBUTTON:
			mouse_right = pressed_down;
			break;
		case MWHEEL:
			short high_word = (short)((unsigned long long)mouse_key_code >> 16) & 0xFFFF;
			if (high_word > 0)
			{
				mouse_wheel_up = true;
				mouse_wheel_down = false;
			}
			else if (high_word == 0)
			{
				mouse_wheel_up = false;
				mouse_wheel_down = false;
			}
			else if (high_word < 0)
			{
				mouse_wheel_up = false;
				mouse_wheel_down = true;
			}

			break;
	}

	if (mb != MWHEEL)
	{
		//mouse_x = LOWORD(position);
		//mouse_y = HIWORD(position);
		mouse_x = (unsigned short)((unsigned long long)position) & 0xFFFF;
		mouse_y = (unsigned short)((unsigned long long)position >> 16) & 0xFFFF;
	}
}

void _input::UpdateMousePosition(long position)
{
	mouse_x = (unsigned short)((unsigned long long)position) & 0xFFFF;
	mouse_y = (unsigned short)((unsigned long long)position >> 16) & 0xFFFF;
}



key::key()
{
	pressed = false;

	single_hit = false;
	last_state = false;

	hold_cycle = false;
	held_down = false;
}

void key::update_state()
{
	static clock_t cycle_time;
	static clock_t cycle_last = clock();

	static unsigned int time_interval = 350;

	hold_cycle = false;
	if (last_state != pressed)
	{
		last_state = pressed;
		if (pressed)
		{
			single_hit = !single_hit;

			cycle_last = cycle_time;
			time_interval = 350;
			held_down = true;
			hold_cycle = true;
		}
	}

	if (!pressed) held_down = false;

	if (held_down)
	{
		cycle_time = clock();
		if (cycle_time - cycle_last >= time_interval || hold_cycle)
		{
			cycle_last = cycle_time;

			if (time_interval > 50) time_interval -= 150;
			else if (time_interval > 20) time_interval -= 5;

			hold_cycle = true;
		}
	}
}





key::operator bool()
{
	return pressed;
}

void key::operator=(bool operand)
{
	pressed = operand;
}

void key::operator=(key operand)
{
	pressed = operand.pressed;
}

bool key::operator==(bool operand)
{
	return operand == pressed;
}

bool key::operator==(key operand)
{
	return operand.pressed == pressed;
}

bool key::operator!=(bool operand)
{
	return operand != pressed;
}

bool key::operator!=(key operand)
{
	return operand.pressed != pressed;
}
