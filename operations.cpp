#include "operations.h"



 double power(double base, unsigned int exponent)
{
	if (base == 0) return 0;

	double return_value = 1;
	for (int i = 0; i < exponent; i++)
	{
		return_value *= base;
	}

	return return_value;
}

 double absolute_value(double x)
{
	if (x >= 0) return x;
	else return x * (-1);
}

 int random_integer(int low_end, int high_end)
{
	int range = absolute_value(high_end - low_end) + 1;
	return rand() % range + low_end;
}

 unsigned int count_digits(unsigned int num)
{
	if (num == 0) return 1;
	
	unsigned int digits = 0;
	while (power(10, digits) <= num) digits++;
	return digits;
}

 wchar_t* wstring(std::string str)
{
	wchar_t* return_value = (wchar_t*)calloc(str.size() + 1, sizeof(wchar_t));
	if (return_value == nullptr) return return_value;

	for (int i = 0; i < str.size(); i++)
	{
		return_value[i] = (wchar_t)str[i];
	}
	return_value[str.size()] = L'\0';
	return return_value;
}


 void DisplayMessage(std::string message)
{
	MessageBox(NULL, wstring(message), wstring(""), MB_OK);
}

 void DisplayMessage(std::string message, std::string title)
{
	MessageBox(NULL, wstring(message), wstring(title), MB_OK);
}

