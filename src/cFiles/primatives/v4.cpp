#include "v4.h"

V4::V4(float x, float y, float z, float w){
	xyzw[0] = x;	
	xyzw[1] = y;	
	xyzw[2] = z;	
	xyzw[3] = w;	
}
float& V4::operator[](int i) {
	//This returns & so that it can be edited later
	return xyzw[i];
}
