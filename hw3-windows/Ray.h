#include "stdafx.h"

#ifndef RAY
#define RAY

class Ray
{
public:
	Ray(vec3 origin)
	{
		this->origin = origin;
		t = 0;
	}
	vec3 origin;
	vec3 direction;
	float t;
};

#endif