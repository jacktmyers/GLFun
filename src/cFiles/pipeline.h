#pragma once

#include <stdio.h>
#include <string>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_events.h>

#include "shaderUtil.h"
#include "scene.h"

class Pipeline {
	private:
		SDL_Window* gWindow;
		SDL_GLContext gContext;
		GLuint gProgramID;
		int screenWidth;
		int screenHeight;
		ShaderUtil* shaderManager;
		bool live;
		int mouseX, mouseY;
		bool leftMouseButtonDown;
		unsigned int* pix;

	public:
		// Variables
		Scene* scene;
		bool initialized;

		// Functions
		Pipeline(int screenWidth, int screenHeight);
		bool initSDL(int screenWidth, int screenHeight);
		bool initGL(int screenWidth, int screenHeight);
		void printProgramLog(GLuint program);
		void run();
		void handleEvent(SDL_Event* Event);
		void render();
		void cleanup();
		Uint32 getpixel(SDL_Surface *surface, int x, int y);
};

