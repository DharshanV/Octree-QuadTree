#pragma once
#include <SFML/Graphics.hpp>
#include "Point.h"
#include "Boundary.h"
using namespace sf;

class Triangle {
public:
	Triangle():p1(0,0), p2(0, 0), p3(0, 0),color(Color::Red) { }
	Triangle(Vector2f p1, Vector2f p2, Vector2f p3) : p1(p1), 
		p2(p2), p3(p3), color(Color::Red) {}

	bool isInside(const Boundary& bound) const {
		return (bound.contains(p1) && bound.contains(p2) && bound.contains(p3));
	}
	vector<Vertex> getVertices() const {
		vector<Vertex> temp;
		temp.reserve(3);
		temp.push_back(Vertex(Vector2f(p1.x, p1.y), color));
		temp.push_back(Vertex(Vector2f(p2.x, p2.y), color));
		temp.push_back(Vertex(Vector2f(p3.x, p3.y), color));
		return temp;
	}
	const Vector2f* getP1() const { return &p1; };
	const Vector2f* getP2() const { return &p2; };
	const Vector2f* getP3() const { return &p3; };
private:
	Vector2f p1;
	Vector2f p2;
	Vector2f p3;
	Color color;
};