#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class Line {
public:
	Line(const Vector2f& p1, const Vector2f& p2) :
		p1(p1), p2(p2) {

	}

	void setEnd(const Vector2f& p2) {
		this->p2 = p2;
	}

	void draw(RenderWindow& window) {
		Vertex line[2];
		line[0] = Vertex(p1);
		line[1] = Vertex(p2);
		window.draw(line, 2, Lines);
	}
	Vector2f p1, p2;
};