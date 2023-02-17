#include "framerate.h"



void _framerate::update_framerate()
{
	previous_frame = current_frame;

	// CLOCKS_PER_SEC = 1000
	current_frame = clock();
	if (current_frame - previous_second >= 1000)
	{
		previous_second = current_frame;
		count_seconds++;

		previous_second_frame_count = current_second_frame_count;
		current_second_frame_count = 0;
	}

	current_second_frame_count++;

	// assessing expected framerate from a single interval
	clock_t current_interval = current_frame - previous_frame;
	time_factor = current_interval / (double)(1000 / reference_framerate);	// 1000 / 30 = 33.33 ms
}
