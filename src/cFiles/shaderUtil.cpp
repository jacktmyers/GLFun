#include "shaderUtil.h"
#include <sstream>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>

bool ShaderUtil::attatchShader(GLuint gProgramID, GLenum shaderType, std::string path){
	std::ifstream shaderFile;
	std::string shaderString;

	shaderFile.open(path);
	if(!shaderFile.is_open()){
		printf("Could not open %s\n", path.c_str());
		return false;
	}
	while(shaderFile){
		shaderString.push_back((char) shaderFile.get());
	}
	shaderFile.close();
	const GLchar* shaderSourceArray[] = {
		shaderString.c_str()
	};
	GLuint gShader = glCreateShader(shaderType);
	glShaderSource(gShader, 1, shaderSourceArray, NULL);
	glCompileShader(gShader);
	GLint shaderCompiled = GL_FALSE;
	glGetShaderiv(gShader, GL_COMPILE_STATUS, &shaderCompiled);
	if(shaderCompiled != GL_TRUE){
		printf("Unable to compile vertex shader %d!\n", gShader);
		printShaderLog(gShader);
		return false;
	}
	glAttachShader(gProgramID, gShader);
	return true;
}

void ShaderUtil::printShaderLog(GLuint shader){
	if(glIsShader(shader)){
		int infoLogLength = 0;
		int maxLength = infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		char* infoLog = new char[maxLength];
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if(infoLogLength > 0){
				printf("%s\n", infoLog);
		}
		delete[] infoLog;
	} else{
		printf("Name %d is not a shader\n", shader);
	}
}	

void ShaderUtil::printProgramLog(GLuint program){
	if(glIsProgram(program)){
		int infoLogLength = 0;
		int maxLength = infoLogLength;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
		char* infoLog = new char[ maxLength ];
		glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
		if(infoLogLength > 0){
			printf( "%s\n", infoLog );
		}
		delete[] infoLog;
	} else{
		printf("Name %d is not a program\n", program);
	}
}
