#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Point.h"
#include "QuadTree.h"
#include "Line.h"
using namespace sf;
Vector2f normalize(const Vector2f& v) {
	float l = sqrtf(v.x*v.x + v.y*v.y);
	return v / l;
}

int inline GetIntersection(float fDst1, float fDst2, const Vector2f& P1, const Vector2f& P2, Vector2f& Hit) {
	if ((fDst1 * fDst2) >= 0.0f) return 0;
	if (fDst1 == fDst2) return 0;
	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return 1;
}

int inline InBox(Vector2f& Hit, const Point& B1, const Point& B2, const int Axis) {
	if (Axis == 1 && Hit.y > B1.y && Hit.y < B2.y) return 1;
	if (Axis == 2 && Hit.x > B1.x && Hit.x < B2.x) return 1;
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return 1;
	return 0;
}

int intersect(const Point& B1, const Point& B2, const Vector2f& L1, const Vector2f& L2, Vector2f& Hit) {
	if (L2.x < B1.x && L1.x < B1.x) return false;
	if (L2.x > B2.x && L1.x > B2.x) return false;
	if (L2.y < B1.y && L1.y < B1.y) return false;
	if (L2.y > B2.y && L1.y > B2.y) return false;
	if (L1.x > B1.x && L1.x < B2.x && L1.y > B1.y && L1.y < B2.y) {
		Hit = L1;
		return true;
	}
	if ((GetIntersection(L1.x - B1.x, L2.x - B1.x, L1, L2, Hit) && InBox(Hit, B1, B2, 1))
		|| (GetIntersection(L1.y - B1.y, L2.y - B1.y, L1, L2, Hit) && InBox(Hit, B1, B2, 2))
		|| (GetIntersection(L1.x - B2.x, L2.x - B2.x, L1, L2, Hit) && InBox(Hit, B1, B2, 1))
		|| (GetIntersection(L1.y - B2.y, L2.y - B2.y, L1, L2, Hit) && InBox(Hit, B1, B2, 2)))
		return true;

	return false;
}

Boundary getSearchArea(const RectangleShape& box) {
	float boxSizeX = box.getSize().x;
	float boxSizeY = box.getSize().y;
	float x = box.getPosition().x;
	float y = box.getPosition().y;
	return Boundary(Point(x, y), Point(x + boxSizeX, y + boxSizeY));
}

void createParticle(vector<Vertex>& vertex, float x, float y, float size, Color color) {
	vertex.push_back(Vertex(Vector2f(x - size, y - size), color));
	vertex.push_back(Vertex(Vector2f(x + size, y - size), color));
	vertex.push_back(Vertex(Vector2f(x + size, y + size), color));
	vertex.push_back(Vertex(Vector2f(x - size, y + size), color));
}

void getParticles(const vector<Point>& points, vector<Vertex>& vertexArray, float particleSize, Color color) {
	for (const Point& p : points) {
		createParticle(vertexArray, p.x, p.y, particleSize, color);
	}
}

float random(float min, float max) {
	return (float)(rand() % (int)max + min);
}

Point getPoint(float x, float y, float size) {
	return Point(x + size, y + size);
}

void getSelectedParticles(const QuadTree& tree, const Line& line, vector<Vertex>& vertices, float size, Color color) {
	const Boundary* treeBounds = tree.getBound();
	Vector2f hit;
	if (!intersect(treeBounds->topLeft, treeBounds->bottomRight, line.p1, line.p2, hit)) return;
	const vector<Point>* points = tree.getPoints();
	for (const Point& p : *points) {
		if (intersect(getPoint(p.x, p.y, -size), getPoint(p.x, p.y, size), line.p1, line.p2, hit)) {
			createParticle(vertices, p.x, p.y, size, color);
		}
	}
	if (!tree.subdivided()) return;
	getSelectedParticles(*tree.getNW(), line, vertices, size, color);
	getSelectedParticles(*tree.getNE(), line, vertices, size, color);
	getSelectedParticles(*tree.getSW(), line, vertices, size, color);
	getSelectedParticles(*tree.getSE(), line, vertices, size, color);
}