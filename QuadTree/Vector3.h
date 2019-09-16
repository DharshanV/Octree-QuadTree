#ifndef Vector3_H
#define Vector3_H
#include <iostream>
#include <cmath>
#include <assert.h>
using namespace std;
class Vector3 {
public:
	Vector3() : x(0), y(0), z(0) { }
	Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
	Vector3(float value) : x(value), y(value), z(value) { }
	Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) { }
	void print(ostream& out) const { out << "[" << x << "," << y << "," << z<<"]"; }
	virtual ~Vector3() { }
public:
	inline float normalize() {
		float L = this->length();
		*this /= L;
		return L;
	}

	inline Vector3 getNormalized() {
		Vector3 temp(*this);
		temp /= temp.length();
		return temp;
	}

	inline float length() {
		return this->sqrt(sqr(x) + sqr(y) + sqr(z));
	}

	inline float dot(const Vector3& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	inline Vector3 cross(const Vector3& v) const {
		Vector3 temp;
		temp.x = y * v.z - (v.y* z);
		temp.y = -(x*v.z - (v.x * z));
		temp.z = x * v.y - (v.x*y);
		return temp;
	}

public:
	inline Vector3& operator = (const Vector3& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	inline Vector3& operator += (const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	inline Vector3& operator -= (const Vector3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	inline Vector3& operator /= (float value) {
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}

	inline Vector3& operator *= (float value) {
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	inline float operator [] (int index) {
		if (index == 0) return x;
		if (index == 1) return y;
		if (index == 2) return z;
		assert(!true);
		return 0;
	}

	inline float operator [] (int index) const {
		if (index == 0) return x;
		if (index == 1) return y;
		if (index == 2) return z;
		assert(!true);
		return 0;
	}

	inline Vector3 operator - () {
		Vector3 temp(*this);
		temp *= -1;
		return temp;
	}
private:
	inline float sqr(float value) {
		return value * value;
	}

	inline float sqrt(float value) {
		return sqrtf(value);
	}

public:
	float x, y, z;
};

inline Vector3 operator + (const Vector3& a, const Vector3& b) {
	Vector3 temp(a);
	temp += b;
	return temp;
}

inline Vector3 operator - (const Vector3& a, const Vector3& b) {
	Vector3 temp(a);
	temp -= b;
	return temp;
}

inline Vector3 operator * (const Vector3& a, float value) {
	Vector3 temp(a);
	temp *= value;
	return temp;
}

inline Vector3 operator * (float value, const Vector3& a) {
	Vector3 temp(a);
	temp *= value;
	return temp;
}

inline Vector3 operator / (const Vector3& a, float value) {
	Vector3 temp(a);
	temp /= value;
	return temp;
}

ostream& operator << (ostream& out, const Vector3& a) {
	a.print(out);
	return out;
}

#endif // !Vector3_H