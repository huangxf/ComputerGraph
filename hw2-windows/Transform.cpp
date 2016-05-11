// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
    mat3 ret;
    // YOUR CODE FOR HW2 HERE
    // Please implement this.  Likely the same as in HW 1.
	float radius = pi * degrees / 180;
	mat3 Identity3x3 = mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);
	mat3 part2 = mat3(axis.x*axis.x, axis.x*axis.y, axis.x*axis.z, 
					axis.x*axis.y, axis.y*axis.y, axis.y*axis.z, 
					axis.x*axis.z, axis.y*axis.z, axis.z*axis.z);
	mat3 part3 = mat3(0, axis.z, -axis.y,
					-axis.z, 0, axis.x, 
					axis.y, -axis.x, 0);
	mat3 part1 = Identity3x3 * cos(radius);
	ret = part1 + (1 - cos(radius)) * part2 + sin(radius) * part3;
    return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.
	up = upvector(up, eye); //up vector must be normalized. Otherwise the size of object will be changed in transformation
	eye = eye * rotate(-degrees, up);
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE 
    // Likely the same as in HW 1.
	up = upvector(up, eye);
	vec3 axis = glm::normalize(glm::cross(up, eye));
	mat3 r_Matrix = rotate(degrees, axis);
	eye = eye * r_Matrix;
	up = up * r_Matrix;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.  
	vec3 a = eye - center ;
	vec3 w = glm::normalize(a);
	vec3 u = glm::normalize(glm::cross(up, w));
	vec3 v = glm::cross(w, u);

    //glLookAt matrix
	ret = mat4(u.x,                             v.x,                            w.x,                           0,
			   u.y,                             v.y,                            w.y,                           0,
			   u.z,                             v.z,                            w.z,                           0,
			   -u.x*eye.x-u.y*eye.y-u.z*eye.z, -v.x*eye.x-v.y*eye.y-v.z*eye.z, -w.x*eye.x-w.y*eye.y-w.z*eye.z, 1);
    return ret; 
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well. 
	float theta = pi * fovy / 360; //half of fovy in radius.
	float d = 1/tan(theta); //d = cot(theta).
	float A = -(zFar + zNear) / (zFar - zNear);
	float B = - 2 * zFar * zNear / (zFar - zNear);
	//glPerspective matrix
	ret = mat4( d / aspect, 0, 0, 0,
			             0, d, 0, 0,
					     0, 0, A, -1,
					     0, 0, B, 0 );
    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Implement scaling 
	ret = mat4(sx, 0, 0, 0,
			    0,sy, 0, 0,
				0, 0,sz, 0,
				0, 0, 0, 1);
    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Implement translation 
	ret = mat4( 1, 0, 0, 0,
			    0, 1, 0, 0,
				0, 0, 1, 0,
			   tx,ty,tz, 1);
    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
