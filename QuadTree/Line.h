#ifndef LINE_H
#define LINE_H
#include <iostream>
#include "Vector3.h"
using namespace std;
class Line {
public:
	Line(const Vector3& p1, const Vector3& p2) :
		p1(p1), p2(p2) {

	}

	void setEnd(const Vector3& p2) {
		this->p2 = p2;
	}

	void draw() {
	}
private:
	Vector3 p1, p2;
};
#endif // !LINE_H