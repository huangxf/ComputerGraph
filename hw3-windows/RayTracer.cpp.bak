// RayTracer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Scene.h"
#include "Sample.h"
#include "Film.h"
#include "readfile.h"


int main(int argc, char * argv[])
{
	Scene scene;
	scene.eye = vec3(0, 0, 0);

	scene.width = 255;
	scene.height = 255;

	if(argc <= 1) return 1;
	std::string filename(argv[1]);

	Film film(scene);
	ReadFile loader;
	loader.readfile(filename.c_str(), film);
	//film.preprocess();
	film.render();
	film.output(film.output_name.c_str());

	return 0;
}