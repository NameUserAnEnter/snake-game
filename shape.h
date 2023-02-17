#ifndef SHAPE_H
#define SHAPE_H

#include <stdlib.h>
#include "functions.h"

class Shape
{
private:
	int x;
	int y;
	unsigned int width;
	unsigned int height;

	int next_x;
	int next_y;


	bool initialized;
public:
	_sys* sys = sys;

	color* bytes;

	int GetX();
	int GetY();

	unsigned int GetWidth();
	unsigned int GetHeight();

	int GetNextX();
	int GetNextY();

	Shape(_sys*);
	void InitShape(unsigned int _width, unsigned int _height);
	void ReleaseShape();

	~Shape();
	


	void NextPos(int _x, int _y);
	void UpdatePos();

	void Update();

	bool operator!=(Shape operand);
};



bool point_inbetween(int low_end, int the_point, int high_end);
bool within_shape(int x, int y, int shape_x, int shape_y, unsigned int width, unsigned int height);
bool within_shape(int x, int y, Shape shape);

#endif
