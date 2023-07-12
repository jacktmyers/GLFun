#include "tm.h"
#include <math.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <list>

bool TM::loadFromStl(std::string path){
  std::ifstream meshFile; 
	
	// Check if open
	meshFile.open(path);
	if(!meshFile.is_open()){
		printf("Could not open %s\n", path.c_str());
		return false;
	}

	// Read the last line from the file
	meshFile.seekg(-2,std::ios::end);  
	bool keepLooping = true;
	while(keepLooping) {
		char ch;
		meshFile.get(ch);
		if((int)meshFile.tellg() <= 1) {
			meshFile.seekg(0);
			keepLooping = false;
		}
		else if(ch == '\n') {
			keepLooping = false;
		}
		else {
			meshFile.seekg(-2,std::ios::cur);
		}
	}
	std::string lastLine;
	getline(meshFile,lastLine); 

	// Last line is not "endsolid model"
	if(lastLine.compare(0,8,"endsolid") != 0){
		printf("STL file does not fit format\n");
		meshFile.close();
		return false;
	}

	// Go to the beginning of the file
	meshFile.seekg (0, std::ios::beg);

	// Start reading
	std::string currLine;	
	bool collectingTri = false;
	bool inLoop = false;
	float f0, f1, f2;
	int vertsN = 0;
	for(;;){
		getline(meshFile,currLine);
		if(inLoop){
			if(currLine.compare(0,7,"endloop") == 0){
				if(vertsN != 3){
					printf("Error in STL reading\n");
					meshFile.close();
					return false;
				}
				inLoop = false;
				vertsN = 0;
			}else{
				// This needs some regex checking before hand to be secure
				sscanf(currLine.c_str(), "vertex %f %f %f", &f0, &f1, &f2);
				triList.push_back(V3(f0,f1,f2));
				vertsN++;
			}
		}else if(collectingTri){
			if(currLine.compare(0,10,"outer loop") == 0){
				inLoop = true;
			}else if(currLine.compare(0,8,"endfacet") == 0){
				collectingTri = false;
			}else{
				printf("Error in STL reading\n");
				meshFile.close();
				return false;
			}
		}else if(currLine.compare(0,12,"facet normal") == 0){
			collectingTri = true;
			// This needs some regex checking before hand to be secure
			sscanf(currLine.c_str(), "facet normal %f %f %f", &f0, &f1, &f2);
			normList.push_back(V3(f0,f1,f2));
		}
		if(currLine.compare(0,8,"endsolid") == 0){
			break;
		}
	}
	// Close the file
	meshFile.close();
	if(!putInMemory()){
		return false;
	}
	return true;
}

// Load into arrays triangle data of the mesh
bool TM::putInMemory() {
	std::list<V3> added;
	// Create the space for the triangle info
	tris = new unsigned int[triList.size()];
	trisMemN = triList.size();
	int triI = 0;
	V3 currVert;
	// Loop through triangle list
	for (auto mainIt = triList.begin(); mainIt != triList.end(); ++mainIt){
		currVert = *mainIt;	
		int index = 0;
		bool found = false;
		// Search to see if vertex was used before
		for (auto searchIt = added.begin(); searchIt != added.end(); ++searchIt){
			if(currVert == *searchIt){
				found = true;
				break;
			}
			index++;
		}
		// Add vertex to file (if necessary) and add tri
		if(found){
			tris[triI++] = index;
		}else{
			added.push_back(currVert);
			tris[triI++] = index;
		}
	}
	// Convert added list into array
	verts = new V3[added.size()];
	vertsMemN = added.size();
	int index = 0;
	for (auto mainIt = added.begin(); mainIt != added.end(); ++mainIt){
		verts[index++] = *mainIt;	
	}
	// Convert norm list into array
	norms = new V3[normList.size()];
	index = 0;
	for (auto mainIt = normList.begin(); mainIt != normList.end(); ++mainIt){
		norms[index++] = *mainIt;	
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
