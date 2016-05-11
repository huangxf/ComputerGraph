#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "Transform.h" 
#include "Film.h"
#include "Light.h"


#ifndef READFILE
#define READFILE

using namespace std;

class ReadFile
{
public:
// Readfile definitions 

	ReadFile(): maxverts(3000L), maxvertnorms(1000L), maxobjects(1000L), numobjects(0L), numverts(0L), numused(0), shininess(0), name("")
	{
		ambient[0] = 0.2; ambient[1] = 0.2; ambient[2] = 0.2; ambient[3] = 1; 
		attenuation[0] = 1.0; attenuation[1] = 0.0; attenuation[2] = 0.0; attenuation[3] = 1.0;
		memset(diffuse, 0, 4 * sizeof(float));
		memset(specular, 0, 4 * sizeof(float));
		memset(emission, 0, 4 * sizeof(float));
	}

void matransform (stack<mat4> &transfstack, float * values) ;
static enum {view, translate, scale} transop ; // which operation to transform 
enum shape {cube, sphere, teapot} ;

//float s_x, s_y ; // the scale in x and y 
//float tx, ty ; // the translation in x and y

vec3 eyeinit ; 
vec3 upinit ; 
vec3 center ; 
int amountinit;
int w, h ; 
float fovy ; 
long maxverts ;
long maxvertnorms ;
long maxobjects ;
long numobjects ;
long numverts ;
std::string name;

vec3 vertices[60000L];

// Lighting parameter array, similar to that in the fragment shader

float lightposn [40] ; // Light Positions
float lightcolor[40] ; // Light Colors
float lightransf[40] ; // Lights transformed by modelview
int numused ;                     // How many lights are used 

// Materials (read from file) 
// With multiple objects, these are colors for each.
float ambient[4] ; 
float diffuse[4] ; 
float specular[4] ; 
float emission[4] ; 
float attenuation[4];
float shininess ; 

void rightmultiply (const mat4 & M, stack<mat4> &transfstack) ;
bool readvals (stringstream &s, const int numvals, float * values) ;
void readfile (const char * filename, Film & film) ;


};

#endif
