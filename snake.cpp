#include "snake.h"

Snake::Snake(_sys* _sys_, _input* _input_, _framerate* _framerate_, bool& _break_main_): break_main(_break_main_)
{
	srand(time(NULL));

	sys = _sys_;
	input = _input_;
	framerate = _framerate_;

	tile_width = 40;
	tile_height = 40;

	map_width = sys->uWindowWidth - 11;
	map_height = sys->uWindowHeight - 33;


	playing = false;
	leave = false;
	viewing_scores = false;

	option_width = 96;
	line_height = 18;
	new_score = false;

	output = false;

	reset_values();
	sort_scores();
}

void Snake::reset_values()
{
	snake_length = 0;

	snake_direction = up;
	next_direction = snake_direction;

	tries_left = 3;
	score = 0;

	target_x = 0;
	target_y = 0;

	snake_offset = 0;

	target_index = 0;

	total_offset_in_last_second = 0;
	sum_offset = 0;

	radius = 0.0;
	angle = M_PI * 3 / 2;
	angle_offset = (M_PI / 2) * 0.05;

	stop_snake = true;
	died = false;

	time_start = framerate->count_seconds;
	time_passed = framerate->count_seconds - time_start;

	angle_offset_span = 0;
	radius_span = 0;
}



void Snake::Update()
{
	static bool last_state = false;
	if (playing != last_state)
	{
		if (playing)
		{
			init_scene();
		}
		last_state = playing;
	}

	
	if (playing)
	{
		static const bool limit_framerate = false;
		static const unsigned int max_rate = 60;

		if (!limit_framerate)
		{
			update_scene();
			render_scene();
		}
		else
		{
			static clock_t update_time;
			static clock_t update_last = clock();
			update_time = clock();
			if (update_time - update_last >= 1000 / max_rate)
			{
				update_last = update_time;
				update_scene();
				render_scene();
			}
		}

		SingleHit(input->k_escape, died = true; playing = false);
	}
	else if (died)
	{
		deadscreen();
	}
	else if (leave)
	{
		leavescreen();
	}
	else if (viewing_scores)
	{
		scorescreen();
	}
	else
	{
		menuscreen();
	}
}



struct hitbox
{
	int x; int y; int width; int height; hitbox(int _x, int _y, int _width, int _height) { x = _x; y = _y; width = _width; height = _height; } hitbox() {}
	bool within(int point_x, int point_y) { if (point_x >= x && point_y >= y && point_x < x + width && point_y < y + height) return true; else return false; }

	void operator=(hitbox operand) { x = operand.x; y = operand.y; width = operand.width; height = operand.height; }
};

struct score_row
{
	std::string name;
	std::string time;
	unsigned int score;
};

std::vector<score_row> sort_ascending(std::vector<score_row> data)
{
	std::vector<score_row> return_value;
	std::vector<score_row> data_sub1;
	std::vector<score_row> data_sub2;
	unsigned int pivot_point = data.size() / 2;

	for (int i = 0; i < data.size(); i++)
	{
		if (i == pivot_point)
		{
			if (data_sub1.size() <= data_sub2.size()) data_sub1.push_back(data[i]);
			else data_sub2.push_back(data[i]);
		}
		else if (data[i].score < data[pivot_point].score)
		{
			data_sub1.push_back(data[i]);
		}
		else
		{
			data_sub2.push_back(data[i]);
		}
	}

	if (data_sub1.size() > 1) data_sub1 = sort_ascending(data_sub1);
	if (data_sub2.size() > 1) data_sub2 = sort_ascending(data_sub2);

	for (int i = 0; i < data_sub1.size(); i++) return_value.push_back(data_sub1[i]);
	for (int i = 0; i < data_sub2.size(); i++) return_value.push_back(data_sub2[i]);

	return return_value;
}

std::vector<score_row> sort_descending(std::vector<score_row> data)
{
	std::vector<score_row> return_value(data.size());
	std::vector<score_row> copy = sort_ascending(data);
	for (int i = 0; i < data.size(); i++)
		return_value[i] = copy[data.size() - 1 - i];

	return return_value;
}

