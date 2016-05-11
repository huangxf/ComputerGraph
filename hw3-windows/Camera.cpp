#include "stdafx.h"
#include "Camera.h"

Ray Camera::generateRay(float sx, float sy, float aspectRatio)
{
	//appectRation是解决分辨率和图像比例关系的关键
	float scale_r = 2 * (sx - 0.5) * (tan(fov * 0.5 * pi / 180) * aspectRatio);
	float scale_u = 2 * (0.5 - sy) * tan(fov * 0.5 * pi / 180);
	vec3 r = right * scale_r;
	vec3 u = up * scale_u;
	Ray ray(eye);
	vec3 vec_un =  front + r + u;
	ray.direction = glm::normalize(vec_un);
    return ray;
}