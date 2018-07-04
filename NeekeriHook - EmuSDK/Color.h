#pragma once
#include "SDK.h"

/* poop color class, pls change */
class Color
{
public:
	int _RGBA[4];

	Color(int R, int G, int B, int A)
	{
		_RGBA[0] = R;
		_RGBA[1] = G;
		_RGBA[2] = B;
		_RGBA[3] = A;
	}
};