#include "stdafx.h"
#include "Ray.h"

#ifndef SHAPE
#define SHAPE

class Shape
{
public:
	int type;
	virtual IntersectResult intersect(Ray ray) 
	{
		IntersectResult result;
		return result;
	};
};

#endif