const char* scores_file = "scores";
void Snake::scorescreen()
{
	static const unsigned int max_display = 30;
	static unsigned int x = sys->uWindowWidth / 2 - 200 / 2;
	static unsigned int y = (sys->uWindowHeight / 2) - ((line_height / 2) * 6) - (max_display * line_height / 2);

	// Table
	///*
	static std::vector<std::string> column_names;
	static std::vector<std::string> column_scores;
	static std::vector<std::string> column_times;
	static std::string longest_name; static std::string longest_score; static std::string longest_time;

	static const std::string header_id = "No";
	static const std::string header_name = "Name";
	static const std::string header_score = "Score";
	static const std::string header_time = "Time";
	static const std::string separator = " | ";
	static unsigned int table_width = 8 * (header_id.size() + std::to_string('.').size() + separator.size() + header_name.size() + separator.size() + header_score.size() + separator.size() + header_time.size());
	static unsigned int last_ID = 0;
	//*/
	static std::vector<hitbox>		option_box;

	static unsigned int current_line = 0;
	static unsigned int hidden_above = 0;
	static unsigned int hidden_below = 0;
	static std::vector<std::string> data;
	static std::vector<std::string> whole_data;
	static bool move_list_up = false;
	static bool move_list_down = false;

	static bool reset_selection = true;
	static bool keep_list_offset = false;
	static bool read = true;

	// Reading data & resetting the table
	if (new_score) read = true;
	if (read)
	{
		// Counting lines & adjusting the above-in-below numbers
		///*
		{
			unsigned int count_lines = 1;	// there is always at least one line in a file
			FILE* counting = nullptr;
			fopen_s(&counting, scores_file, "r");
			if (counting != nullptr)
			{
				char current_char;
				do
				{
					current_char = getc(counting);
					if (current_char == '\n') count_lines++;
				} while (current_char != EOF);
				fclose(counting);
				count_lines--;	// remove the last empty line from the count
			}

			if (count_lines > max_display)	// only up to max_display records are shown even if the number of records in the file exceeds max_display
			{
				if (!move_list_down && !move_list_up && !keep_list_offset)
				{
					hidden_above = 0;
				}
				else if (move_list_up && hidden_above > 0)
				{
					hidden_above--;
					move_list_up = false;
				}
				else if (move_list_down && hidden_below > 0)
				{
					hidden_above++;
					move_list_down = false;
				}
			}
		}
		//*/

		// Reading records from the file
		FILE* scores = nullptr;
		fopen_s(&scores, scores_file, "r");
		if (scores != nullptr)
		{
			whole_data.clear();
			whole_data.push_back("");

			char current_char;
			do
			{
				current_char = getc(scores);
				if (current_char == '\n') whole_data.push_back("");
				else whole_data.back() += current_char;
			} while (current_char != EOF);

			fclose(scores);
			whole_data.pop_back();	// Remove the last empty line
		}

		// Adjusting the in-part of data
		///*
		data.clear();
		for (int i = 0; i < whole_data.size(); i++)
		{
			if (i >= hidden_above && i < hidden_above + max_display)
			{
				data.push_back(whole_data[i]);
			}
		}
		hidden_below = whole_data.size() - (hidden_above + max_display);
		//*/

		// Table adjustments
		///*
		column_names.clear(); column_scores.clear(); column_times.clear();
		longest_name = ""; longest_score = ""; longest_time = "";
		// Reading column data
		for (int i = 0; i < data.size(); i++)
		{
			enum column_type { name = 0, score, time } current_column = name;
			column_names.push_back("");
			column_scores.push_back("");
			column_times.push_back("");

			for (int j = 0; j < data[i].size(); j++)
			{
				if (data[i][j] == '=')
				{
					j += 2;
					while (j < data[i].size())
					{
						if (data[i][j] != ',')
						{
							if (current_column == name)  column_names[column_names.size() - 1] += data[i][j];
							if (current_column == score) column_scores[column_scores.size() - 1] += data[i][j];
							if (current_column == time)  column_times[column_times.size() - 1] += data[i][j];
							j++;
						}
						else break;
					}

					if (current_column == name)  current_column = score;
					else if (current_column == score) current_column = time;
					else break;
				}
			}

			if (column_names.back().size() >= longest_name.size())   longest_name = column_names.back();
			if (column_scores.back().size() >= longest_score.size()) longest_score = column_scores.back();
			if (column_times.back().size() >= longest_time.size())   longest_time = column_times.back();
		}

		if (longest_name.size() < header_name.size())  longest_name = header_name;
		if (longest_score.size() < header_score.size()) longest_score = header_score;
		if (longest_time.size() < header_time.size())  longest_time = header_time;

		// Resizing and aligning the columns
		for (int i = 0; i < data.size(); i++)
		{
			if (column_names[i].size() < longest_name.size())
			{
				int diff = longest_name.size() - column_names[i].size();
				for (int j = 0; j < diff; j++)
				{
					column_names[i] += ' ';
				}
			}
			if (column_scores[i].size() < longest_score.size())
			{
				int diff = longest_score.size() - column_scores[i].size();

				std::string temp = column_scores[i];
				column_scores[i] = "";
				for (int j = 0; j < diff; j++)
				{
					column_scores[i] += ' ';
				}
				column_scores[i] += temp;
			}
			if (column_times[i].size() < longest_time.size())
			{
				int diff = longest_time.size() - column_times[i].size();

				std::string temp = column_times[i];
				column_times[i] = "";
				for (int j = 0; j < diff; j++)
				{
					column_times[i] += ' ';
				}
				column_times[i] += temp;
			}
		}


		// Adding the ID column and calculating the whole table width
		last_ID = data.size() + hidden_above;
		if (last_ID < 10) last_ID = 10;
		table_width = 8 * (std::to_string(last_ID).size() + std::string(".").size() + separator.size() + longest_name.size() + separator.size() + longest_score.size() + separator.size() + longest_time.size());
		//*/

		x = sys->uWindowWidth / 2 - table_width / 2;

		// Resetting the variables
		{
			new_score = false;
			read = false;

			option_box.clear();
			option_box.resize(data.size() + 1);

			if (reset_selection)
			{
				current_line = data.size();
				reset_selection = false;
			}
		}
	}





	static bool ask = false;
	static bool confirm = false;

	// Mouse-wheel scrolling the table
	bool mouse_scroll = false;
	if (!ask)
	{
		if (current_line + hidden_above > 0)
		{
			WheelPull(input->mouse_wheel_up, read = true; move_list_up = true; mouse_scroll = true);
		}
		if (current_line < data.size() + hidden_below)
		{
			WheelPull(input->mouse_wheel_down, read = true; move_list_down = true; mouse_scroll = true);
		}
	}

	bool mouse_move = false;
	static unsigned int last_mouse_x = input->mouse_x;
	static unsigned int last_mouse_y = input->mouse_y;
	if (input->mouse_x != last_mouse_x || input->mouse_y != last_mouse_y || mouse_scroll)
	{
		last_mouse_x = input->mouse_x;
		last_mouse_y = input->mouse_y;
		mouse_move = true;
	}

	// Row hitboxes
	for (int i = 0; i < option_box.size(); i++)
	{
		option_box[i].width = table_width;
		option_box[i].width = 200;
		option_box[i].height = line_height;

		option_box[i].x = x;
		option_box[i].y = y + line_height * i;


		if (option_box[i].within(input->mouse_x, input->mouse_y) && !ask)
		{
			if (mouse_move) current_line = i;
			sys->Print("opt_" + std::to_string(i + 1), x, y + line_height * (data.size() + 4));
		}
	}

	// Print the table header line
	///*
	{
		std::string header_line = "";
		int diff = 0;
		diff = std::to_string(last_ID).size() - header_id.size();
		for (int i = 0; i < diff; i++) header_line += ' ';
		header_line += header_id;
		header_line += '.';
		header_line += separator;
		diff = longest_name.size() - header_name.size();
		header_line += header_name;
		for (int i = 0; i < diff; i++) header_line += ' ';
		header_line += separator;
		diff = longest_score.size() - header_score.size();
		for (int i = 0; i < diff; i++) header_line += ' ';
		header_line += header_score;
		header_line += separator;
		diff = longest_time.size() - header_time.size();
		for (int i = 0; i < diff; i++) header_line += ' ';
		header_line += header_time;
		sys->Print(header_line, x, y - line_height * 1);
		// the underline
		header_line = "";
		for (int i = 0; i < table_width / 8; i++) header_line += '_';
		sys->Print(header_line, x, y - line_height * 1);
	}
	//*/

	// Print the table
	///*
	for (int i = 0; i < data.size(); i++)
	{
		unsigned int index = i;

		unsigned int ID = index + 1 + hidden_above;

		std::string output_line = "";
		int diff = std::to_string(last_ID).size() - std::to_string(ID).size();
		for (int j = 0; j < diff; j++) output_line += ' ';
		output_line += std::to_string(ID);
		output_line += '.';

		output_line += separator;
		output_line += column_names[index];
		output_line += separator;
		output_line += column_scores[index];
		output_line += separator;
		output_line += column_times[index];


		if (index == current_line) sys->Print("> ", x - 8 * 2, y + line_height * index);
		sys->Print(output_line, x, y + line_height * index);
	}
	//*/

	// Print the bottom edge of the table
	///*
	{
		std::string header_line = "";
		for (int i = 0; i < table_width / 8; i++) header_line += '_';
		sys->Print(header_line, x, y + line_height * (data.size() - 1));
	}
	//*/

	// Print the data directly instead of a table
	/*
	for (int i = 0; i < data.size(); i++)
	{
		unsigned int ID = i + 1 + hidden_above;

		std::string output_line = "";
		output_line += std::to_string(ID) + ". " + data[i];

		if (i == current_line) sys->Print(">", x - 16, y + line_height * i);
		sys->Print(output_line, x, y + line_height * i);
	}
	*/

	// Moving around
	if (!ask)
	{
		if (current_line > 0)
		{
			HeldDown(input->k_up, current_line--);
		}
		else if (current_line + hidden_above > 0)
		{
			HeldDown(input->k_up, read = true; move_list_up = true);
		}

		if (current_line < data.size())
		{
			HeldDown(input->k_down, current_line++);
		}
		else if (current_line < data.size() + hidden_below)
		{
			HeldDown(input->k_down, read = true; move_list_down = true);
		}
	}

	// Which line is active
	if (current_line == data.size())
	{
		sys->Print("> ", x - 8 * 2, y + line_height * (data.size()));
		if (!ask) sys->Print("Press ENTER to leave.", x, y + line_height * (data.size() + 1));

		input->k_enter.update_state();
		input->mouse_left.update_state();
		if (input->k_enter.single_hit || (option_box[current_line].within(input->mouse_x, input->mouse_y) && input->mouse_left.single_hit))
		{
			if (input->k_enter.single_hit)																		input->k_enter.single_hit = false;
			if (option_box[current_line].within(input->mouse_x, input->mouse_y) && input->mouse_left.single_hit)	input->mouse_left.single_hit = false;

			reset_selection = true; viewing_scores = false; ask = false; confirm = false; keep_list_offset = false; read = true;
		}
	}
	else
	{
		if (!ask) sys->Print("Press ENTER to remove score.", x, y + line_height * (data.size() + 1));
		if (!ask && current_line < data.size())
		{
			input->k_enter.update_state();
			input->mouse_left.update_state();
			if (input->k_enter.single_hit || (option_box[current_line].within(input->mouse_x, input->mouse_y) && input->mouse_left.single_hit))
			{
				if (input->k_enter.single_hit)																		input->k_enter.single_hit = false;
				if (option_box[current_line].within(input->mouse_x, input->mouse_y) && input->mouse_left.single_hit)	input->mouse_left.single_hit = false;

				ask = true;
			}
		}
	}

	// Record deletion
	if (ask)
	{
		hitbox option_yes = hitbox(option_box[option_box.size() - 1].x, y + line_height * (data.size() + 2), 50, line_height);
		hitbox option_no = hitbox(option_box[option_box.size() - 1].x + 50, y + line_height * (data.size() + 2), 50, line_height);



		sys->Print("Are you sure?", x, y + line_height * (data.size() + 1));


		if (!confirm)
		{
			sys->Print("  Yes / > No", x, y + line_height * (data.size() + 2));


			SingleHit(input->k_enter, ask = false);
			SingleHit(input->k_left, confirm = true);

			if (option_no.within(input->mouse_x, input->mouse_y))
			{
				SingleHit(input->mouse_left, ask = false);
			}
			if (mouse_move && option_yes.within(input->mouse_x, input->mouse_y)) confirm = true;
		}
		if (confirm)
		{
			sys->Print("> Yes / No  ", x, y + line_height * (data.size() + 2));

			input->k_enter.update_state();
			input->mouse_left.update_state();
			if (input->k_enter.single_hit || (option_yes.within(input->mouse_x, input->mouse_y) && input->mouse_left.single_hit))
			{
				if (input->k_enter.single_hit)														input->k_enter.single_hit = false;
				if (option_yes.within(input->mouse_x, input->mouse_y) && input->mouse_left.single_hit) input->mouse_left.single_hit = false;

				ask = false;
				confirm = false;
				read = true;
				keep_list_offset = true;
				//viewing_scores = false;	// set it to false if there's a runtime error to test if it's caused by this block of code: scorescreen() won't be called after this instance is done

				data.clear();
				for (int i = 0; i < whole_data.size(); i++) data.push_back(whole_data[i]);

				for (int i = current_line + hidden_above; i < data.size() - 1; i++)
				{
					data[i] = data[i + 1];
				}
				data.pop_back();

				FILE* scores = nullptr;
				fopen_s(&scores, scores_file, "w");
				if (scores != nullptr)
				{
					for (int i = 0; i < data.size(); i++)
					{
						fputs(data[i].c_str(), scores);
						fputs("\n", scores);
					}
					fclose(scores);
				}
			}


			SingleHit(input->k_right, confirm = false);
			if (mouse_move && option_no.within(input->mouse_x, input->mouse_y) && confirm) confirm = false;
		}
	}

	sys->Print("Back", x, y + line_height * (data.size()));
	sys->Print(std::to_string(input->mouse_x) + ", " + std::to_string(input->mouse_y), x, y + line_height * (data.size() + 3));

	SingleHit(input->k_escape, reset_selection = true; viewing_scores = false; ask = false; confirm = false; keep_list_offset = false; read = true);
}

