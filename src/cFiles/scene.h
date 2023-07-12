#pragma once

#include "tm.h"
#include "camera.h"
#include <vector>

class Scene{
	public:
		// Variables
		int meshN;
		Camera *camera;
		std::vector<TM*> meshes;
		// Functions
		Scene(int width, int height);
		void addTM(TM* tm);
		~Scene();
	private:
		// Variables
		// Functions
};
