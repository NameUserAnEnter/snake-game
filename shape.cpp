#include "shape.h"


void Shape::Update()
{
	if (bytes == nullptr) return;

	sys->SetOnBackBuffer(next_x, next_y, width, height, bytes);
	
	x = next_x;
	y = next_y;
}


bool Shape::operator!=(Shape operand)
{
	if (initialized == false && operand.initialized == false) return false; // op1 == op2
	else if (initialized == true && operand.initialized == true)
	{
		if (operand.GetWidth() == width && operand.GetHeight() == height)
		{
			if (bytes != nullptr && operand.bytes != nullptr)
			{
				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < width; j++)
					{
						if (bytes[i * width + j] != operand.bytes[i * width + j]) return true;	// op1 != op2, bytes don't match
					}
				}

				return false;	// op1 == op2
			}
			else return true;	// op1 != op2, dimensions don't match
		}
	}
	else return true;	// op1 != op2, one of the operands is not initialized while the other is initialized
}

void Shape::UpdatePos()
{
	x = next_x;
	y = next_y;
}

void Shape::NextPos(int _x, int _y)
{
	next_x = _x;
	next_y = _y;
}




 bool point_inbetween(int low_end, int the_point, int high_end)
{
	if(the_point >= low_end && the_point <= high_end) return true;
	else return false;
}

 bool within_shape(int x, int y, int shape_x, int shape_y, unsigned int width, unsigned int height)
{
	if (x >= shape_x && x < shape_x + width && y >= shape_y && y < shape_y + height) return true;
	else return false;
}

 bool within_shape(int x, int y, Shape shape)
{
	return within_shape(x, y, shape.GetX(), shape.GetY(), shape.GetWidth(), shape.GetHeight());
}



 unsigned int Shape::GetWidth()
{
	return width;
}

 unsigned int Shape::GetHeight()
{
	return height;
}

 int Shape::GetNextX()
{
	return next_x;
}

 int Shape::GetNextY()
{
	return next_y;
}

 int Shape::GetX()
{
	return x;
}

 int Shape::GetY()
{
	return y;
}

 Shape::Shape(_sys* _sys_)
{
	bytes = nullptr;

	sys = _sys_;

	width = 0;
	height = 0;

	x = 0;
	y = 0;

	next_x = 0;
	next_y = 0;

	initialized = false;
}

 void Shape::InitShape(unsigned int _width, unsigned int _height)
{
	initialized = true;

	width = _width;
	height = _height;

	x = 0 - width;
	y = 0 - height;

	size_t byte_count = (size_t)(height * width);
	bytes = (color*)calloc(byte_count, sizeof(color));
	if (bytes != nullptr)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				bytes[i * width + j].R = 255;
				bytes[i * width + j].G = 0;
				bytes[i * width + j].B = 255;
				bytes[i * width + j].A = 255;
			}
		}
	}
}

 void Shape::ReleaseShape()
{
	if (initialized)
	{
		free(bytes);
	}
}

 Shape::~Shape()
{
}


