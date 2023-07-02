#version 140

uniform vec3 squareColor;
out vec4 LFragment;

void main(){
	vec4 adjustedCoord = (gl_FragCoord - 125)/250;
	LFragment = vec4(
	adjustedCoord.x + squareColor.x, 
	adjustedCoord.y + squareColor.y, 
	adjustedCoord.y + squareColor.z, 
	1.0 );
}