void Snake::sort_scores()
{
	std::vector<std::string> data;

	// Reading records from the file
	FILE* scores = nullptr;
	fopen_s(&scores, scores_file, "r");
	if (scores != nullptr)
	{
		data.clear();
		data.push_back("");

		char current_char;
		do
		{
			current_char = getc(scores);
			if (current_char == '\n') data.push_back("");
			else data.back() += current_char;
		} while (current_char != EOF);

		fclose(scores);
		data.pop_back();	// Remove the last empty line
	}


	// Reading column data
	std::vector<std::string> column_names, column_scores, column_times;
	for (int i = 0; i < data.size(); i++)
	{
		enum column_type { name = 0, score, time } current_column = name;
		column_names.push_back("");
		column_scores.push_back("");
		column_times.push_back("");

		for (int j = 0; j < data[i].size(); j++)
		{
			if (data[i][j] == '=')
			{
				j += 2;
				while (j < data[i].size())
				{
					if (data[i][j] != ',')
					{
						if (current_column == name)  column_names[column_names.size() - 1] += data[i][j];
						if (current_column == score) column_scores[column_scores.size() - 1] += data[i][j];
						if (current_column == time)  column_times[column_times.size() - 1] += data[i][j];
						j++;
					}
					else break;
				}

				if (current_column == name)  current_column = score;
				else if (current_column == score) current_column = time;
				else break;
			}
		}
	}

	std::vector<score_row> score_data;
	for (int i = 0; i < column_scores.size(); i++)
	{
		score_data.push_back(score_row());
		score_data.back().score = (unsigned int)std::stoi(column_scores[i], nullptr, 10);
		score_data.back().time = column_times[i];
		score_data.back().name = column_names[i];
	}


	FILE* clear = nullptr;
	fopen_s(&clear, scores_file, "w");
	if (clear != nullptr)
	{
		fclose(clear);
	}
	score_data = sort_descending(score_data);
	for (int i = 0; i < score_data.size(); i++)
	{
		add_score_row(score_data[i].name, score_data[i].score, score_data[i].time);
	}
}

