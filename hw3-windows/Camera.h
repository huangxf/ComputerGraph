#include "stdafx.h"
#include "Ray.h"

#ifndef CAMERA
#define CAMERA

class Camera
{
public:
	Camera() {};

	Camera(vec3 eye, vec3 lookat, vec3 up, float fov)
	{
		this->eye = eye;
		this->lookat = lookat;
		this->front = glm::normalize(lookat - eye);
		this->right = glm::normalize(glm::cross(front, up));
		this->up = glm::normalize(glm::cross(right, front));
		this->fov = fov;
	}
	
	Ray generateRay(float sx, float sy, float aspectRatio);

	vec3 up;
	vec3 front;
	vec3 right;
	vec3 eye;
	vec3 lookat;
	float fov;
};

#endif