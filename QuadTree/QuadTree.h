#pragma once
#include <iostream>
#include <list>
#include "Boundary.h"
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
		points.reserve(maxCount);
		init();
	}

	void insert(const vec3& point,uint depth = 0) {
		//if it doesn't contain then the point is out of bounds
		if (!bound.contains(point)) return;
		//if there is size then add it
		if (points.size() < maxCount) { 
			points.push_back(point); 
			return;
		}
		//at this point we need to sub divide
		if(!divided) subdivide();
		NW1->insert(point, depth + 1);
		NE1->insert(point, depth + 1);
		SW1->insert(point, depth + 1);
		SE1->insert(point, depth + 1);
		NW2->insert(point, depth + 1);
		NE2->insert(point, depth + 1);
		SW2->insert(point, depth + 1);
		SE2->insert(point, depth + 1);
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
		NW1->searchArea(boundary, searchPoints);
		NE1->searchArea(boundary, searchPoints);
		SW1->searchArea(boundary, searchPoints);
		SE1->searchArea(boundary, searchPoints);
		NW2->searchArea(boundary, searchPoints);
		NE2->searchArea(boundary, searchPoints);
		SW2->searchArea(boundary, searchPoints);
		SE2->searchArea(boundary, searchPoints);
	}

	//void subdivide() {
	//	Boundary NWB, NEB, SWB, SEB;
	//	float dw = fabs(bound.topLeft.x - bound.bottomRight.x) / 2;
	//	float dh = fabs(bound.topLeft.y - bound.bottomRight.y) / 2;
	//	NWB = Boundary(bound.topLeft, Point(bound.topLeft.x + dw, bound.topLeft.y + dh));
	//	NEB = Boundary(Point(bound.topLeft.x + dw, bound.topLeft.y), Point(bound.topLeft.x + dw * 2, bound.topLeft.y + dh));
	//	SWB = Boundary(Point(bound.topLeft.x, bound.topLeft.y + dh), Point(bound.topLeft.x + dw, bound.topLeft.y + dh * 2));
	//	SEB = Boundary(Point(bound.topLeft.x + dw, bound.topLeft.y + dh), bound.bottomRight);
	//	NW = new QuadTree(NWB, maxCount);
	//	NE = new QuadTree(NEB, maxCount);
	//	SW = new QuadTree(SWB, maxCount);
	//	SE = new QuadTree(SEB, maxCount);
	//	divided = true;
	//}

	void subdivide() {
		Boundary NWB1, NEB1, SWB1, SEB1;
		Boundary NWB2, NEB2, SWB2, SEB2;
		float dl = bound.getLength() / 2;
		float dw = bound.getWidth() / 2;
		float dh = bound.getHeight() / 2;
		vec3 topLeft, bottomRight;
		topLeft = bound.getTopLeft();
		bottomRight = bound.getBottomRight();

		NWB1 = Boundary(topLeft, vec3(topLeft.x + dl, topLeft.y - dh, topLeft.z - dw));
		NEB1 = Boundary(vec3(topLeft.x+dl,topLeft.y,topLeft.z),vec3(topLeft.x+dl*2, topLeft.y-dh, topLeft.z-dw));
		SWB1 = Boundary(vec3(topLeft.x, topLeft.y-dh, topLeft.z),vec3(topLeft.x+dl, topLeft.y-dh*2, topLeft.z-dw));
		SEB1 = Boundary(vec3(topLeft.x+dl, topLeft.y-dh, topLeft.z), vec3(topLeft.x + dl *2, topLeft.y - dh * 2, topLeft.z - dw));
		NW1 = new QuadTree(NWB1, maxCount);
		NE1 = new QuadTree(NEB1, maxCount);
		SW1 = new QuadTree(SWB1, maxCount);
		SE1 = new QuadTree(SEB1, maxCount);

		NWB2 = Boundary(vec3(topLeft.x, topLeft.y, topLeft.z - dw), vec3(topLeft.x + dl, topLeft.y - dh, topLeft.z - dw *2));
		NEB2 = Boundary(vec3(topLeft.x + dl, topLeft.y, topLeft.z - dw), vec3(topLeft.x + dl * 2, topLeft.y - dh, topLeft.z - dw * 2));
		SWB2 = Boundary(vec3(topLeft.x, topLeft.y - dh, topLeft.z - dw), vec3(topLeft.x + dl, topLeft.y - dh * 2, topLeft.z - dw * 2));
		SEB2 = Boundary(vec3(topLeft.x + dl, topLeft.y - dh, topLeft.z - dw), vec3(topLeft.x + dl * 2, topLeft.y - dh * 2, topLeft.z - dw * 2));
		NW2 = new QuadTree(NWB2, maxCount);
		NE2 = new QuadTree(NEB2, maxCount);
		SW2 = new QuadTree(SWB2, maxCount);
		SE2 = new QuadTree(SEB2, maxCount);

		divided = true;
	}

	void draw(uint depth = 0) {
		//if(depth == 1) 
		bound.draw();
		if (!divided) return;
		NW1->draw(depth + 1);
		NE1->draw(depth + 1);
		SW1->draw(depth + 1);
		SE1->draw(depth + 1);
		NW2->draw(depth + 1);
		NE2->draw(depth + 1);
		SW2->draw(depth + 1);
		SE2->draw(depth + 1);
	}

	const Boundary* getBound() const {
		return &bound;
	}

	const vector<vec3>* getPoints() const {
		return &points;
	}

	//const vector<Triangle>* getTriangles() const {
	//	return &triangles;
	//}

	bool subdivided() const{
		return divided;
	}

	QuadTree* getNW1() const { return NW1; };
	QuadTree* getNE1() const { return NE1; };
	QuadTree* getSW1() const { return SW1; };
	QuadTree* getSE1() const { return SE1; };
	QuadTree* getNW2() const { return NW2; };
	QuadTree* getNE2() const { return NE2; };
	QuadTree* getSW2() const { return SW2; };
	QuadTree* getSE2() const { return SE2; };
	 
	~QuadTree() {
		if (!divided) return;
		delete NW1;
		delete NE1;
		delete SW1;
		delete SE1;
		delete NW2;
		delete NE2;
		delete SW2;
		delete SE2;
	}

private:
	void init() {
		divided = false;
		NW1 = NULL;
		NE1 = NULL;
		SW1 = NULL;
		SE1 = NULL;
		NW2 = NULL;
		NE2 = NULL;
		SW2 = NULL;
		SE2 = NULL;
	}

private:
	vector<vec3> points;
	Boundary bound;
	uint maxCount;
	QuadTree* NW1;
	QuadTree* NE1;
	QuadTree* SW1;
	QuadTree* SE1;
	QuadTree* NW2;
	QuadTree* NE2;
	QuadTree* SW2;
	QuadTree* SE2;
	bool divided;
	float boxOutlineSize = 0.5;
};

