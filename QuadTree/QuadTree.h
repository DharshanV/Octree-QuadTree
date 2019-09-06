#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
typedef unsigned int uint;
using namespace std;

struct Point {
public:
	Point() : x(0), y(0) {}
	Point(float x, float y) : x(x), y(y) {}
	float x;
	float y;
};

struct Boundary {
public:
	Boundary() :topLeft(0,0),bottomRight(0,0) { }
	Boundary(const Point& topLeft, const Point& bottomRight) : 
		topLeft(topLeft),bottomRight(bottomRight) {

	}
	bool contains(const Point& point) const {
		if ((point.x >= topLeft.x && point.x <= bottomRight.x) &&
			(point.y >= topLeft.y && point.y <= bottomRight.y)) {
			return true;
		}
		return false;
	}

	bool intersects(const Boundary& boundary) {
		return (topLeft.x < boundary.bottomRight.x) && 
			(bottomRight.x > boundary.topLeft.x) &&
			(topLeft.y < boundary.bottomRight.y) && 
			(bottomRight.y > boundary.topLeft.y);
	}

	float getWidth() {
		return fabs(topLeft.x - bottomRight.x);
	}

	float getHeight() {
		return fabs(topLeft.y - bottomRight.y);
	}

	Point topLeft;
	Point bottomRight;
};

class QuadTree {
public:
	QuadTree(const Boundary& bound, uint maxCount) :
		bound(bound),maxCount(maxCount) {
		points.reserve(maxCount);
		divided = false;
		NW = NULL;
		NE = NULL;
		SW = NULL;
		SE = NULL;
	}

	void insert(const Point& point) {
		//if it doesn't contain then the point is out of bounds
		if (!bound.contains(point)) return;
		//if there is size then add it
		if (points.size() < maxCount) { 
			points.push_back(point); 
			return;
		}
		//at this point we need to sub divide
		if(!divided) subdivide(point);
		NW->insert(point);
		NE->insert(point);
		SW->insert(point);
		SE->insert(point);
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

	void drawBox(sf::RenderWindow& window) {
		window.draw(getBox());
		if (!divided) return;
		NW->drawBox(window);
		NE->drawBox(window);
		SW->drawBox(window);
		SE->drawBox(window);
	}

	sf::RectangleShape getBox() {
		sf::RectangleShape box;
		float w, h;
		w = fabs(bound.topLeft.x - bound.bottomRight.x);
		h = fabs(bound.topLeft.y - bound.bottomRight.y);
		box.setSize(sf::Vector2f(w, h));
		box.setPosition(sf::Vector2f(bound.topLeft.x, bound.topLeft.y));
		box.setOutlineColor(sf::Color(sf::Color::White));
		box.setFillColor(sf::Color::Transparent);
		box.setOutlineThickness(boxOutlineSize);
		return box;
	}

	void subdivide(const Point& point) {
		Boundary NWB, NEB, SWB, SEB;

		float dw = fabs(bound.topLeft.x - bound.bottomRight.x) / 2;
		float dh = fabs(bound.topLeft.y - bound.bottomRight.y) / 2;

		NWB = Boundary(bound.topLeft, Point(bound.topLeft.x + dw, bound.topLeft.y + dh));
		NEB = Boundary(Point(bound.topLeft.x+dw, bound.topLeft.y), Point(bound.topLeft.x + dw*2, bound.topLeft.y + dh));
		SWB = Boundary(Point(bound.topLeft.x , bound.topLeft.y + dh), Point(bound.topLeft.x + dw, bound.topLeft.y + dh * 2));
		SEB = Boundary(Point(bound.topLeft.x + dw, bound.topLeft.y + dh), bound.bottomRight);

		NW = new QuadTree(NWB, maxCount);
		NE = new QuadTree(NEB, maxCount);
		SW = new QuadTree(SWB, maxCount);
		SE = new QuadTree(SEB, maxCount);
		divided = true;
	}

	~QuadTree() {
		if (!divided) return;
		delete NW;
		delete NE;
		delete SW;
		delete SE;
	}
private:
	vector<Point> points;
	Boundary bound;
	uint maxCount;
	QuadTree* NW;
	QuadTree* NE;
	QuadTree* SW;
	QuadTree* SE;
	bool divided;
	float boxOutlineSize = 0.5;
};

