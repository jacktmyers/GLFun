#pragma once

#include "tm.h"
#include "camera.h"
#include <list>

class Scene{
	public:
		// Variables
		int meshN;
		Camera *camera;
		std::list<TM*> meshes;
		// Functions
		Scene(int width, int height);
		void addTM(TM* tm);
	private:
		// Variables
		// Functions
};
