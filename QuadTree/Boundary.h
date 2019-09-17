#pragma once
#include <iostream>
#include <Utitlites/VertexArray.h>
#include <Utitlites/VertexBuffer.h>
#include <Utitlites/VertexBufferLayout.h>
#include "Vector3.h"
using namespace std;
class Boundary {
public:
	Boundary() :topLeft(0, 0, 0), bottomRight(0, 0, 0) { init(); }
	Boundary(const Vector3& topLeft, const Vector3& bottomRight) :
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

	bool contains(const Vector3& point) const {
		if ((point.x >= topLeft.x && point.x <= bottomRight.x)&& 
			(point.y >= bottomRight.y && point.y <= topLeft.y) && 
			(point.z >= bottomRight.z && point.z <= topLeft.z)) {
			return true;
		}
		return false;
	}

	bool intersects(const Boundary& boundary) {
		if (fabs(topLeft.x - boundary.topLeft.x) <= getLength() + boundary.getLength()) {
			//check the Y axis
			if (fabs(topLeft.y - boundary.topLeft.y) <= getHeight() + boundary.getHeight()) {
				//check the Z axis
				if (fabs(topLeft.z - boundary.topLeft.z) <= getWidth() + boundary.getWidth()) {
					return true;
				}
			}
		}
		return false;
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

	Vector3 getTopLeft() {
		return topLeft;
	}

	Vector3 getBottomRight() {
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
	Vector3 topLeft;
	Vector3 bottomRight;

	vector<float> vertices;
	VertexArray* vao;
	VertexBuffer* vbo;
};