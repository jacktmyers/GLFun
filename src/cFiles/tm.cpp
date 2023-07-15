#include "tm.h"
#include <math.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <list>
#include "../externals/stl_reader/stl_reader.h"

TM::TM(){
	verts=norms= NULL;	
	tris= NULL;
	vertsMemN=trisMemN= 0;
}

TM::~TM(){
	vertVec.clear();	
	normVec.clear();	
	triVec.clear();	
	solidVec.clear();	
	if(verts != NULL){
		delete[] verts;
	}
	if(norms != NULL){
		delete[] norms;
	}
}

bool TM::loadFromStl(std::string path){
	stl_reader::ReadStlFile (path.c_str(), vertVec, normVec, 
			triVec, solidVec);
	if(!putInMemory()){
		return false;
	}
	return true;
}

// Load into arrays triangle data of the mesh
bool TM::putInMemory() {
	verts = new V3[vertsMemN= vertVec.size()/3];
	for(long unsigned int i = 0; i<vertsMemN; i++){
		verts[i] = V3(vertVec[i*3],vertVec[i*3+1],vertVec[i*3+2]);
	}
	tris = new unsigned int[trisMemN= triVec.size()];
	for(long unsigned int i = 0; i<trisMemN; i++){
		tris[i] = triVec[i];
	}
	norms = new V3[normsMemN= normVec.size()/3];
	for(long unsigned int i = 0; i<normsMemN; i++){
		norms[i] = V3(normVec[i*3],normVec[i*3+1],normVec[i*3+2]);
	}
	return true;
}

// Get the center of the object
V3 TM::getCenter(){
	V3 ret(0.0f, 0.0f, 0.0f);
	for (int vi = 0; vi < vertsMemN; vi++)
		ret = ret + verts[vi];
	ret = ret / (float)vertsMemN;
	return ret;
}

// Set the center of the object
void TM::setCenter(V3 newCenter){
	V3 center = getCenter();
	V3 tv = newCenter - center;
	translate(tv);
}

// Move the mesh by a vector
void TM::translate(V3 tv){
	for (int vi = 0; vi < vertsMemN; vi++)
		verts[vi] = verts[vi] + tv;
}

// Rotate about center with axis aligned with the world axis
void TM::rotateAboutCenter(float yawDeg, float pitchDeg, float rollDeg) {
	rotateAboutPoint(yawDeg, pitchDeg, rollDeg, getCenter());
}

// Rotate about a point with axis aligned with the world axis
void TM::rotateAboutPoint(float yawDeg, float pitchDeg, float rollDeg, V3 point) {
	M33 yawMat, pitchMat, rollMat;
	yawMat.setAsRotationAboutAxis('z', yawDeg);
	pitchMat.setAsRotationAboutAxis('y', pitchDeg);
	rollMat.setAsRotationAboutAxis('x', rollDeg);
	M33 rotMat = yawMat*pitchMat*rollMat;
	for (int vi = 0; vi < vertsMemN; vi++) {
		verts[vi] = (rotMat*(verts[vi]-point))+point;
	}
	for (int ni = 0; ni < (vertsMemN/3); ni++){
		norms[ni] = (rotMat*(norms[ni]-point))+point;
	}
}

// Normalize XYZ coords matrix
M44 TM::normalizeCoordsMat(){
	float x0,x1,y0,y1,z0,z1;
	x0=x1= verts[0][0];
	y0=y1= verts[0][1];
	z0=z1= verts[0][2];

	// Get the largest and smallest coords
	for(int vi=1;	vi < vertsMemN; vi++){
		V3 currVert = verts[vi];
		if(currVert[0] > x1){
			x1 = currVert[0];
		}else if(currVert[0] < x0){
			x0 = currVert[0];
		}

		if(currVert[1] > y1){
			y1 = currVert[1];
		}else if(currVert[1] < y0){
			y0 = currVert[1];
		}

		if(currVert[2] > z1){
			z1 = currVert[2];
		}else if(currVert[2] < z0){
			z0 = currVert[2];
		}
	}

	// Create normalizing matrix
	return M44(V4(1/(x1-x0),0,0,-x0/(x1-x0)),
						 V4(0,1/(y1-y0),0,-y0/(y1-y0)),
						 V4(0,0,1/(z1-z0),-z0/(z1-z0)),
						 V4(0,0,0,1));
}
