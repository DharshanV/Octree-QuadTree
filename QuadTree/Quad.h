#pragma once
#include "Vector3.h"
#include <Utitlites/Shader.h>
class Quad {
public:
	Quad(const Vector3& position,float size) :
		position(position), size(size) {
		init();
	}

	Quad(const Quad& q) :
		position(q.position), size(q.size) {
		init();
	}

	Quad& operator = (const Quad& other) {
		if (this == &other) return *this;
		delete vao;
		delete vbo;
		position = other.position;
		size = other.size;
		init();
		return *this;
	}

	~Quad() {
		if (vao != NULL) { delete vao; vao = NULL; }
		if (vbo != NULL) { delete vbo; vbo = NULL; }
	}

	void init() {
		createVerticies();
		vao = new VertexArray();
		vbo = new VertexBuffer(&vertices[0].x, vertices.size() * 12);
		VertexBufferLayout layout;
		layout.push<float>(3);
		vao->addBuffer(*vbo, layout);
	}

	void draw(Shader& shader) {
		shader.use();
		vao->bind();
		glPointSize(8.0f);
		glDrawArrays(GL_POINTS, 0, vertices.size());
	}
private:
	void createVerticies() {
		vertices.push_back(position);
	}
private:
	Vector3 position;
	float size;
	vector<Vector3> vertices;
	VertexArray* vao;
	VertexBuffer* vbo;
};