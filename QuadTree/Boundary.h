#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Point.h"
using namespace std;
using namespace sf;
class Boundary {
public:
	Boundary() :topLeft(0, 0), bottomRight(0, 0) {}
	Boundary(const Point& topLeft, const Point& bottomRight) :
		topLeft(topLeft), bottomRight(bottomRight) {

	}

	bool contains(const Point& point) const {
		if ((point.x >= topLeft.x && point.x <= bottomRight.x) &&
			(point.y >= topLeft.y && point.y <= bottomRight.y)) {
			return true;
		}
		return false;
	}

	bool contains(const Vector2f& point) const {
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

	void draw(sf::RenderWindow& window) {
		float sizeX = fabs(topLeft.x - bottomRight.x);
		float sizeY = fabs(topLeft.y - bottomRight.y);
		sf::RectangleShape box(sf::Vector2f(sizeX, sizeY));
		box.setPosition(topLeft.x, topLeft.y);
		box.setFillColor(sf::Color::Transparent);
		box.setOutlineThickness(2);
		window.draw(box);
	}

	Point topLeft;
	Point bottomRight;
};