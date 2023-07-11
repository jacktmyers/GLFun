#version 330

in vec4 vertexColor;

void main(){
	gl_FragColor = vec4(gl_FragCoord[3]*20-.4, mod(gl_FragCoord.x/500,1), mod(gl_FragCoord.y/500,1), 1);
}
