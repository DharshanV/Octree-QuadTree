#include "Utilites.h"
using namespace std;

float width = 500;
float height = 500;
float mouseX = 0;
float mouseY = 0;
bool renderBoxes = false;
const uint nParticles = 25000;

QuadTree tree(Boundary(Point(5, 5), Point(width - 5, height - 10)), 6);
Vector2f lineStart(width / 2, height);
Vector2f lineEnd(width / 2, height);

void processEvents(RenderWindow& window);

int main() {
	srand(time(0));
	sf::RenderWindow window(sf::VideoMode(width, height), "QuadTree", Style::Default,ContextSettings(0,0,5));
	window.setFramerateLimit(60);

	Line line(lineStart, lineEnd);

	float particleSize = 1;
	list<Point> searchParticles;
	vector<Vertex> particlePosition;
	particlePosition.reserve(nParticles * 4);
	for (int i = 0; i < nParticles; i++) {
		float x = random(5, width - 10);
		float y = random(5, height - 20);
		tree.insert(Point(x, y));
		createParticle(particlePosition, x, y, particleSize, Color::Red);
	}

	while (window.isOpen()) {
		processEvents(window);

		lineEnd = Vector2f(mouseX, mouseY);
		line.setEnd(lineEnd);

		window.clear();

		//draw all particles
		if (particlePosition.size() != 0)
			window.draw(&particlePosition[0], particlePosition.size(), Quads);

		//draw all selected particles
		vector<Vertex> particleVertex;
		getSelectedParticles(tree,line, particleVertex, particleSize+1, Color::Green);
		if (particleVertex.size() != 0)
			window.draw(&particleVertex[0], particleVertex.size(), Quads);

		line.draw(window);
		window.display();
	}
	return 0;
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
		if (event.key.code == sf::Keyboard::Num9) {
			renderBoxes = true;
		}
		if (event.key.code == sf::Keyboard::Num0) {
			renderBoxes = false;
		}
	}
	if (event.type == Event::MouseMoved) {
		mouseX = event.mouseMove.x;
		mouseY = event.mouseMove.y;
	}
}