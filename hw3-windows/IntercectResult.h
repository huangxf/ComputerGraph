#include "stdafx.h"



#ifndef INTERSECTRESULT
#define INTERSECTRESULT

class Sphere;

class IntersectResult
{
public:
	IntersectResult()
	{
		distance = 0;
		shape = 0;
	}
	Sphere * shape;
	float distance;
	vec3 intersectionPoint;
	vec3 normal;
};

#endif