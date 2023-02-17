#ifndef FRAMERATE_H
#define FRAMERATE_H

#include <time.h>

struct _framerate
{
	unsigned int count_seconds = 0;
	unsigned int previous_second_frame_count = 0;
	unsigned int current_second_frame_count = 0;

	clock_t previous_second = clock();

	clock_t previous_frame;
	clock_t current_frame = clock();

	double time_factor = 1.0;
	const int reference_framerate = 60;


	void update_framerate();

	void operator=(_framerate operand)
	{
		count_seconds = operand.count_seconds;
		previous_second_frame_count = operand.previous_second_frame_count;
		current_second_frame_count = operand.current_second_frame_count;

		previous_second = operand.previous_second;
		
		previous_frame = operand.previous_frame;
		current_frame = operand.current_frame;

		time_factor = operand.time_factor;
	}
};

#endif
