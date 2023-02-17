#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include <Windows.h>
#define M_PI 3.14159



 double power(double, unsigned int);

 double absolute_value(double);

 int random_integer(int, int);

 unsigned int count_digits(unsigned int);

 wchar_t* wstring(std::string);


 void DisplayMessage(std::string);

 void DisplayMessage(std::string, std::string);

#endif
