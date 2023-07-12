#pragma once
#include <ostream>
#include <istream>

class V3{
	public:
		// Variables
		// Functions
		V3(){};
		V3(float x, float y, float z);
		V3 operator+(V3 v1);
		float operator*(V3 v1);
		float& operator[](int i);
		float length();
		V3 operator^(V3 v1);
		V3 normalize();
		V3 operator-(V3 v1);
		V3 operator*(float scale);
		V3 operator/(float scale);
		bool operator==(V3 other);
		friend std::ostream& operator<<(std::ostream& ostr, V3 v);
		friend std::istream& operator>>(std::istream& istr,V3& v);
		//~V3();
	private:
		// Variables
		float xyz[3];
		// Functions
};
