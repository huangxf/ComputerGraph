#include "stdafx.h"
#include "Film.h"
#include "FreeImage.h"
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;

void Film::output(const char * path)
{
	std::string filename(path);
	if(filename == "") filename = "output.png";

	BYTE * pixels = new BYTE[3 * sizeof(BYTE) * scene.width * scene.height];
	for(int i = 0; i < scene.width * scene.height; i++)
	{
		pixels[i*3] = (BYTE)samples[i].rgba.b;
		pixels[i*3 + 1] = (BYTE)samples[i].rgba.g;
		pixels[i*3 + 2] = (BYTE)samples[i].rgba.r;
	}
	FreeImage_Initialise();
	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, scene.width, scene.height, scene.width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	FreeImage_Save(FIF_PNG, img, filename.c_str(), 0);
	FreeImage_DeInitialise();
	if(!pixels) delete[] pixels;
	return;
}

void Film::render()
{
	for(int j = 0; j < scene.height; j++)
	{
		float sy = 1 - (j + 0.5) * 1.0f / scene.height;
		for(int i = 0; i < scene.width; i++)
		{
			float sx = (i + 0.5) * 1.0f / scene.width;

			Ray ray = camera.generateRay(sx, sy, this->ratio);
			//vec4 v = vec4(-sqrt(0.5f), 0, sqrt(0.5f), 1);
			//mat4 matrix = objects.at(0).trans;
			//v = matrix * v;
			//Ray ray(vec3(0,-4,4));
			//ray.direction = glm::normalize(vec3(v.x, v.y, v.z)-vec3(0,-4,4));
			IntersectResult result = trace(ray);
			//cout<<result.distance<<endl;

			if(result.distance > 0) {
				//如果是球体，则要重新计算法线，三角则不用
				//if(result.shape->type == 0) //sphere, recaculate normal
				//{
					//vec3 radiusVec = result.intersectionPoint - vec3(result.shape->trans * vec4(result.shape->origin, 1));
					//result.shape->origin = vec3(result.shape->trans * vec4(result.shape->origin, 1));
					//vec3 normal_transferd =  vec3(glm::transpose(glm::inverse(result.shape->trans)) * vec4(result.normal, 0));
					//result.normal = glm::normalize(normal_transferd);

					//float radius = sqrt(glm::dot(radiusVec, radiusVec));
					//cout<<"["<<normal_transferd.x<<","<<normal_transferd.y<<","<<normal_transferd.z<<"]-->";
					//cout<<"["<<result.normal.x<<","<<result.normal.y<<","<<result.normal.z<<"]"<<endl;
				//}
				vec4 color = getColor(ray, result, 0); 
				Sample sample(
					255 * color.x , 
					255 * color.y, 
					255 * color.z, 
					1);
				samples[j * scene.width + i] =  sample;
			}
			//std::cout<<"Process:"<<(j * scene.width + i)<<"/"<<(scene.width*scene.height)<<std::endl;
		}
	}
}

void Film::preprocess()
{
	for(vector<Sphere>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		(*it).inverse_trans = glm::inverse((*it).trans);
	}
}

IntersectResult Film::trace(const Ray &ray)
{
		float min = 100000.00f;
		IntersectResult result;
		for(vector<Sphere>::iterator it = objects.begin(); it != objects.end(); it++)
		{
			//物体保持不变，但是将光从World space转换到物体的local space, 然后作interstect测试。 考虑到在world space下，从局部变换过来的物体如果被ray击中
			//那么这个过程等价于在local space下，原始物体被从world space转换到local space的ray击中。
			Ray ray_inverse = ray;
			mat4 trans_inverse = glm::inverse((*it).trans);
			ray_inverse.direction = glm::normalize(vec3(trans_inverse * vec4(ray.direction, 0))); //注意扩展direction的时候必须用homogeneous coordinates，否则direction就不是方向向量了
			ray_inverse.origin = vec3(trans_inverse * vec4(ray.origin, 1));
			IntersectResult point = (*it).intersect(ray_inverse);
			if(point.distance > 0)
			{
				//由于ray在做intersection的时候变换到了局部坐标，因此判断之后还要再变回世界坐标
				//相应的交点，法线，距离等都要重新计算
				//注意三角形的法线在变换前后并不是不变的，所以也必须重新计算
				point.intersectionPoint = vec3(point.shape->trans * vec4(point.intersectionPoint, 1));
				vec3 origin_to_intersect =  point.intersectionPoint - ray.origin;
				point.distance = sqrt(glm::dot(origin_to_intersect, origin_to_intersect));
				if(point.distance < min)
				{
					min = point.distance;
					vec3 normal_transferd =  vec3(glm::transpose(glm::inverse(point.shape->trans)) * vec4(point.normal, 0));
					point.normal = glm::normalize(normal_transferd);
					result = point;
				}
			}
		}
		return result;
}

