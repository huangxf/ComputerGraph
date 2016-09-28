#include "stdafx.h"
#include "Sphere.h"
#include <iostream>

float clamp(float val)
{
	if(abs(val) <= 0.00000000000001) return 0.0f;
	else return val;
}

IntersectResult Sphere::intersect(Ray ray)
{
	IntersectResult result;
	if(this->type == 0) {
		//vec3 v = ray.origin - this->origin;
		//float a = glm::dot(v, v) - pow(this->radius, 2);
		//float DdotV = glm::dot(ray.direction, v);
		//float discr = DdotV * DdotV - a;
		//if(discr >= 0)
		//{
		//	result.shape = this;
		//	if(DdotV <= 0) {
		//		result.distance = -DdotV - sqrt(discr);
		//	} else {
		//		result.distance = 0;
		//	}
		//	result.intersectionPoint = ray.origin + ray.direction * result.distance;
		//	result.normal = glm::normalize(result.intersectionPoint - this->origin);

		vec3 L = ray.origin - this->origin;
		float A = glm::dot(ray.direction, ray.direction);
		float B = 2 * glm::dot(ray.direction, L);
		float C = glm::dot(L, L) - this->radius * this->radius;
		float discr = clamp(B*B - 4 * A * C);
		if(discr >= 0)
		{
			float dist = clamp((-B - sqrt(discr))/(2 * A));
			if(dist < 0) {dist = clamp((-B + sqrt(discr))/(2 * A));}
			result.shape = this;
			result.distance = dist;
			result.intersectionPoint = ray.origin + ray.direction * result.distance;
			result.normal = glm::normalize(result.intersectionPoint - this->origin);

			//vec3 temp = result.intersectionPoint - this->origin;
			//float radius = sqrt(glm::dot(temp,temp));
			//std::cout<<"radius:"<<radius<<std::endl;
		}
	} else if(this->type == 1) {
			float u,v;
			vec3 edge1 = v2 - origin; 
			vec3 edge2 = v3 - origin; 

			vec3 pvec = glm::cross(ray.direction, edge2); 
			float det = glm::dot(edge1, pvec); 
			if (det == 0 || det < 0) return result; 
 
			vec3 tvec = ray.origin - origin; 
			u = glm::dot(tvec, pvec); 
			if (u < 0 || u > det) return result; 
 
			vec3 qvec = glm::cross(tvec, edge1); 
			v = glm::dot(ray.direction, qvec); 
			if (v < 0 || u + v > det) return result; 
 
			float invDet = 1 / det; 
 
			result.distance = glm::dot(edge2, qvec) * invDet; 
			u *= invDet; 
			v *= invDet; 
			result.intersectionPoint = ray.origin + ray.direction * result.distance;
			result.normal = glm::normalize(glm::cross(edge1, edge2));

			result.shape = this;
 
			return result; 

	}
	return result;
}

void Sphere::transform() {
		vec4 temp = vec4(origin, 1);
		vec4 transform = trans * temp;
		origin = (vec3)transform;
		
		temp = vec4(v2, 1);
		transform = trans * temp;
		v2 = (vec3)transform;

		temp = vec4(v3, 1);
		transform = trans * temp;
		v3 = (vec3)transform;
	}