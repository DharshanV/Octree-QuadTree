#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include "QuadTree.h"
using namespace std;
using namespace sf;
void processEvents(RenderWindow& window);
float random(float min, float max);
void createParticle(vector<Vertex>& vertex, float x, float y, float size, Color color);
void getParticles(const list<Point>& points, vector<Vertex>& vertexArray, float particleSize, Color color);
Boundary getSearchArea(const RectangleShape& box);

uint width = 700;
uint height = 700;
const uint nParticles = 50000;
QuadTree tree(Boundary(Point(5, 5), Point(width - 5, height - 5)), 35);
Boundary searchArea;
RectangleShape searchBox;
bool renderBoxes = false;

int main() {
	srand(time(0));
	sf::RenderWindow window(sf::VideoMode(width, height), "QuadTree");

	float particleSize = 1;
	list<Point> searchParticles;
	vector<Vertex> particlePosition;
	particlePosition.reserve(nParticles * 4);
	for (int i = 0; i < nParticles; i++) {
		float x = random(5, width - 5);
		float y = random(5, height - 5);
		tree.insert(Point(x, y));
		createParticle(particlePosition, x, y, particleSize, Color::Red);
	}

	searchBox.setOutlineColor(Color::Green);
	searchBox.setFillColor(Color::Transparent);
	searchBox.setOutlineThickness(2);
	searchBox.setSize(Vector2f(50,50));
	searchBox.setPosition(width/2, height/2);

	while (window.isOpen()) {
		searchArea = getSearchArea(searchBox);
		tree.searchArea(searchArea, searchParticles);

		processEvents(window);

		window.clear();
		window.draw(searchBox);

		//draw all particles
		window.draw(&particlePosition[0], particlePosition.size(), Quads);

		//draw all selected particles
		vector<Vertex> particleVertex;
		getParticles(searchParticles, particleVertex, particleSize,Color::Green);
		if(particleVertex.size() != 0)
		window.draw(&particleVertex[0], particleVertex.size(), Quads);

		//draw quadtree
		vector<Vertex> boxVertex;
		if (renderBoxes) {
			tree.getBoxs(boxVertex, Color(255,255,255));
			if (boxVertex.size() != 0)
				window.draw(&boxVertex[0], boxVertex.size(), Lines);
		}

		window.display();
		searchParticles.clear();
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
		if (event.key.code == sf::Keyboard::Num9) {
			renderBoxes = true;
		}
		if (event.key.code == sf::Keyboard::Num0) {
			renderBoxes = false;
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
	}
}

Boundary getSearchArea(const RectangleShape& box) {
	float boxSizeX = box.getSize().x;
	float boxSizeY = box.getSize().y;
	float x = box.getPosition().x;
	float y = box.getPosition().y;
	return Boundary(Point(x ,y), Point(x + boxSizeX,y + boxSizeY));
}

void createParticle(vector<Vertex>& vertex, float x, float y, float size, Color color) {
	vertex.push_back(Vertex(Vector2f(x - size, y - size), color));
	vertex.push_back(Vertex(Vector2f(x + size, y - size), color));
	vertex.push_back(Vertex(Vector2f(x + size, y + size), color));
	vertex.push_back(Vertex(Vector2f(x - size, y + size), color));
}

void getParticles(const list<Point>& points,vector<Vertex>& vertexArray,float particleSize, Color color) {
	for (const Point& p : points) {
		createParticle(vertexArray, p.x, p.y, particleSize, color);
	}
}