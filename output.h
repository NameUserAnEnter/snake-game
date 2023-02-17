#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>
#include "input.h"
#include "framerate.h"
#include "functions.h"


enum info
{
	snake_pos = 1,
	target_pos,
	snake_game_output,
	mouse_pos,
	window_res,
	timing
};

enum output_modes
{
	testing,
	gameplay
};

static bool add_output[10] = { false, false, false, false, false,
							   false, false, false, false, false };

inline void init_output()
{
	static const output_modes output_mode = testing;
	switch (output_mode)
	{
		case testing:
			add_output[snake_pos] = true;
			add_output[target_pos] = true;
			add_output[mouse_pos] = false;
			add_output[window_res] = false;
			add_output[timing] = true;
			add_output[snake_game_output] = true;
			break;
		case gameplay:
			add_output[snake_pos] = true;
			add_output[target_pos] = true;
			add_output[mouse_pos] = false;
			add_output[window_res] = false;
			add_output[timing] = false;
			add_output[snake_game_output] = true;
			break;
	}
}


template<typename T>
inline void print_output(T* snake_game, _sys* sys, _input* input, _framerate* framerate)
{
	if (!snake_game->playing) return;

	static std::string	column_left;
	static std::string	column_right;

	column_left.clear();
	column_left = "";

	column_right.clear();
	column_right = "";

	for (int i = 0; i < 10; i++)
	{
		input->k_numeric[i].update_state();
		if (input->k_numeric[i].single_hit)
		{
			add_output[i] = !add_output[i];
			input->k_numeric[i].single_hit = false;
		}
	}


	std::string space1 = "\n";
	std::string space2 = "\n\n";
	if (add_output[snake_pos])
	{
		if (column_left.size() != 0) { column_left += space1;	column_right += space1; }

		column_left += "Snake position: ";						column_right += std::to_string(snake_game->GetX()) + ", " + std::to_string(snake_game->GetY());
	}
	if (add_output[target_pos])
	{
		if (column_left.size() != 0) { column_left += space1;	column_right += space1; }

		column_left += "Target position: ";						column_right += std::to_string(snake_game->GetTargetX()) + ", " + std::to_string(snake_game->GetTargetY());
	}
	if (add_output[snake_game_output])
	{
		if (column_left.size() != 0) { column_left += space2;	column_right += space2; }

		column_left += "Score: ";								column_right += std::to_string(snake_game->score);
		column_left += "\nTries left: ";						column_right += "\n" + std::to_string(snake_game->tries_left);
		column_left += "\nTime passed: ";						column_right += "\n" + std::to_string(snake_game->time_passed);
	}
	if (add_output[mouse_pos])
	{
		if (column_left.size() != 0) { column_left += space1;	column_right += space1; }

		if (input->mouse_left == true) {		column_left += "Mouse position: ";	column_right += std::to_string(input->mouse_x) + ", " + std::to_string(input->mouse_y) + " | LMB"; }
		else if (input->mouse_right == true) {	column_left += "Mouse position: ";	column_right += std::to_string(input->mouse_x) + ", " + std::to_string(input->mouse_y) + " | RMB"; }
		else {									column_left += "Mouse position: ";	column_right += std::to_string(input->mouse_x) + ", " + std::to_string(input->mouse_y); }
	}
	if (add_output[window_res])
	{
		if (column_left.size() != 0) { column_left += space1;	column_right += space1; }

		column_left += "Window resolution: ";					column_right += std::to_string(sys->uWindowWidth) + "x" + std::to_string(sys->uWindowHeight);
	}
	if (add_output[timing])
	{
		if (column_left.size() != 0) { column_left += space2;	column_right += space2; }

		column_left += "Frames per second: ";					column_right += std::to_string(framerate->previous_second_frame_count);
		column_left += "\ntime_factor: ";						column_right += "\n" + std::to_string((int)framerate->time_factor);
		column_left += "\noffset: ";							column_right += "\n" + std::to_string((int)snake_game->snake_offset);
		column_left += "\nin last second: ";					column_right += "\n" + std::to_string(snake_game->total_offset_in_last_second);
	}




	sys->Print_right(column_right);
	sys->Print_left(column_left);


	column_left.clear();
	column_right.clear();
}

#endif