void Snake::save_score(std::string name, unsigned int _score, unsigned int _time_passed)
{
	FILE* scores = nullptr;
	fopen_s(&scores, scores_file, "a");
	if (scores == nullptr) return;

	fputs("name = ", scores);
	fputs(name.c_str(), scores);
	fputs(", ", scores);

	fputs("score = ", scores);
	fputs(std::to_string(_score).c_str(), scores);
	fputs(", ", scores);

	fputs("time = ", scores);
	unsigned int minutes = _time_passed / 60;
	unsigned int seconds = _time_passed % 60;
	std::string time = std::to_string(minutes) + ":";
	if (seconds < 10) time += "0";
	time += std::to_string(seconds);
	fputs(time.c_str(), scores);

	putc('\n', scores);
	new_score = true;
	fclose(scores);


	sort_scores();
}

void Snake::add_score_row(std::string name, unsigned int _score, std::string _time_passed)
{
	FILE* scores = nullptr;
	fopen_s(&scores, scores_file, "a");
	if (scores == nullptr) return;

	fputs("name = ", scores);
	fputs(name.c_str(), scores);
	fputs(", ", scores);

	fputs("score = ", scores);
	fputs(std::to_string(_score).c_str(), scores);
	fputs(", ", scores);

	fputs("time = ", scores);
	fputs(_time_passed.c_str(), scores);

	putc('\n', scores);
	new_score = true;
	fclose(scores);
}




