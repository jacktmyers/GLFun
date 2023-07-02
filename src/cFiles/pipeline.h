#ifndef __PIPELINE_H__
	#define __PIPELINE_H__
#include <stdio.h>
#include <string>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_events.h>

#include "shaderUtil.h"

class Pipeline {
	private:
		SDL_Window* gWindow;
		SDL_GLContext gContext;
		GLuint gProgramID;
		GLint gVertexPos2DLocation;
		GLuint gVBO;
		GLuint gIBO;
		int screenWidth;
		int screenHeight;
		ShaderUtil* shaderManager;
		bool live;
		int mouseX, mouseY;
		bool leftMouseButtonDown;

	public:
		bool initialized;

		Pipeline(int screenWidth, int screenHeight);
		bool initSDL(int screenWidth, int screenHeight);
		bool initGL(int screenWidth, int screenHeight);
		void printProgramLog(GLuint program);
		void run();
		void handleEvent(SDL_Event* Event);
		void render(float r, float g, float b);
		void cleanup();
};

#endif
