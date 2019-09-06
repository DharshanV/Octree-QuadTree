#include <SFML/Graphics.hpp>
#include <iostream>
#include "QuadTree.h"
using namespace std;
using namespace sf;
void processEvents(RenderWindow& window);
float random(float min, float max);
sf::CircleShape getPoint(const Point& point);

uint width = 500;
uint height = 500;
QuadTree tree(Boundary(Point(5, 5), Point(width - 5, height - 5)), 4);
Boundary searchArea;
RectangleShape searchBox;

int main() {
	srand(time(0));
	sf::RenderWindow window(sf::VideoMode(width, height), "QuadTree");

	for (int i = 0; i < 1000; i++) {
		tree.insert(Point(random(5, width - 5), random(5, height - 5)));
	}

	searchBox.setFillColor(Color::Transparent);
	searchBox.setOutlineThickness(2);
	searchBox.setOutlineColor(Color::Green);
	searchBox.setSize(Vector2f(50,50));
	searchBox.setPosition(50, 50);

	list<Point> query;
	while (window.isOpen()) {
		tree.searchArea(searchArea,query);

		processEvents(window);
		window.clear();
		tree.draw(window);
		for (Point& p : query) {
			window.draw(getPoint(p));
		}
		query.clear();
		window.draw(searchBox);
		window.display();
	}
	return 0;
}

float random(float min, float max) {
	return (float)(rand() % (int)max + min);
}

void processEvents(RenderWindow& window) {
	sf::Event event;
	window.pollEvent(event);
	if (event.type == Event::Closed) {
		window.close();
	}
	if (event.type == Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::Escape) {
			window.close();
		}
	}
	if (event.type == Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			
		}
	}
	if (event.type == Event::MouseMoved) {
		float boxSizeX = searchBox.getSize().x/2;
		float boxSizeY = searchBox.getSize().y/2;
		searchBox.setPosition(event.mouseMove.x - boxSizeX, event.mouseMove.y-boxSizeY);
		searchArea = Boundary(Point(event.mouseMove.x - boxSizeX, event.mouseMove.y - boxSizeY),Point(event.mouseMove.x + boxSizeX, event.mouseMove.y + boxSizeY));
	}
}

sf::CircleShape getPoint(const Point& point) {
	sf::CircleShape circle(4);
	circle.setOrigin(-sf::Vector2f(point.x - 4, point.y - 4));
	circle.setFillColor(sf::Color::Green);
	return circle;
}