void Snake::deadscreen()
{
	unsigned int minutes_passed = time_passed / 60;
	unsigned int seconds_passed = time_passed % 60;

	std::string minutes; minutes.clear(); minutes = "";
	std::string seconds; seconds.clear(); seconds = "";

	if (seconds_passed < 10) seconds += "0";
	minutes += std::to_string(minutes_passed);
	seconds += std::to_string(seconds_passed);

	static unsigned int x = sys->uWindowWidth / 2 - 200 / 2;
	static unsigned int y = sys->uWindowHeight / 2 - line_height / 2;
	sys->Print("You scored: " + std::to_string(score) + " in " + minutes + ":" + seconds, x, y);

	static std::string name("");
	static bool display_cursor = false;
	static clock_t switch_time;
	static clock_t switch_last = clock();
	switch_time = clock();
	if (switch_time - switch_last >= 150)
	{
		switch_last = switch_time;
		display_cursor = !display_cursor;
	}
	if (!display_cursor)	sys->Print("Save your score as: " + name, x, y + line_height);
	else					sys->Print("Save your score as: " + name + "_", x, y + line_height);


	if (name.size() < 60)
	{
		for (int i = 0; i < alphabet_length; i++)
		{
			if (!input->k_shift) { HeldDown(input->k_alphabet[i], name += ((int)'a' + i)); }
			else { HeldDown(input->k_alphabet[i], name += ((int)'A' + i)); }
		}

		for (int i = 0; i < 10; i++)
		{
			HeldDown(input->k_numeric[i], name += ((int)'0' + i));
		}

		HeldDown(input->k_space, name += "_");
	}


	if (name.size() > 0)
	{
		if (!input->k_ctrl)
		{
			HeldDown(input->k_backspace, name.pop_back());
		}
		else
		{
			SingleHit(input->k_backspace, name = "");
		}
	}

	sys->Print("Press ENTER to continute.", x, y + line_height * 2);
	sys->Print("Press ESCAPE to ignore and leave.", x, y + line_height * 3);

	SingleHit(input->k_enter, clear_scene(); died = false; save_score(name, score, time_passed));
	SingleHit(input->k_escape, clear_scene(); died = false);
	SingleHit(input->mouse_left, clear_scene(); died = false; save_score(name, score, time_passed));
}

