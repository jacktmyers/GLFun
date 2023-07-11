#version 330

layout (location = 0) in vec3 aPos; // the position variable has attribute position 0

uniform mat4 modelViewMat;
uniform mat4 projMat;

//out vec4 vertexColor; // specify a color output to the fragment shader

void main(){
	gl_Position = projMat * modelViewMat * vec4(aPos,1.0);
}
