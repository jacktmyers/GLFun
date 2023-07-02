#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500
#include <stdio.h>
#include "pipeline.h"

int main(int argc, char** argv){
	Pipeline* pipeline = new Pipeline(SCREEN_WIDTH, SCREEN_HEIGHT);
	if(!pipeline->initialized){
		printf("Pipeline not initialized! Exiting\n");
		return -1;
	}
	pipeline->run();
	printf("Exiting\n");
	return 1;
}