void Snake::menuscreen()
{
	enum menu_option
	{
		start_game = 0,
		scores,
		exit
	};
	static unsigned int current_option = start_game;

	static const unsigned int n_options = 3;
	static std::string options[n_options];
	for (int i = 0; i < n_options; i++)
	{
		options[i].clear();
		options[i] = "";

		if (i == current_option) options[i] += "> ";
	}

	options[start_game] += "Start game";
	options[scores] += "Scores";
	options[exit] += "Exit";

	std::string output; output.clear(); output = "";
	for (int i = 0; i < n_options; i++)
	{
		output += options[i];
		if (i != n_options - 1) output += "\n";

		options[i].clear();
	}

	static unsigned int x = sys->uWindowWidth / 2 - option_width / 2;
	static unsigned int y = sys->uWindowHeight / 2 - (line_height * (n_options + 1)) / 2;
	sys->Print(output, x, y);
	output.clear();

	sys->Print(std::to_string(input->mouse_x) + ", " + std::to_string(input->mouse_y), x, y + line_height * n_options);
	static unsigned int last_mouse_x = input->mouse_x;
	static unsigned int last_mouse_y = input->mouse_y;

	static bool mouse_move = false;
	if (input->mouse_x != last_mouse_x || input->mouse_y != last_mouse_y)
	{
		last_mouse_x = input->mouse_x;
		last_mouse_y = input->mouse_y;
		mouse_move = true;
	}
	else mouse_move = false;

	static hitbox option_box[n_options];
	for (int i = 0; i < n_options; i++)
	{
		option_box[i] = hitbox(x, y + i * line_height, option_width, line_height);


		if (option_box[i].within(input->mouse_x, input->mouse_y) && mouse_move) current_option = i;
		if (option_box[i].within(input->mouse_x, input->mouse_y)) sys->Print("opt_" + std::to_string(i + 1), x, y + (n_options + 1) * line_height);
	}


	HeldDown(input->k_up,	if (current_option != 0)			 current_option--; else current_option = n_options - 1);
	HeldDown(input->k_down, if (current_option != n_options - 1) current_option++; else current_option = 0);



	input->mouse_left.update_state();
	input->k_enter.update_state();
	if (input->mouse_left.single_hit || input->k_enter.single_hit)
	{
		input->mouse_left.single_hit = false;
		input->k_enter.single_hit = false;


		switch (current_option)
		{
			case start_game:
				playing = true;
				break;
			case scores:
				viewing_scores = true;
				break;
			case exit:
				leave = true;
				break;
		}
	}


	SingleHit(input->k_escape, break_main = true);
}

