#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500
#include <stdio.h>
#include "pipeline.h"
#include "tm.h"

int main(int argc, char** argv){
	Pipeline* pipeline = new Pipeline(SCREEN_WIDTH, SCREEN_HEIGHT);
	// Load our meshes into a scene
	TM* cube = new TM();
	cube->loadFromStl("./stls/cube.stl");
	cube->setCenter(V3(0,0,-20));
	pipeline->scene->addTM(cube);
	// Look at the cube
	pipeline->scene->camera->positionAndOrient(V3(10,10,10), V3(0,0,-20), V3(0,1,0));

	// Check if initialized
	if(!pipeline->initialized){
		printf("Pipeline not initialized! Exiting\n");
		return -1;
	}
	
	pipeline->run();
	printf("Exiting\n");
	return 0;
}
