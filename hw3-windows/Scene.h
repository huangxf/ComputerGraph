#include "stdafx.h"
#include <string>

#ifndef SCENE
#define SCENE

class Scene
{
public:
	vec3 eye;
	vec3 corners[4];
	int width; //ouput width
	int height; //output height
	int maxDepth;
	std::string output_filename;

};

#endif