#include "pipeline.h"
#include "shaderUtil.h"
#include "scene.h"
#include "primatives/m44.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#define SHADER_DIR "./src/shaders"

Pipeline::Pipeline(int _screenWidth, int _screenHeight):
	screenWidth(_screenWidth),
	screenHeight(_screenHeight){
	gProgramID = 0; // Shader Program
	shaderManager = new ShaderUtil();
	scene = new Scene(screenWidth, screenHeight);
	pix = new unsigned int[screenWidth*screenHeight];

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
	glClearColor( 0.f, 0.f, 0.f, 0.f );
	return true;
}

void Pipeline::run(){
	SDL_Event event;
	live = true;
	while(live){
			SDL_PollEvent(&event);
			handleEvent(&event);
			render();	
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
			unsigned int pixel = 0;
			glReadBuffer(GL_BACK);
			glReadPixels(mouseX, (screenHeight-mouseY), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);
			float scaleFactor = ((float)1/(float)255); // Used of scaling debug output
			printf("X: %d Y: %d (X:%.2f,Y:%.2f,Z:%.2f,%.2f)\n",mouseX, screenHeight-mouseY, 
					(float)(pixel & 0xFF)*scaleFactor, // red
					(float)((pixel >> 8)&0xFF)*scaleFactor, // green
					(float)((pixel >> 16)&0xFF)*scaleFactor, // blue
					(float)(pixel >> 24)*scaleFactor // alpha
				);
		}
	}
	if(Event->type == SDL_MOUSEBUTTONUP){
		if(leftMouseButtonDown && Event->button.button == SDL_BUTTON_LEFT){
			leftMouseButtonDown = false;
		}
	}
}

void Pipeline::render(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// Set camera
	scene->camera->setIntrinsics(0.1f, 1000.0f);
	scene->camera->setExtrinsics();
	for(auto sceneIt = scene->meshes.begin(); sceneIt != scene->meshes.end(); ++sceneIt){
		glUseProgram(gProgramID);
		TM *currMesh = *sceneIt;
		
		// Cube Spinnning
		currMesh->rotateAboutCenter(.2, .3, 1);

		// Get Model View Matrix
		GLfloat modelViewMat[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMat);

		// Get Projection matrix
		GLfloat projMat[16];
		glGetFloatv(GL_PROJECTION_MATRIX, projMat);

		// Load matrices into shader
		GLint location = glGetUniformLocation(gProgramID, "modelViewMat");
		glUniformMatrix4fv(location, (GLsizei)1, GL_FALSE, modelViewMat);
		location = glGetUniformLocation(gProgramID, "projMat");
		glUniformMatrix4fv(location, (GLsizei)1, GL_FALSE, projMat);
		
		//printf("ERROR: %X\n", glGetError());
		
		glEnable(GL_DEPTH_TEST);  
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, currMesh->verts);
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, currMesh->trisMemN, GL_UNSIGNED_INT, currMesh->tris); // Seg Fault Here
		glDisableClientState(GL_VERTEX_ARRAY);

		// Disable program
		glUseProgram((GLuint)NULL);
	}
	SDL_GL_SwapWindow(gWindow);
}

void Pipeline::cleanup(){
	SDL_GL_DeleteContext(gContext);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
}
