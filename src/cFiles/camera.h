#pragma once

#include "primatives/v3.h"

class Camera{
	public:
		// Variables
		// Functions
		Camera(float hfov, int _w, int _h);
		V3 getVD();
		float getF();
		void setIntrinsics(float near, float far);
		void setExtrinsics();
		void positionAndOrient(V3 newEye, V3 lookAt, V3 up);
		void lookAt(V3 lookAt);
	private:
		// Variables
		int w, h;
		V3 a, b, c, eye;
		float hfov; // Field of view in degrees
		// Functions
};
