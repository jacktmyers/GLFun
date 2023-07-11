#pragma once

#include <string>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_events.h>

class ShaderUtil {
	public:
		bool attatchShader(GLuint gProgramID, GLenum shaderType, std::string path);
		void printShaderLog(GLuint shader);
		void printProgramLog(GLuint program);
};
