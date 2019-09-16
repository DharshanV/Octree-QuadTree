#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
using namespace std;
using namespace sf;

float width = 500;
float height = 500;
float mouseX = 0;
float mouseY = 0;

void processEvents(Window& window);

int main() {
	ContextSettings settings;
	settings.antialiasingLevel = 5;
	sf::Window window(sf::VideoMode(width, height), "Qctree", Style::Default, settings);
	window.setActive(true);
	window.setFramerateLimit(60);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	while (window.isOpen()) {
		processEvents(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		window.display();
	}
	return 0;
}

void processEvents(Window& window) {
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
	if (event.type == Event::Resized) {
		glViewport(0, 0, event.size.width, event.size.height);
	}
}