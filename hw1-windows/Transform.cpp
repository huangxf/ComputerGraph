// Transform.cpp: implementation of the Transform class.


#include "Transform.h"

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
  // YOUR CODE FOR HW1 HERE
  float radius = pi * degrees / 180;
  mat3 Identity3x3 = mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);
  mat3 part2 = mat3(axis.x*axis.x, axis.x*axis.y, axis.x*axis.z, 
					axis.x*axis.y, axis.y*axis.y, axis.y*axis.z, 
					axis.x*axis.z, axis.y*axis.z, axis.z*axis.z);
  mat3 part3 = mat3(0, axis.z, -axis.y,
					-axis.z, 0, axis.x, 
					axis.y, -axis.x, 0);
  mat3 part1 = Identity3x3 * cos(radius);
  mat3 result = part1 + (1 - cos(radius)) * part2 + sin(radius) * part3;
  // You will change this return call
  return result;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE
  eye = eye * rotate(-degrees, up);
  //up = up * rotate(-degrees, up);
  //lookAt(eye, up);
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE 
  vec3 axis = glm::normalize(glm::cross(up, eye));
  mat3 r_Matrix = rotate(degrees, axis);
  eye = eye * r_Matrix;
  up = up * r_Matrix;
  //lookAt(eye,up);
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
  // YOUR CODE FOR HW1 HERE
  vec3 a = eye - vec3(0, 0, 0); //always look at origin point
  vec3 w = glm::normalize(a);
  vec3 u = glm::normalize(glm::cross(up, w));
  vec3 v = glm::cross(w, u);

  //glLookAt matrix
  mat4 result = mat4(u.x,                             v.x,                            w.x,                           0,
					 u.y,                             v.y,                            w.y,                           0,
					 u.z,                             v.z,                            w.z,                           0,
					 -u.x*eye.x-u.y*eye.y-u.z*eye.z, -v.x*eye.x-v.y*eye.y-v.z*eye.z, -w.x*eye.x-w.y*eye.y-w.z*eye.z, 1);
  // You will change this return call
  return result;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
