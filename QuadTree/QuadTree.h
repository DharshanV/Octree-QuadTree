#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
typedef unsigned int uint;

struct Point {
public:
	Point() : x(0), y(0) {}
	Point(float x, float y) : x(x), y(y) {}
private:
	float x;
	float y;
};

struct Boundary {
public:
	Boundary(const Point& topLeft, const Point& bottomRight) : 
		topLeft(topLeft),bottomRight(bottomRight) {

	}
private:
	Point topLeft;
	Point bottomRight;
};

class QuadTree {
public:
	QuadTree(const Boundary& bounds, uint maxCount) : 
		bounds(bounds),maxCount(maxCount) {

	}

	void insert(const Point& point) {
		
	}

	void draw(sf::RenderWindow* window) {

	}

	~QuadTree() { 
		delete NW;
		delete NE;
		delete SW;
		delete SE;
	}
private:
	vector<Point> myData;
	Boundary bounds;
	uint maxCount;
	QuadTree* NW;
	QuadTree* NE;
	QuadTree* SW;
	QuadTree* SE;
};

