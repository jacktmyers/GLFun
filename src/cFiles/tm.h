#pragma once

#include "primatives/v3.h"
#include "primatives/m33.h"
#include "primatives/m44.h"
#include <string>
#include <list>

class TM{
	public:
		// Variables
		V3* verts, *norms;
		unsigned int* tris;
		int vertsMemN; // Number of verts loaded into memory
		int trisMemN; // Number of tris loaded into memory
		// Functions
		TM(){};
    bool loadFromStl(std::string path);
		bool putInMemory();
		V3 getCenter();
		void translate(V3 tv);
		void setCenter(V3 newCenter);
		void rotateAboutPoint(float yawDeg, float pitchDeg, float rollDeg, V3 point);
		void rotateAboutCenter(float yawDeg, float pitchDeg, float rollDeg);
		M44 normalizeCoordsMat();
		~TM();
	private:
		// Variables
		std::list<V3> triList, colList, normList;
		// Functions
};
