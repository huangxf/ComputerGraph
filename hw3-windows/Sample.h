#include "stdafx.h"

#ifndef SAMPLE
#define SAMPLE

class Sample
{
public:
	Sample()
	{
		rgba = vec4(0, 0, 0, 1); //default background color is black
	}

	Sample(int r, int g, int b, int a)
	{
		vec4 rgba(r, g, b, a);
		this->rgba = rgba;
	}

	int x,y; //pixel position
	vec4 rgba; //color
};

#endif