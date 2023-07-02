#include "pipeline.h"
#include "shaderUtil.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#define SHADER_DIR "./src/shaders"

Pipeline::Pipeline(int screenWidth, int screenHeight){
	gProgramID = 0; // Shader Program
	gVertexPos2DLocation = -1; // Vertex Attribute 2D Location
	gVBO = 0; // Vertex Buffer
	gIBO = 0; // Index Buffer
	shaderManager = new ShaderUtil();

	if(shaderManager == NULL){
		printf("shaderManager not initialized!\n");
		initialized = false;
		return;
	}
	if(!initSDL(screenWidth, screenHeight)){
		printf("SDL not initialized!\n");
		initialized = false;	
		return;
	}
	if(!initGL(screenWidth, screenHeight)){
		printf("GL not initialized!\n");
		initialized = false;
		return;
	}
	initialized = true;
	return;
}

bool Pipeline::initSDL(int screenWidth, int screenHeight){
	// OpenGL 4.6
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	// Create Context
	gWindow = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if(gWindow == NULL){
		printf("SDL Window Error: %s\n", SDL_GetError());
		return false;
	}
	// Create the context
	gContext = SDL_GL_CreateContext(gWindow); 
	if(gContext == NULL){
		printf("OpenGL Context Creation Error: %s\n", SDL_GetError());
		return false;			
	}
	// Set current context
	if(SDL_GL_MakeCurrent(gWindow, gContext) != 0){
		printf("OpenGL Context Set Error: %s\n", SDL_GetError());
		return false;			
	}
	// Check context
	printf("Checking context: %s\n", glGetString(GL_VERSION));
	glewExperimental=true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
    printf("Failed to initialize GLEW\n");
    return false;
	}
	// Use Vsync
	if(SDL_GL_SetSwapInterval(1) < 0){
		printf("Warning: Unable to set VSync: %s\n", SDL_GetError());
	}
	return true;
}

bool Pipeline::initGL(int screenWidth, int screenHeight){
	gProgramID = glCreateProgram();	
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string shaderDir = SHADER_DIR;
	shaderManager->attatchShader(gProgramID, GL_VERTEX_SHADER, shaderDir + "/vertexShader.glsl");
	shaderManager->attatchShader(gProgramID, GL_FRAGMENT_SHADER, shaderDir + "/fragmentShader.glsl");
	glLinkProgram(gProgramID);
	//Check for errors
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
	if(programSuccess != GL_TRUE){
			printf( "Error linking program %d!\n", gProgramID );
			shaderManager->printProgramLog( gProgramID );
			return false;
	}
	gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
	if(gVertexPos2DLocation == -1) {
			printf( "LVertexPos2D is not a valid glsl program variable!\n" );
			return false;
	}
	glClearColor( 0.f, 0.f, 0.f, 0.f );
	GLfloat vertexData[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
	};
	GLuint indexData[] = { 0, 1, 2, 3 };
	glGenBuffers( 1, &gVBO );
	glBindBuffer( GL_ARRAY_BUFFER, gVBO );
	glBufferData( GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW );
	glGenBuffers( 1, &gIBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW );
	return true;
}

void Pipeline::run(){
	printf("Running pipeline...\n");
	float r = 0;
	int direction = 0;
	SDL_Event event;
	live = true;
	while(live){
			SDL_PollEvent(&event);
			handleEvent(&event);
			render(r,r,r);	
			if(r >= .75)
				direction = 1;
			if(r <= -1)
				direction = 0;
			if(direction == 0)
				r+=.01;
			if(direction == 1)
				r-=.01;
	}
	cleanup();
}

void Pipeline::handleEvent(SDL_Event* Event) {
	SDL_GetMouseState(&mouseX, &mouseY);
	if(Event->type == SDL_QUIT){
		live = false;
		return;
	}
	if(Event->type == SDL_TEXTINPUT){
		char keyPress = Event->text.text[0];	
		if(keyPress == 'q'){
			live = false;
			return;
		}
	}
	if(Event->type == SDL_MOUSEBUTTONDOWN){
		if(!leftMouseButtonDown && Event->button.button == SDL_BUTTON_LEFT){
			leftMouseButtonDown = true;
		}
	}
	if(Event->type == SDL_MOUSEBUTTONUP){
		if(leftMouseButtonDown && Event->button.button == SDL_BUTTON_LEFT){
			leftMouseButtonDown = false;
		}
	}
}

void Pipeline::render(float r, float g, float b){
	glClear( GL_COLOR_BUFFER_BIT );
	glUseProgram(gProgramID);
	// Color Changing Code
	GLint squareColor = glGetUniformLocation(gProgramID, "squareColor");	
	glUniform3f(squareColor, r,g,b);
	//
	glEnableVertexAttribArray(gVertexPos2DLocation);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);
	glDisableVertexAttribArray(gVertexPos2DLocation);
	glUseProgram((GLuint)NULL);
	SDL_GL_SwapWindow(gWindow);
}

void Pipeline::cleanup(){
	SDL_GL_DeleteContext(gContext);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
}
