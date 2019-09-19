#pragma once
#include <iostream>
#include <list>
#include "Boundary.h"
#include "Line.h"
typedef unsigned int uint;
using namespace std;
using namespace glm;
class QuadTree {
public:
	QuadTree() {
		init();
	}

	QuadTree(const Boundary& bound, uint maxCount) :
		bound(bound),maxCount(maxCount) {
		init();
	}

	void insert(const vec3& point,uint depth = 0) {
		//if it doesn't contain then the point is out of bounds
		if (!bound.contains(point)) return;
		//if there is size then add it
		if (points.size() < maxCount || depth >= 20) { 
			points.push_back(point); 
			return;
		}
		//at this point we need to sub divide
		if(!divided) subdivide();
		for (QuadTree* tree : subtrees) {
			tree->insert(point, depth + 1);
		}
	}

	//First check if triangle is inside the node box. If it does,
	//then subdivide until to insert triangle into the lowest node.
	//If it doesn't insert into its child, then insert into the root's
	//data.
	//bool insert(const Triangle& triangle, uint depth = 0) {
	//	//if (!triangle.isInside(bound) || depth >= 10) return false;
	//	//if (!divided) subdivide();
	//	////insert into root if no nodes can support it
	//	//if (!(NW->insert(triangle, depth + 1) || NE->insert(triangle, depth + 1) ||
	//	//	SW->insert(triangle, depth + 1) || SE->insert(triangle, depth + 1))) {
	//	//	triangles.push_back(triangle);
	//	//}
	//	return true;
	//}

	void searchArea(const Boundary& boundary, vector<vec3>& searchPoints) {
		if (!bound.intersects(boundary)) return;
		for (vec3& p : points) {
			if(boundary.contains(p)) searchPoints.push_back(p);
		}
		if (!divided) return;
		for (QuadTree* tree : subtrees) {
			tree->searchArea(boundary, searchPoints);
		}
	}

	void searchArea(const Line& line, vector<vec3>& searchPoints) {
		if (!bound.intersects(line)) return;
		for (vec3& p : points) {
			if (line.contains(p)) searchPoints.push_back(p);
		}
		if (!divided) return;
		for (QuadTree* tree : subtrees) {
			tree->searchArea(line, searchPoints);
		}
	}

	void subdivide() {
		Boundary boundarys[8];
		float dl = bound.getLength() / 2;
		float dw = bound.getWidth() / 2;
		float dh = bound.getHeight() / 2;
		vec3 topLeft = bound.getTopLeft();
		vec3 bottomRight = bound.getBottomRight();

		boundarys[0] = Boundary(topLeft, vec3(topLeft.x + dl, topLeft.y - dh, topLeft.z - dw));
		boundarys[1] = Boundary(vec3(topLeft.x + dl, topLeft.y, topLeft.z), vec3(topLeft.x + dl * 2, topLeft.y - dh, topLeft.z - dw));
		boundarys[2] = Boundary(vec3(topLeft.x, topLeft.y - dh, topLeft.z), vec3(topLeft.x + dl, topLeft.y - dh * 2, topLeft.z - dw));
		boundarys[3] = Boundary(vec3(topLeft.x + dl, topLeft.y - dh, topLeft.z), vec3(topLeft.x + dl * 2, topLeft.y - dh * 2, topLeft.z - dw));
		boundarys[4] = Boundary(vec3(topLeft.x, topLeft.y, topLeft.z - dw), vec3(topLeft.x + dl, topLeft.y - dh, topLeft.z - dw * 2));
		boundarys[5] = Boundary(vec3(topLeft.x + dl, topLeft.y, topLeft.z - dw), vec3(topLeft.x + dl * 2, topLeft.y - dh, topLeft.z - dw * 2));
		boundarys[6] = Boundary(vec3(topLeft.x, topLeft.y - dh, topLeft.z - dw), vec3(topLeft.x + dl, topLeft.y - dh * 2, topLeft.z - dw * 2));
		boundarys[7] = Boundary(vec3(topLeft.x + dl, topLeft.y - dh, topLeft.z - dw), vec3(topLeft.x + dl * 2, topLeft.y - dh * 2, topLeft.z - dw * 2));
		
		for (int i = 0; i < 8; i++) {
			subtrees[i] = new QuadTree(boundarys[i], maxCount);
		}
		divided = true;
	}

	void draw(uint depth = 0) {
		bound.draw();
		if (!divided) return;
		for (QuadTree* tree : subtrees) {
			tree->draw(depth + 1);
		}
	}

	const Boundary* getBound() const {
		return &bound;
	}

	const list<vec3>* getPoints() const {
		return &points;
	}

	//const vector<Triangle>* getTriangles() const {
	//	return &triangles;
	//}

	bool subdivided() const{
		return divided;
	}

	~QuadTree() {
		if (!divided) return;
		for (QuadTree* tree : subtrees) {
			delete tree;
		}
	}

private:
	void init() {
		divided = false;
		for (QuadTree* tree : subtrees) {
			tree = NULL;
		}
	}

private:
	list<vec3> points;
	Boundary bound;
	uint maxCount;
	QuadTree* subtrees[8];
	bool divided;
};

