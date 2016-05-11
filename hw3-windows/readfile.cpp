/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167x by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

/*****************************************************************************/
// This file is readfile.cpp.  It includes helper functions for matrix 
// transformations for a stack (matransform) and to rightmultiply the 
// top of a stack.  These functions are given to aid in setting up the 
// transformations properly, and to use glm functions in the right way.  
// Their use is optional in your program.  
  

// The functions readvals and readfile do basic parsing.  You can of course 
// rewrite the parser as you wish, but we think this basic form might be 
// useful to you.  It is a very simple parser.

// Please fill in parts that say YOUR CODE FOR HW 2 HERE. 
// Read the other parts to get a context of what is going on. 
  
/*****************************************************************************/

// Basic includes to get this file to work.  
#include "stdafx.h"
#include "readfile.h"

using namespace std;


// You may not need to use the following two functions, but it is provided
// here for convenience

// The function below applies the appropriate transform to a 4-vector
void ReadFile::matransform(stack<mat4> &transfstack, float* values) 
{
    mat4 transform = transfstack.top(); 
    vec4 valvec = vec4(values[0],values[1],values[2],values[3]); 
    vec4 newval = transform * valvec; 
    for (int i = 0; i < 4; i++) values[i] = newval[i]; 
}

void ReadFile::rightmultiply(const mat4 & M, stack<mat4> &transfstack) 
{
    mat4 &T = transfstack.top(); 
    T = T * M; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool ReadFile::readvals(stringstream &s, const int numvals, float* values) 
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i]; 
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n"; 
            return false;
        }
    }
    return true; 
}

