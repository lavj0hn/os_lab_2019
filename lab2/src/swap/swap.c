#include "swap.h"

void Swap(char *left, char *right)
{
	char change;
	change = *left; 
	* left = * right;
	* right = change;
}
