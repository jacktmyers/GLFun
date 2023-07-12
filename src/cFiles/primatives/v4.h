#pragma once
#include <ostream>
#include <istream>

class V4{
	public:
		// Variables
		// Functions
		V4(){};
		V4(float x, float y, float z, float w);
		float& operator[](int i);
		//~V4();
	private:
		// Variables
		float xyzw[4];
		// Functions
};
