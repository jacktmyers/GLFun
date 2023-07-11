#pragma once

#include "v3.h"
#include <istream>

class M33{
	public:
		// Variables
		// Functions
		M33();
		M33(V3 r0, V3 r1, V3 r2);
		void setColumn(int,V3);
		V3 getColumn(int);
		void setRow(int,V3);
		void setAsRotationAboutAxis(char, float);
		V3& operator[](int i);
		friend std::istream& operator>>(std::istream& istr, M33& m);
		V3 operator*(V3 v);
		M33 operator*(M33 m1);
		M33 operator*(float f);
	private:
		// Variables
		V3 rows[3];
		// Functions
};