vec4 Film::getColor(const Ray &ray, const IntersectResult &result, int depth)
{
	if(depth > maxDepth) return vec4(0, 0, 0, 1);
	vec4 finalColor;

	vec3 point = result.intersectionPoint; //intersectionPoint

	vec3 eyedirn = glm::normalize(camera.eye - point) ; 
	vec3 _normal = glm::normalize(result.normal);

	vec4 ambient(
		result.shape->ambient[0],
		result.shape->ambient[1], 
		result.shape->ambient[2],
		result.shape->ambient[3] );

	vec4 emission(
		result.shape->emission[0],
		result.shape->emission[1],
		result.shape->emission[2],
		result.shape->emission[3] );

	vec4 diffuse(
		result.shape->diffuse[0],
		result.shape->diffuse[1],
		result.shape->diffuse[2],
		result.shape->diffuse[3] );

	vec4 specular(
		result.shape->specular[0],
		result.shape->specular[1],
		result.shape->specular[2],
		result.shape->specular[3] );



	float shininess = result.shape->shininess;

	finalColor = ambient + emission;


	const static float EPSILON = 0.00001f;

	if(glm::dot(ray.direction,result.normal) < 0)
			point = point + EPSILON * result.normal;
	else
			point = point - EPSILON * result.normal;
	
	for(vector<Light>::iterator it = lights.begin(); it !=  lights.end(); it++)
	{
		int vi = 0; //directional light是不会参加阴影计算的，所以这里必须是初始化为vi=0
		int type = (*it).type;

		vec3 direction0;  
		float light_dis_square = 0.0f;
		if(type == 0)  direction0	= glm::normalize ((*it).light_v) ;
		else {
			vec3 point_to_light = vec3((*it).light_v - point);
			direction0 = glm::normalize(point_to_light);
			Ray shadow_ray(point);
			shadow_ray.direction = glm::normalize(direction0);
			light_dis_square = glm::dot(point_to_light, point_to_light);
			IntersectResult shadow_result = trace(shadow_ray);
			if(shadow_result.distance > 0 && (shadow_result.distance *  shadow_result.distance) < light_dis_square) vi = 0;
			else vi = 1;
			//vi = 1;
		}

		vec3 half0 = glm::normalize (direction0 + eyedirn) ;
		vec3 attenuation = (*it).attenuation;
		vec4 col0 = computeLight(direction0, vec4((*it).rgb, 1), _normal, half0, diffuse, specular, shininess, vi, attenuation, light_dis_square) ;
		finalColor += col0;
		//if(vi == 0) cout<<finalColor.x<<","<<finalColor.y<<","<<finalColor.z<<point.x<<","<<point.y<<","<<point.z<<endl;
	}

	
	vec3 refDir = reflectDir(ray, result);
	Ray refRay(point);
	refRay.direction = refDir;
	IntersectResult ir = trace(refRay);
	if(ir.distance > 0) {
		//depth++;
		vec4 nextLevelColor = (specular * getColor(refRay, ir, ++depth));
		//if(depth == 1)  cout<<"["<<finalColor.x<<","<<finalColor.y<<","<<finalColor.z<<"]-->";
		finalColor += nextLevelColor;
		//if(depth == 1)  cout<<"["<<finalColor.x<<","<<finalColor.y<<","<<finalColor.z<<"]""+"<<endl;
	}
	return finalColor;
}

vec3 Film::reflectDir(const Ray &injectRay, const IntersectResult& reflectPoint)
{
	vec3 direction = glm::normalize(injectRay.direction);
	vec3 normal = glm::normalize(reflectPoint.normal);
	vec3 reflect = direction - 2 * glm::dot(direction, normal) * normal;
	return glm::normalize(reflect);
}

vec4 Film::computeLight(
		const vec3 &direction, 
		const vec4 &lightcolor, 
		const vec3 &normal, 
		const vec3 &halfvec, 
		const vec4 &mydiffuse, 
		const vec4 &myspecular, 
		const float &myshininess,
		const int &vi,
		const vec3 &attenuation,
		const float &light_dis_square)
{
	float fraction = attenuation[0] + attenuation[1]*sqrt(light_dis_square) + attenuation[2] * light_dis_square;
 	float nDotL = glm::dot(normal, direction)  ;         
	vec4 lambert = vi * mydiffuse * (lightcolor / fraction)  * std::max(nDotL, 0.0f) ;  

	float nDotH = glm::dot(normal, halfvec) ; 
	vec4 phong = vi * myspecular * (lightcolor / fraction) * pow (std::max(nDotH, 0.0f), myshininess) ; 

	vec4 retval = lambert + phong ; 
	return retval ; 
}