void Snake::leavescreen()
{
	static bool default_value = false;
	static bool confirm = default_value;

	static unsigned int x = sys->uWindowWidth / 2 - option_width / 2 - 15;
	static unsigned int y = sys->uWindowHeight / 2 - (line_height * 2) / 2 + line_height * 1;

	sys->Print("Leave?", x + 15, y - line_height);
	if (confirm) sys->Print("> Yes / No  ", x, y);
	else		 sys->Print("  Yes / No <", x, y);

	sys->Print(std::to_string(input->mouse_x) + ", " + std::to_string(input->mouse_y), x + 15, y + line_height);



	if (confirm)
	{
		SingleHit(input->k_right, confirm = false);
		SingleHit(input->k_enter, break_main = true);

		SingleHit(input->mouse_left, break_main = true);
	}
	else
	{
		SingleHit(input->k_left, confirm = true);
		SingleHit(input->k_enter, leave = false; confirm = default_value);

		SingleHit(input->mouse_left, leave = false; confirm = default_value);
	}

	static hitbox option_yes(x, y, option_width / 2, line_height);
	static hitbox option_no(x + option_width / 2, y, option_width / 2, line_height);


	static unsigned int last_mouse_x = input->mouse_x;
	static unsigned int last_mouse_y = input->mouse_y;
	static bool mouse_move = false;
	if (input->mouse_x != last_mouse_x || input->mouse_y != last_mouse_y)
	{
		last_mouse_x = input->mouse_x;
		last_mouse_y = input->mouse_y;
		mouse_move = true;
	}
	else mouse_move = false;

	if (option_yes.within(input->mouse_x, input->mouse_y) && mouse_move) confirm = true;
	if (option_no.within(input->mouse_x, input->mouse_y) && mouse_move) confirm = false;

	if (option_yes.within(input->mouse_x, input->mouse_y)) sys->Print("opt_1", x - 15, y + line_height * 2);
	if (option_no.within(input->mouse_x, input->mouse_y)) sys->Print("opt_2", x - 15, y + line_height * 2);
}

int Snake::GetX()
{
	return head.GetX();
}

int Snake::GetY()
{
	return head.GetY();
}


int Snake::GetTargetX()
{
	return target.GetX();
}

int Snake::GetTargetY()
{
	return target.GetY();
}



void Snake::add_snake_piece()
{
	snake_model->AddObject(tile_width, tile_height);

	snake_length++;
}

void Snake::move_snake(int x, int y)
{
	for (int i = snake_length - 1; i > 0; i--)
	{
		snake_model->shapes[i].NextPos(snake_model->shapes[i - 1].GetX(), snake_model->shapes[i - 1].GetY());
	}
	head.NextPos(x, y);
}

void Snake::move_whole_snake(int x, int y)
{
	for (int i = 0; i < snake_length; i++)
	{
		snake_model->shapes[i].NextPos(x, y);
	}
}

void Snake::init_scene()
{
	reset_values();


	scene = new Scene(sys, break_main);
	scene->InitScene();
	snake_model = new Scene(sys, break_main);
	snake_model->InitScene();

	for (int i = 0; i < 10; i++) add_snake_piece();
	move_whole_snake(origin_x, origin_y + VELOCITY * 10);

	while (head.GetY() != origin_y)
	{
		move_snake(head.GetNextX(), head.GetNextY() - VELOCITY);
		for (int j = 0; j < snake_length; j++) snake_model->shapes[j].UpdatePos();
	}

	scene->AddObject(tile_width, tile_height);
	target_index = scene->size - 1;
	set_target();

	output = true;
}

