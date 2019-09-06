#include <SFML/Graphics.hpp>
#include <iostream>
#include "QuadTree.h"
using namespace std;
using namespace sf;
void processEvents(RenderWindow& window);

int main() {
	uint width = 700;
	uint height = 700;
	sf::RenderWindow window(sf::VideoMode(700, 500), "QuadTree");
	QuadTree tree(Boundary(Point(0, 0), Point(width, height)),4);

	while (window.isOpen()) {
		processEvents(window);


		window.clear();
		tree.draw(&window);
		window.display();
	}
	return 0;
}

void processEvents(RenderWindow& window) {
	sf::Event event;
	while (window.pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			window.close();
			break;
		case Event::KeyReleased: {
			if (event.key.code == sf::Keyboard::Escape) {
				window.close();
			}
			break;
		}
		default:
			break;
		}
	}

}