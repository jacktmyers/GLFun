#include "v3.h"
#include "math.h"
#include <ostream>
#include <istream>
#include <iostream>


V3::V3(float x, float y, float z) {
	xyz[0] = x;
	xyz[1] = y;
	xyz[2] = z;
}

V3 V3::operator+(V3 v1) {
	V3 ret;
	V3& v0 = *this;
	ret[0] = v0[0] + v1[0];
	ret[1] = v0[1] + v1[1];
	ret[2] = v0[2] + v1[2];
	return ret;
}

V3 V3::operator-(V3 v1) {
	V3 ret;
	V3& v0 = *this;
	ret[0] = v0[0] - v1[0];
	ret[1] = v0[1] - v1[1];
	ret[2] = v0[2] - v1[2];
	return ret;
}

V3 V3::operator*(float scale) {
	V3& v0 = *this;
	V3 ret;
	ret[0] = v0[0] * scale;
	ret[1] = v0[1] * scale;
	ret[2] = v0[2] * scale;
	return ret;
}

V3 V3::operator/(float scale) {
	V3& v0 = *this;
	V3 ret;
	ret[0] = v0[0] / scale;
	ret[1] = v0[1] / scale;
	ret[2] = v0[2] / scale;
	return ret;
}

bool V3::operator==(V3 other) {
	V3 &curr = *this;
	if (curr[0] != other[0])
		return false;
	if (curr[1] != other[1])
		return false;
	if (curr[2] != other[2])
		return false;
	return true;
}

V3 V3::normalize() {
	V3& v0 = *this;
	V3 ret;
	float len = v0.length();
	ret = v0 / len;
	return ret;
}

float V3::length() {
	V3& v0 = *this;
	return sqrt(v0 * v0);
}

// Cross product
V3 V3::operator^(V3 v1) {
	V3& v0 = *this;
	V3 ret;
	ret[0] = (v0[1] * v1[2]) - (v1[1] * v0[2]);
	ret[1] = -1 * ((v0[0] * v1[2]) - (v1[0] * v0[2]));
	ret[2] = (v0[0] * v1[1]) - (v1[0] * v0[1]);
	return ret;
}

float& V3::operator[](int i) {
	//This returns & so that it can be edited later
	return xyz[i];
}

float V3::operator*(V3 v1) {
	V3& v0 = *this;
	return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
}

std::ostream& operator<<(std::ostream& ostr, V3 v) {
	return ostr << "[" << v[0] << " " << v[1] << " " << v[2] << "]";
}

std::istream& operator>>(std::istream& istr, V3& v) {
	std::cerr << "Enter x:" << std::endl;
	istr >> v[0];
	std::cerr << "Enter y:" << std::endl;
	istr >> v[1];
	std::cerr << "Enter z:" << std::endl;
	istr >> v[2];
	return istr;
}
