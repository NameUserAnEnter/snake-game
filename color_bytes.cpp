#include "color_bytes.h"

solid::solid()
{
	R = 255;
	G = 255;
	B = 255;
}

solid::solid(unsigned char _R, unsigned char _G, unsigned char _B)
{
	R = _R;
	G = _G;
	B = _B;
}

solid::solid(unsigned long source)
{
	R = (source & 0x000000FF) >> 0;
	G = (source & 0x0000FF00) >> 8;
	B = (source & 0x00FF0000) >> 16;
}

solid::solid(color source)
{
	R = source.R;
	G = source.G;
	B = source.B;
}

bool solid::operator!=(solid comp)
{
	if (R != comp.R || G != comp.G || B != comp.B) return true;
	else return false;
}




color::color()
{
	R = 255;
	G = 255;
	B = 255;
	A = 255;
}

color::color(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char _A)
{
	R = _R; G = _G; B = _B; A = _A;
}

color::color(unsigned long source)
{
	R = (source & 0x000000FF) >> 0;
	G = (source & 0x0000FF00) >> 8;
	B = (source & 0x00FF0000) >> 16;
	A = 255;
}

color::color(solid source)
{
	R = source.R;
	G = source.G;
	B = source.B;
	A = 255;
}

bool color::operator!=(color comp)
{
	if (R != comp.R || G != comp.G || B != comp.B || A != comp.A)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool color::operator!=(solid comp)
{
	if (solid(R, G, B) != solid(comp)) return true;
	else return false;
}

