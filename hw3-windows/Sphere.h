#include "stdafx.h"
#include "Ray.h"
#include "IntercectResult.h"
#include "Shape.h"
#include <string>



#ifndef SPHERE
#define SPHERE

class Sphere
{
public:
	Sphere() {}

	Sphere(vec3 origin, float radius)
	{
		this->origin = origin;
		this->radius = radius;
		this->type = 0;
		this->trans = mat4(1,0,0,0,
							   0,1,0,0,
							   0,0,1,0,
							   0,0,0,1 );
	}

	Sphere(vec3 origin, vec3 v2, vec3 v3)
	{
		this->origin = origin;
		this->v2 = v2;
		this->v3 = v3;
		this->radius = 0;
		this->type = 1;
		this->trans = mat4(1,0,0,0,
							   0,1,0,0,
							   0,0,1,0,
							   0,0,0,1 );
	}

	IntersectResult intersect(Ray ray);

	void transform(); 
	
	int type;
	vec3 origin;
	vec3 v2;
	vec3 v3;
	vec3 noraml;
	float radius;
	
	std::string name;

	float ambient[4] ; 
	float diffuse[4] ; 
	float specular[4] ;
	float emission[4] ; 
	float shininess ;
	mat4 trans ; 
	mat4 inverse_trans;
};

#endif
