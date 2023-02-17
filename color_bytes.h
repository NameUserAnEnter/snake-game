#ifndef COLOR_BYTES_H
#define COLOR_BYTES_H


struct solid;
struct color;

struct solid
{
	unsigned char R, G, B;

	bool operator!=(solid comp);


	solid();
	solid(unsigned char _R, unsigned char _G, unsigned char _B);
	solid(unsigned long source);
	solid(color source);
};



struct color
{
	unsigned char R, G, B, A;

	bool operator!=(color comp);
	bool operator!=(solid comp);


	color();
	color(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char _A);
	color(unsigned long source);


	color(solid source);
};


#endif
