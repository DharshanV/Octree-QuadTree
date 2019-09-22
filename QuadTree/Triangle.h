#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Boundary.h"
using namespace std;
using namespace glm;

class Triangle {
public:
	Triangle():p1(0,0,0), p2(0, 0, 0), p3(0, 0, 0),color(1,1,1) { }
	Triangle(vec3 p1, vec3 p2, vec3 p3) : p1(p1), 
		p2(p2), p3(p3), color(1, 1, 1) {}

	const vec3* getP1() const { return &p1; };
	const vec3* getP2() const { return &p2; };
	const vec3* getP3() const { return &p3; };
private:
	vec3 p1;
	vec3 p2;
	vec3 p3;
	vec3 color;
	vector<float> vertices;
};