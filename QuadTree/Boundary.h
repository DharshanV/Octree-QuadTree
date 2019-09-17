#pragma once
#include <iostream>
#include <Utitlites/VertexArray.h>
#include <Utitlites/VertexBuffer.h>
#include <Utitlites/VertexBufferLayout.h>

using namespace std;
using namespace glm;
class Boundary {
public:
	Boundary() :topLeft(0, 0, 0), bottomRight(0, 0, 0) { init(); }
	Boundary(const vec3& topLeft, const vec3& bottomRight) :
		topLeft(topLeft), bottomRight(bottomRight) {
		init();
	}

	Boundary(const Boundary& other) {
		topLeft = other.topLeft;
		bottomRight = other.bottomRight;
		init();
	}

	Boundary& operator = (const Boundary& other) {
		if (this == &other) return *this;
		delete vao;
		delete vbo;
		topLeft = other.topLeft;
		bottomRight = other.bottomRight;
		init();
		return *this;
	}

	bool contains(const vec3& point) const {
		if ((point.x >= topLeft.x && point.x <= bottomRight.x)&& 
			(point.y >= bottomRight.y && point.y <= topLeft.y) && 
			(point.z >= bottomRight.z && point.z <= topLeft.z)) {
			return true;
		}
		return false;
	}

	bool intersects(const Boundary& other) {
		vec3 thisCenter(topLeft.x + getLength() / 2, topLeft.y - getHeight() / 2, topLeft.z - getWidth() / 2);
		vec3 otherCenter(other.topLeft.x + other.getLength() / 2, other.topLeft.y - other.getHeight() / 2, other.topLeft.z - other.getWidth() / 2);
		return (abs(thisCenter.x - otherCenter.x) * 2 < (getLength() + other.getLength())) &&
			(abs(thisCenter.y - otherCenter.y) * 2 < (getHeight() + other.getHeight())) && 
			(abs(thisCenter.z - otherCenter.z) * 2 < (getWidth() + other.getWidth()));
	}

	float getLength() const {
		return fabs(topLeft.x - bottomRight.x);
	}

	float getWidth() const {
		return fabs(topLeft.z - bottomRight.z);
	}

	float getHeight() const {
		return fabs(topLeft.y - bottomRight.y);
	}

	void draw() {
		vao->bind();
		glLineWidth(1.5);
		glDrawArrays(GL_LINE_STRIP, 0, vertices.size()/3);
	}

	vec3 getTopLeft() {
		return topLeft;
	}

	vec3 getBottomRight() {
		return bottomRight;
	}

	~Boundary() {
		if (vao != NULL) {delete vao; vao = NULL;}
		if (vbo != NULL) { delete vbo; vbo = NULL; }
	}
private:
	void init() {
		createVerticies();
		vao = new VertexArray();
		vbo = new VertexBuffer(&vertices[0], vertices.size() * 4);
		VertexBufferLayout layout;
		layout.push<float>(3);
		vao->addBuffer(*vbo, layout);
	}
	
	void createVerticies() {
		vertices.reserve(48);
		//left quad
		vertices.push_back(topLeft.x);
		vertices.push_back(topLeft.y);
		vertices.push_back(topLeft.z);
		vertices.push_back(topLeft.x);
		vertices.push_back(topLeft.y);
		vertices.push_back(topLeft.z-getWidth());
		vertices.push_back(topLeft.x);
		vertices.push_back(topLeft.y - getHeight());
		vertices.push_back(topLeft.z - getWidth());
		vertices.push_back(topLeft.x);
		vertices.push_back(topLeft.y-getHeight());
		vertices.push_back(topLeft.z);
		vertices.push_back(topLeft.x);
		vertices.push_back(topLeft.y);
		vertices.push_back(topLeft.z);

		//top quad
		vertices.push_back(topLeft.x + getLength());
		vertices.push_back(topLeft.y);
		vertices.push_back(topLeft.z);
		vertices.push_back(topLeft.x + getLength());
		vertices.push_back(topLeft.y);
		vertices.push_back(topLeft.z - getWidth());
		vertices.push_back(topLeft.x);
		vertices.push_back(topLeft.y);
		vertices.push_back(topLeft.z-getWidth());

		//back quad
		vertices.push_back(topLeft.x);
		vertices.push_back(topLeft.y-getHeight());
		vertices.push_back(topLeft.z-getWidth());
		vertices.push_back(bottomRight.x);
		vertices.push_back(bottomRight.y);
		vertices.push_back(bottomRight.z);
		vertices.push_back(bottomRight.x);
		vertices.push_back(bottomRight.y+getHeight());
		vertices.push_back(bottomRight.z);

		vertices.push_back(bottomRight.x);
		vertices.push_back(bottomRight.y);
		vertices.push_back(bottomRight.z);
		vertices.push_back(bottomRight.x);
		vertices.push_back(bottomRight.y);
		vertices.push_back(bottomRight.z+getWidth());

		vertices.push_back(bottomRight.x);
		vertices.push_back(bottomRight.y + getHeight());
		vertices.push_back(bottomRight.z + getWidth());

		vertices.push_back(bottomRight.x);
		vertices.push_back(bottomRight.y);
		vertices.push_back(bottomRight.z + getWidth());

		vertices.push_back(bottomRight.x -getLength());
		vertices.push_back(bottomRight.y);
		vertices.push_back(bottomRight.z+getWidth());
	}
private:
	vec3 topLeft;
	vec3 bottomRight;

	vector<float> vertices;
	VertexArray* vao;
	VertexBuffer* vbo;
};