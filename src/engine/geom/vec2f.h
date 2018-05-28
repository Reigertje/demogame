#ifndef VEC2F_H
#define VEC2F_H

#include <math.h>

struct vec2f {

	float x;
	float y;

	vec2f() : x(0), y(0) {};

	vec2f(float x, float y) : x(x), y(y) {};

	void set(float nx, float ny) {
		x = nx;
		y = ny;
	}

	void set(const vec2f& v) {
		x = v.x;
		y = v.y;
	}

	float length() const {
		return sqrtf(squaredLength());
	}

	float squaredLength() const {
		return x * x + y * y;
	}

	void normalize() {
		float l = length();
		if (l != 0) {
			x /= l; 
			y /= l; 
		} else {
			x = 0;
			y = 0; 
		}
	}

	vec2f& operator=(const vec2f& v) {
		set(v);
		return *this;
	}
	
	vec2f operator+(const vec2f& v) {
		return vec2f(x + v.x, y + v.y);
	}

	vec2f operator-(const vec2f& v) {
		return vec2f(x - v.x, y - v.y);
	}
	
	vec2f& operator+=(const vec2f& v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	vec2f& operator-=(const vec2f& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}

	vec2f operator*(float f) {
		return vec2f(x * f, y * f);
	}

	vec2f operator/(float f) {
		return vec2f(x / f, y / f);
	}

	vec2f& operator*=(float f) {
		x *= f;
		y *= f;
		return *this;
	}

	vec2f& operator/=(float f) {
		x /= f;
		y /= f;
		return *this;
	}

};

#endif