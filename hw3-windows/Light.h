#include "stdafx.h"

#ifndef LIGHT
#define LIGHT

class Light
{
public:
	Light() {}
	Light(int type, vec3 light_v, vec3 rgb)
	{
		this->type = type; //point light
		this->light_v = light_v;
		this->rgb = rgb;
		this->attenuation = vec3(1, 0, 0);    //by default
	}

	int type;
	vec3 light_v;
	vec3 rgb;
	vec3 attenuation;
};

#endif 