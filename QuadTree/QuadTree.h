#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include "Point.h"
#include "Triangle.h"
#include "Boundary.h"
typedef unsigned int uint;
using namespace std;
using namespace sf;

class QuadTree {
public:
	QuadTree() {
		divided = false;
		NW = NULL;
		NE = NULL;
		SW = NULL;
		SE = NULL;
	}

	QuadTree(const Boundary& bound, uint maxCount) :
		bound(bound),maxCount(maxCount) {
		points.reserve(maxCount);
		divided = false;
		NW = NULL;
		NE = NULL;
		SW = NULL;
		SE = NULL;
	}

	void insert(const Point& point,uint depth = 0) {
		//if it doesn't contain then the point is out of bounds
		if (!bound.contains(point)) return;
		//if there is size then add it
		if (points.size() < maxCount) { 
			points.push_back(point); 
			return;
		}
		//at this point we need to sub divide
		if(!divided) subdivide();
		NW->insert(point, depth + 1);
		NE->insert(point, depth + 1);
		SW->insert(point, depth + 1);
		SE->insert(point, depth + 1);
	}

	//First check if triangle is inside the node box. If it does,
	//then subdivide until to insert triangle into the lowest node.
	//If it doesn't insert into its child, then insert into the root's
	//data.
	bool insert(const Triangle& triangle, uint depth = 0) {
		if (!triangle.isInside(bound) || depth >= 10) return false;
		if (!divided) subdivide();
		//insert into root if no nodes can support it
		if (!(NW->insert(triangle, depth + 1) || NE->insert(triangle, depth + 1) ||
			SW->insert(triangle, depth + 1) || SE->insert(triangle, depth + 1))) {
			triangles.push_back(triangle);
		}
		return true;
	}

	void searchArea(const Boundary& boundary, list<Point>& searchPoints) {
		if (!bound.intersects(boundary)) return;
		for (Point& p : points) {
			if (boundary.contains(p)) searchPoints.push_back(p);
		}
		if (!divided) return;
		NW->searchArea(boundary, searchPoints);
		NE->searchArea(boundary, searchPoints);
		SW->searchArea(boundary, searchPoints);
		SE->searchArea(boundary, searchPoints);
	}

	void getBoxs(vector<sf::Vertex>& quadTree, sf::Color color) {
		float topX = bound.topLeft.x;
		float topY = bound.topLeft.y;
		float bottomX = bound.bottomRight.x;
		float bottomY = bound.bottomRight.y;
		float dw = fabs(topX - bottomX);
		float dh = fabs(topY - bottomY);

		quadTree.push_back(sf::Vertex(sf::Vector2f(topX,topY), color));
		quadTree.push_back(sf::Vertex(sf::Vector2f(topX+dw,topY), color));

		quadTree.push_back(sf::Vertex(sf::Vector2f(topX + dw, topY), color));
		quadTree.push_back(sf::Vertex(sf::Vector2f(topX + dw, topY+dh), color));

		quadTree.push_back(sf::Vertex(sf::Vector2f(topX + dw, topY + dh), color));
		quadTree.push_back(sf::Vertex(sf::Vector2f(topX, topY+dh), color));

		quadTree.push_back(sf::Vertex(sf::Vector2f(topX, topY + dh), color));
		quadTree.push_back(sf::Vertex(sf::Vector2f(topX, topY), color));
		if (!divided) return;
		NW->getBoxs(quadTree, color);
		NE->getBoxs(quadTree, color);
		SW->getBoxs(quadTree, color);
		SE->getBoxs(quadTree, color);
	}

	void subdivide() {
		Boundary NWB, NEB, SWB, SEB;

		float dw = fabs(bound.topLeft.x - bound.bottomRight.x) / 2;
		float dh = fabs(bound.topLeft.y - bound.bottomRight.y) / 2;

		NWB = Boundary(bound.topLeft, Point(bound.topLeft.x + dw, bound.topLeft.y + dh));
		NEB = Boundary(Point(bound.topLeft.x + dw, bound.topLeft.y), Point(bound.topLeft.x + dw * 2, bound.topLeft.y + dh));
		SWB = Boundary(Point(bound.topLeft.x, bound.topLeft.y + dh), Point(bound.topLeft.x + dw, bound.topLeft.y + dh * 2));
		SEB = Boundary(Point(bound.topLeft.x + dw, bound.topLeft.y + dh), bound.bottomRight);

		NW = new QuadTree(NWB, maxCount);
		NE = new QuadTree(NEB, maxCount);
		SW = new QuadTree(SWB, maxCount);
		SE = new QuadTree(SEB, maxCount);
		divided = true;
	}

	const Boundary* getBound() const {
		return &bound;
	}

	const vector<Point>* getPoints() const {
		return &points;
	}

	const vector<Triangle>* getTriangles() const {
		return &triangles;
	}

	bool subdivided() const{
		return divided;
	}

	QuadTree* getNW() const { return NW; };
	QuadTree* getNE() const { return NE; };
	QuadTree* getSW() const { return SW; };
	QuadTree* getSE() const { return SE; };
	 
	~QuadTree() {
		if (!divided) return;
		delete NW;
		delete NE;
		delete SW;
		delete SE;
	}
private:
	vector<Point> points;
	vector<Triangle> triangles;
	Boundary bound;
	uint maxCount;
	QuadTree* NW;
	QuadTree* NE;
	QuadTree* SW;
	QuadTree* SE;
	bool divided;
	float boxOutlineSize = 0.5;
};

