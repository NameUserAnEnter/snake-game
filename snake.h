#ifndef SNAKE_H
#define SNAKE_H

#include "scene.h"
#include <vector>
#include "definitions.h"



enum direction { up, down, left, right };

#define VELOCITY	20
#define origin_x	sys->uWindowWidth / 2 - tile_width / 2
#define origin_y	sys->uWindowHeight / 2 - tile_height / 2

#define target		scene->shapes[target_index]

#define head		snake_model->shapes[0]

class Snake
{
public:
	_sys* sys;
	_input* input;
	_framerate* framerate;
	bool& break_main;
public:
	void Update();
private:
	void init_scene();
	void render_scene();
	void update_scene();
	void clear_scene();

private:

	void add_snake_piece();
	void move_snake(int x, int y);
	void move_whole_snake(int x, int y);
	void set_direction();

private:

	direction		snake_direction;
	direction		next_direction;

	Scene*			scene;
	Scene*			snake_model;

	unsigned int	tile_width, tile_height;
	unsigned int	map_width, map_height;

	unsigned int	snake_length;
public:
	double			snake_offset;
	unsigned int	tries_left;
	unsigned int	score;

	unsigned int	total_offset_in_last_second;
private:
	double			sum_offset;
private:
	int				target_x, target_y;
	unsigned int	target_index;

public:

	int GetX();
	int GetY();

	int GetTargetX();
	int GetTargetY();

	Snake(_sys*, _input*, _framerate*, bool&);

private:
	void set_target();
	void update_target();
	bool shapes_collide(Shape shape_1, Shape shape_2);

public:
	double radius;
	double angle;
	double angle_offset;
public:
	bool	playing;
private:
	void	reset_values();

	bool	stop_snake;
	bool	died;
	bool	leave;
	bool	viewing_scores;
public:
	unsigned int time_passed;
private:
	unsigned int time_start;

private:
	void menuscreen();
	void deadscreen();
	void leavescreen();
	void scorescreen();

	void save_score(std::string name, unsigned int _score, unsigned int _time_passed);
	void add_score_row(std::string _name, unsigned int _score, std::string _time_passed);
	void sort_scores();
private:
	unsigned int angle_offset_span;
	unsigned int radius_span;

	unsigned int option_width;
	unsigned int line_height;
	bool	new_score;
public:
	bool	output;
};


#endif
