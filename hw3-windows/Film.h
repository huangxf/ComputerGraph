#include "Scene.h"
#include "Sample.h"
#include "Camera.h"
#include "Sphere.h"
#include "Light.h"
#include <vector>


#ifndef FILM
#define FILM

class Film
{
public:
	Film(Scene scene)
	{
		this->scene = scene;
		//samples = new Sample[scene.height * scene.width];
		Camera cam(vec3(0, 0, 2), vec3(0, 0, -1), vec3(0, 1, 0), 90);
		this->camera = cam;
		Sphere sp(vec3(0, 0, -20), 10);
		//this->sphere = sp;
		ratio = scene.width / scene.height;
		maxDepth = 5;
	}

	~Film()
	{
		if(!samples) delete[] samples;
	}

	
	IntersectResult trace(const Ray &ray);
	void preprocess();

	void render();
	void output(const char * path);
	vec4 getColor(const Ray &ray, const IntersectResult& result, int depth);
	vec4 computeLight(
		const vec3 &direction, 
		const vec4 &lightcolor, 
		const vec3 &normal, 
		const vec3 &halfvec, 
		const vec4 &mydiffuse, 
		const vec4 &myspecular, 
		const float &myshininess,
		const int  &vi,
		const vec3 &attenuation,
		const float &light_dis_square);
		
	vec3 reflectDir(const Ray &injectRay, const IntersectResult& reflectionPoint);

	Scene scene;
	Sample* samples;
	Camera camera;
	Sphere sphere;
	float ratio;
	int maxDepth;
	std::string output_name;

	std::vector<Sphere> objects;
	std::vector<Light> lights;
};

#endif