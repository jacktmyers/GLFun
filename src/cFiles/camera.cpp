#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_events.h>

#include "camera.h"
#include "math.h"

Camera::Camera(){
}

// Constructor field of view in degrees, w and h
Camera::Camera(float _hfov, int _w, int _h) : hfov(_hfov), w(_w), h(_h) {
	eye = V3(0.0f, 0.0f, 0.0f); // Where the viewer is
	a = V3(1.0f, 0.0f, 0.0f); // Unit vector along plane
	b = V3(0.0f, -1.0f, 0.0f); // Unit vector along plane
	float hfovd = hfov / 180.0f * (float)M_PI; // The angle of the field of view
	// The vector to the top left of the view screen
	c = V3(-(float)w / 2.0f, (float)h / 2.0f, -(float)w / (2.0f * tan(hfovd / 2.0f)));
	modelViewMat = M44();
	projMat = M44();
}

// Get the view direction
V3 Camera::getVD() {
	return (a ^ b).normalize();
}

// Get the distance to the view field
float Camera::getF() {
	return c * getVD();
}

// Set the view distance with GL
void Camera::setIntrinsics(float near, float far) {
	glViewport(0, 0, w, h);
	float scf = near / getF();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-(float)w / 2.0f * scf, (float)w / 2.0f * scf, -(float)h / 2.0f * scf, (float)h / 2.0f * scf, near, far);
	glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&projMat);
}

// Position the camera with GL
void Camera::setExtrinsics(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	V3 L = eye + getVD() * 100.0f;
	gluLookAt(eye[0], eye[1], eye[2], L[0], L[1], L[2], -b[0], -b[1], -b[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&modelViewMat);
}

void Camera::positionAndOrient(V3 newEye, V3 lookAt, V3 up) {
	V3 vdn = (lookAt - newEye).normalize();
	V3 an = (vdn ^ up).normalize();
	V3 bn = (vdn ^ an).normalize();
	// Distance to view plane
	float f = getF();
	// New direction to top left
	V3 cn = (vdn * f) - (an * (float)w / 2.0f) - (bn * (float)h / 2.0f);
	// Set new values
	a = an;
	b = bn;
	c = cn;
	eye = newEye;
}
