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

	bool contains(const Vector3& point) const {
		if ((point.x >= topLeft.x && point.x <= bottomRight.x) &&
			(point.y >= topLeft.y && point.y <= bottomRight.y) && 
			(point.z >= topLeft.z && point.z <= bottomRight.z)) {
			return true;
		}
		return false;
	}

	bool intersects(const Boundary& boundary) {
		return (topLeft.x < boundary.bottomRight.x) &&
			(bottomRight.x > boundary.topLeft.x) &&
			(topLeft.y < boundary.bottomRight.y) &&
			(bottomRight.y > boundary.topLeft.y) &&
			(topLeft.z < boundary.bottomRight.z) &&
			(bottomRight.z > boundary.topLeft.z);
	}

	float getLength() {
		return fabs(topLeft.x - bottomRight.x);
	}

	float getWidth() {
		return fabs(topLeft.z - bottomRight.z);
	}

	float getHeight() {
		return fabs(topLeft.y - bottomRight.y);
	}

	void draw() {
		vao->bind();
		glLineWidth(1.5);
		glDrawArrays(GL_LINE_STRIP, 0, vertices.size()/3);
	}

	~Boundary() {
		delete vao;
		delete vbo;
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