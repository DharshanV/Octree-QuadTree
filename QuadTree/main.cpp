#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include "QuadTree.h"
using namespace std;
using namespace sf;
struct Ray {
public:
	Ray(const Vector2f& orig, const Vector2f& dir) :
		orig(orig), dir(dir) {

	}

	void setDir(const Vector2f& dir) {
		this->dir = dir;
	}

	void draw(RenderWindow& window) {
		Vertex line[2];
		line[0] = Vertex(orig);
		line[1] = Vertex(orig + dir);
		window.draw(line, 2, Lines);
	}
	Vector2f orig, dir;
};

void processEvents(RenderWindow& window);
Vector2f normalize(const Vector2f& v);
bool intersect(const Boundary& box, const Ray &r);

float width = 500;
float height = 500;
float mouseX = 0;
float mouseY = 0;

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 5;
	sf::RenderWindow window(sf::VideoMode(width, height), "QuadTree",sf::Style::Default, settings);

	float size = 30;
	Boundary box(Point(width / 2 - size, height / 2 - size), Point(width / 2 + size, height / 2 + size));
	Vector2f rayPosition(width / 2, height - 10);
	Ray ray(rayPosition,Vector2f(0,0));

	while (window.isOpen()) {
		processEvents(window);

		Vector2f dir = rayPosition - Vector2f(mouseX, mouseY);
		ray.setDir(-dir);
		printf("Intersects: %d\n", intersect(box, ray));

		window.clear();
		box.draw(window);
		ray.draw(window);
		window.display();
	}
	return 0;
}

/*
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
*/

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
	if (event.type == Event::MouseMoved) {
		mouseX = event.mouseMove.x;
		mouseY = event.mouseMove.y;
	}
}

Vector2f normalize(const Vector2f& v) {
	float l = sqrtf(v.x*v.x + v.y*v.y);
	return v / l;
}

bool intersect(const Boundary& box,const Ray &r) {
	Point min = box.topLeft;
	Point max = box.bottomRight;

	float tmin = (min.x - r.orig.x) / r.dir.x;
	float tmax = (max.x - r.orig.x) / r.dir.x;

	if (tmin > tmax) swap(tmin, tmax);

	float tymin = (min.y - r.orig.y) / r.dir.y;
	float tymax = (max.y - r.orig.y) / r.dir.y;

	if (tymin > tymax) swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;
	return true;
}