void Snake::update_scene()
{
	framerate->update_framerate();
	time_passed = framerate->count_seconds - time_start;

	double f_offset = VELOCITY * framerate->time_factor;
	snake_offset = round(f_offset);

	static unsigned int current_second = 0;
	if (framerate->count_seconds != current_second)
	{
		total_offset_in_last_second = sum_offset;
		sum_offset = 0;
		current_second = framerate->count_seconds;
	}

	SingleHit(input->k_space, stop_snake = !stop_snake);

	static clock_t snake_time;
	static clock_t snake_last = clock();
	snake_time = clock();
	if (snake_time - snake_last >= 1 && !stop_snake)
	{
		snake_last = snake_time;

		sum_offset += snake_offset;
		switch (snake_direction)
		{
			case up:
				move_snake(head.GetNextX(), head.GetNextY() - snake_offset);
				break;
			case down:
				move_snake(head.GetNextX(), head.GetNextY() + snake_offset);
				break;
			case left:
				move_snake(head.GetNextX() - snake_offset, head.GetNextY());
				break;
			case right:
				move_snake(head.GetNextX() + snake_offset, head.GetNextY());
				break;
		}

		if (head.GetNextX() < 0 || head.GetNextY() < 0 ||
			head.GetNextX() + head.GetWidth() >= map_width || head.GetNextY() + head.GetHeight() >= map_height)
		{
			next_direction = up;

			move_whole_snake(origin_x, origin_y + VELOCITY * 10);

			while (head.GetY() != origin_y)
			{
				move_snake(head.GetNextX(), head.GetNextY() - VELOCITY);
				for (int j = 0; j < snake_length; j++) snake_model->shapes[j].UpdatePos();
			}

			tries_left--;
			if (tries_left == 0)
			{
				died = true;
				playing = false;
			}
		}


		if (shapes_collide(target, head))
		{
			score++;
			if (score % 10 == 0) tries_left++;
			set_target();

			if (score % 5 == 0)
			{
				radius_span += 1;
				angle_offset_span += 1;
			}
		}
	}
	set_direction();


	update_target();
}

void Snake::update_target()
{
	angle_offset = (M_PI / 2) * ((double)random_integer((-1) * angle_offset_span, angle_offset_span) / (double)100);
	radius = random_integer(radius - radius_span, radius + radius_span);

	angle += angle_offset;
	if (angle >= M_PI * 2) angle = 0;

	int temp_x = target_x + sin(angle) * radius;
	int temp_y = target_y - cos(angle) * radius;
	target.NextPos(temp_x, temp_y);
}

void Snake::set_target()
{
	radius = random_integer(0, 220);

	target_x = random_integer(0 + radius + radius_span, map_width - tile_width - radius - radius_span);
	target_y = random_integer(0 + radius + radius_span, map_height - tile_height - radius - radius_span);

	target.NextPos(target_x, target_y);
}

void Snake::render_scene()
{
	// draw on the backbuffer
	scene->DrawScene();
	snake_model->DrawScene();

	if (stop_snake)
	{
		sys->Print("Press SPACE to move", sys->uWindowWidth / 2 - 138 / 2, sys->uWindowHeight / 2 - line_height / 2 - line_height * 2);
	}
}

void Snake::set_direction()
{
	if (input->k_up && snake_direction != down) next_direction = up;
	if (input->k_down && snake_direction != up) next_direction = down;
	if (input->k_left && snake_direction != right) next_direction = left;
	if (input->k_right && snake_direction != left) next_direction = right;
	snake_direction = next_direction;
}

void Snake::clear_scene()
{
	output = false;
	scene->ReleaseScene();
	snake_model->ReleaseScene();

	delete scene;
	delete snake_model;
}

bool Snake::shapes_collide(Shape shape_1, Shape shape_2)
{
	if (within_shape(shape_2.GetX() /*        */, shape_2.GetY() /*         */, shape_1) ||
		within_shape(shape_2.GetX() + tile_width, shape_2.GetY() /*         */, shape_1) ||
		within_shape(shape_2.GetX() /*        */, shape_2.GetY() + tile_height, shape_1) ||
		within_shape(shape_2.GetX() + tile_width, shape_2.GetY() + tile_height, shape_1))
	{
		return true;
	}
	else
	{
		return false;
	}
}