void ReadFile::readfile(const char* filename, Film & film) 
{
	fovy = film.camera.fov;
	w = film.scene.width;
	h = film.scene.height;
	eyeinit = film.camera.eye;
	upinit = film.camera.up;
	center = film.camera.lookat;

    string str, cmd; 
    ifstream in;
    in.open(filename); 
    if (in.is_open()) {

        // I need to implement a matrix stack to store transforms.  
        // This is done using standard STL Templates 
        stack <mat4> transfstack; 
        transfstack.push(mat4(1.0));  // identity

        getline (in, str); 
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines 

                stringstream s(str);
                s >> cmd; 
                int i; 
                float values[10]; // Position and color for light, colors for others
                                    // Up to 10 params for cameras.  
                bool validinput; // Validity of input 

                // Process the light, add it to database.
                // Lighting Command
                if (cmd == "light" || cmd == "directional" || cmd == "point") {
                    if (numused == 40) { // No more Lights 
                        cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n";
                    } else {
                        validinput = readvals(s, 6, values); // Position/color for lts.
                        if (validinput) {

                            // YOUR CODE FOR HW 2 HERE. 
                            // Note that values[0...7] shows the read in values 
                            // Make use of lightposn[] and lightcolor[] arrays in variables.h
                            // Those arrays can then be used in display too.  
							// Light pos.
							int type;
							for(int i = 0; i < 3; i++)
							{
								lightposn[numused * 4 + i] = values[i];
							}
							vec3 dir = vec3(lightposn[numused * 4], lightposn[numused * 4+1], lightposn[numused * 4+2]);
							if(cmd == "directional") {
								type = 0;
							}
							else {
								type = 1;
							}
							lightposn[numused * 4 + 3] = type;
							// Light color.
							for(int i = 0; i < 3; i++)
							{
								lightcolor[numused * 4 + i] = values[i + 3];
							}
							vec3 color = vec3(lightcolor[numused * 4], lightcolor[numused * 4+1], lightcolor[numused * 4+2]);
							lightcolor[numused * 4 + 3] = 255;

							Light light(type, dir, color);
							light.attenuation = vec3(attenuation[0], attenuation[1], attenuation[2]);
							film.lights.push_back(light);

                            ++numused; 
                        }
                    }
                }


                // Material Commands 
                // Ambient, diffuse, specular, shininess properties for each object.
                // Filling this in is pretty straightforward, so I've left it in 
                // the skeleton, also as a hint of how to do the more complex ones.
                // Note that no transforms/stacks are applied to the colors. 

                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values); // colors 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            ambient[i] = values[i]; 
                        }
						ambient[3] = 1;
                    }
                } else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            diffuse[i] = values[i]; 
                        }
						diffuse[3] = 1;
                    }
                } else if (cmd == "specular") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            specular[i] = values[i]; 
                        }
						specular[3] = 1;
                    }
                } else if (cmd == "emission") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            emission[i] = values[i]; 
                        }
						emission[3] = 1;
                    }
                } else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values); 
                    if (validinput) {
                        shininess = values[0]; 
                    }
                } else if (cmd == "size") {
                    validinput = readvals(s,2,values); 
                    if (validinput) { 
                        w = (int) values[0]; h = (int) values[1]; 
						film.scene.width = w;
						film.scene.height = h;
						film.ratio = w * 1.0f / h;
						film.samples = new Sample[w * h];
                    } 
                } else if (cmd == "camera") {
                    validinput = readvals(s,10,values); // 10 values eye cen up fov
                    if (validinput) {

                        // YOUR CODE FOR HW 2 HERE
                        // Use all of values[0...9]
                        // You may need to use the upvector fn in Transform.cpp
                        // to set up correctly. 
                        // Set eyeinit upinit center fovy in variables.h 

	

						float lookfromx = values[0];
						float lookfromy = values[1];
						float lookfromz = values[2];

						float lookatx = values[3];
						float lookaty = values[4];
						float lookatz = values[5];

						float upx = values[6];
						float upy = values[7];
						float upz = values[8];

						fovy = values[9];

						eyeinit = vec3(lookfromx, lookfromy, lookfromz);
						center = vec3(lookatx, lookaty, lookatz);
						upinit = vec3(upx, upy, upz);
						film.camera = Camera(eyeinit, center, upinit, fovy);
						vec3 zvec = center - eyeinit;
						//up = Transform::upvector(upinit, zvec);
                    }
                }

				else if(cmd == "attenuation")
				{
                    validinput = readvals(s, 3, values); // attenuation 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
							attenuation[i] = values[i]; 
                        }
						attenuation[3] = 1;
                    }
				}

				else if(cmd == "output")
				{
                    //validinput = readvals(s,1,values); // 10 values eye cen up fov
                    //if (validinput) {
						s>>film.output_name;
				}

				else if(cmd == "maxverts")
				{
				}

				else if(cmd == "name")
				{
					//validinput = readvals(s, 1, values);
					//if (validinput) name = values[0];
					s>>name;
				}

				else if(cmd == "maxdepth")
				{
					validinput = readvals(s, 1, values);
					film.maxDepth = values[0];
				}

                // I've left the code for loading objects in the skeleton, so 
                // you can get a sense of how this works.  
                // Also look at demo.txt to get a sense of why things are done this way.
                else if (cmd == "sphere" || cmd == "cube" || cmd == "teapot" || cmd == "tri") {
                    if (numobjects == maxobjects) { // No more objects 
                        cerr << "Reached Maximum Number of Objects " << numobjects << " Will ignore further objects\n";
                    } else {

                        if(cmd == "sphere") validinput = readvals(s, 4, values);
						else validinput = readvals(s, 3, values);

                        if (validinput) {
                            //object * obj = &(objects[numobjects]); 
							Sphere obj;
                            //obj->size = values[0]; 

                            // Set the object's light properties
                            for (i = 0; i < 4; i++) {
                                (obj.ambient)[i] = ambient[i]; 
                                (obj.diffuse)[i] = diffuse[i]; 
                                (obj.specular)[i] = specular[i]; 
                                (obj.emission)[i] = emission[i];
                            }
                            obj.shininess = shininess; 

                            // Set the object's transform
                            obj.trans = transfstack.top(); 
							
                            // Set the object's type
                            if (cmd == "sphere") {
                                obj.type = 0; //sphere
								obj.origin = vec3(values[0], values[1], values[2]);
								obj.name = name;
								//obj.transform();
								obj.radius = values[3];
                            } else if (cmd == "cube") {
                                //obj.type = cube; 
                            } else if (cmd == "teapot") {
                                //obj.type = teapot; 
                            } else if (cmd == "tri") {
								obj.type = 1; //tri
								obj.origin = vertices[(int)(values[0])];
								obj.v2 = vertices[(int)(values[1])];
								obj.v3 = vertices[(int)(values[2])];
								obj.name = name;
								//obj.transform();
							}
							film.objects.push_back(obj);
                        }
                        //++numobjects; 
		
                    }
                }

				else if(cmd == "vertex") {
					validinput = readvals(s, 3, values);
					if(validinput) {
						vertices[numverts++] = vec3(values[0], values[1], values[2]);
					}
				}

                else if (cmd == "translate") {
                    validinput = readvals(s,3,values); 
                    if (validinput) {

                        // YOUR CODE FOR HW 2 HERE.  
                        // Think about how the transformation stack is affected
                        // You might want to use helper functions on top of file. 
                        // Also keep in mind what order your matrix is!
						float tx = values[0];
						float ty = values[1];
						float tz = values[2];
						mat4 translateMatrix = Transform::translate(tx, ty, tz);
						rightmultiply(translateMatrix, transfstack);
                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s,3,values); 
                    if (validinput) {

                        // YOUR CODE FOR HW 2 HERE.  
                        // Think about how the transformation stack is affected
                        // You might want to use helper functions on top of file.  
                        // Also keep in mind what order your matrix is!
						float scalex = values[0];
						float scaley = values[1];
						float scalez = values[2];
						mat4 scaleMatrix = Transform::scale(scalex, scaley, scalez);
						rightmultiply(scaleMatrix, transfstack);

                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s,4,values); 
                    if (validinput) {

                        // YOUR CODE FOR HW 2 HERE. 
                        // values[0..2] are the axis, values[3] is the angle.  
                        // You may want to normalize the axis (or in Transform::rotate)
                        // See how the stack is affected, as above.  
                        // Note that rotate returns a mat3. 
                        // Also keep in mind what order your matrix is!
						float rotatex = values[0];
						float rotatey = values[1];
						float rotatez = values[2];
						vec3 axis = glm::normalize(vec3(rotatex, rotatey, rotatez));
						float angle = values[3];
						mat3 rotateMatrix = Transform::rotate(angle, axis);
						mat4 rotateMat4x4(rotateMatrix); //convertion from 3x3 to 4x4
						rightmultiply(rotateMat4x4, transfstack);
                    }
                }

                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top()); 
                } else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n"; 
                    } else {
                        transfstack.pop(); 
                    }
                }

                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n"; 
                }
            }
            getline (in, str); 
        }

        // Set up initial position for eye, up and amount
        // As well as booleans 

        //eye = eyeinit; 
        //up = upinit; 
        //amount = 5;
        //s_x = s_y = 1.0;  // keyboard controlled scales in x and y 
        //tx = ty = 0.0;  // keyboard controllled translation in x and y  
        //useGlu = false; // don't use the glu perspective/lookat fns

    } else {
        cerr << "Unable to Open Input Data File " << filename << "\n"; 
        throw 2; 
    }
}
