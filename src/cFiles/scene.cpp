#include "scene.h"
#include "camera.h"

Scene::~Scene(){
	meshes.clear();
	delete camera;
}

Scene::Scene(int width, int height){
	meshN = 0;
	camera = new Camera(90,width,height);		
}

void Scene::addTM(TM* tm){
	meshes.push_back(tm);
	meshN++;
}
