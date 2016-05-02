//Name: Zhou Li
//Student Id: 20291588

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "trackball.h"
#include "primitive.h"

// Define the size of the window using these two variables
int window_width = 800, window_height = 600;

// These constants define the motion mode of mouse control
#define MOTION_NONE			0
#define MOTION_TRACKBALL		1
#define MOTION_ZOOM			2
#define MOTION_PAN			3

// These variables are used for controlling the mouse motion
int motion = MOTION_NONE;
int lastx, lasty;
float quat[4];
float eyex = 0, eyey = 0, eyez = 800;

// The display list of the teapot scene
GLuint teapot_list;

// These constants represent the teapot/robot
#define DISPLAY_TEAPOT  0
#define DISPLAY_ROBOT   1

// Display teapot or robot
int current_display = DISPLAY_TEAPOT;

/*
* Initialize the OpenGL settings and create the display list of the scene
*/
void InitSettings()
{
	// Reset the parameter for trackball motion
	trackball(quat, 0.0, 0.0, 0.0, 0.0);

	// Set up OpenGL parameters
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glClearColor(0.6, 0.6, 0.6, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);

	// Set up the light
	GLfloat position[] = { 500, 500, 500, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);

	// Set up the material values for specular reflection
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess[] = { 50.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	// Create a display list for the teapot scene
	teapot_list = glGenLists(1);
	glNewList(teapot_list, GL_COMPILE);

	// Create the objects: a teapot and a floor

	// The vertex ordering of the GLUT teapot is clockwise, therefore
	// glFrontFace(GL_CW) is used before drawing
	glFrontFace(GL_CW);
	glColor3f(1, 0.5, 0.5);
	glutSolidTeapot(250);
	glFrontFace(GL_CCW);

	// Draw the floor, note that the vertex ordering is counter-clockwise
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(500, -190, 500);
	glVertex3f(500, -190, -500);
	glVertex3f(-500, -190, -500);
	glVertex3f(-500, -190, 500);
	glEnd();

	glEndList();
}

int angleHead = 0;
int left_arm_change_angle = 0;
int right_arm_change_angle = 0;
int left_leg_change_angle = 0;
int right_leg_change_angle = 0;
int body_change_angle = 0;


void displayHead() {
	glColor3f(1, 0, 0);
	DrawBox(50, 60 ,80);
	glTranslatef(0, 110, 0);

	glColor3f(1, 0, 0);
	glRotatef(angleHead, 0, 1, 0);
	DrawBox(160, 160, 150);
	glTranslatef(0, -20, 140);


	glColor3f(1, 1, 0);
	glTranslatef(0, -20, -60);
	DrawBox(90, 10, 20);

	glPushMatrix();
	glTranslatef(40, 70, 0);
	glRotatef(90, 1, 0, 0);
	glColor3f(1, 1, 0);
	DrawCylinder(20, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-40, 70, 0);
	glRotatef(90, 1, 0, 0);
	glColor3f(1, 1, 0);
	DrawCylinder(20, 20, 30);
	glPopMatrix();

}

void displayLeftArm() {

	glColor3f(0, 0, 1);
	glRotatef(90, 0, 0, 1);
	glRotatef(left_arm_change_angle, 0, 1, 0);

	glTranslatef(0, 10, 0);
	glColor3f(1, 0, 0);
	DrawBox(70, 70, 70);
	glTranslatef(-100, 0, 0);
	DrawBox(260, 60, 40);
	glTranslatef(-90, 0, 0);

	glColor3f(1, 0, 0);
	glRotatef(0, 0, 1, 0);
	glTranslatef(-60, 0, -10);
	DrawBox(100, 30, 15);
}

void displayRightArm() {

	glColor3f(0, 0, 1);
	glRotatef(270, 0, 0, 1);
	glRotatef(right_arm_change_angle, 0, 1, 0);

	glTranslatef(0, 10, 0);
	glColor3f(1, 0, 0);
	DrawBox(70, 70, 70);

	glTranslatef(100, 0, 0);
	DrawBox(260, 60, 40);
	glTranslatef(0, 0, 0);
	glColor3f(1, 0, 0);
	glRotatef(30, 0, 0, 0);
	glTranslatef(180, 10, 0);
	DrawBox(100, 30, 15);
}

void displayLeg() {
	glTranslatef(0, -40, 0);
	glColor3f(1, 0, 0);
	DrawBox(40, 60, 250);

	glColor3f(1, 0, 0);
	glTranslatef(0, -150, 10);
	DrawBox(80, 160, 60);

}

void displayLeftLeg() {
	glRotatef(-left_leg_change_angle, 1, 0, 0);
	displayLeg();

}

void displayRightLeg() {
	glRotatef(-right_leg_change_angle, 1, 0, 0);
	displayLeg();

}

void displayBody() {
	glColor3f(1, 1, 0);
	DrawBox(200, 200, 300);

	glColor3f(1, 0, 0);
	glTranslatef(0, 20, 70);
	DrawBox(160, 20, 50);



}

void displayRobot() {

	glRotatef(-body_change_angle, 0, 1, 0);

	// display head
	glPushMatrix();
	glTranslatef(0, 120, 0);
	displayHead();
	glPopMatrix();

	// display left arm
	glPushMatrix();
	glTranslatef(-120, 50, 0);
	displayLeftArm();
	glPopMatrix();

	// display right arm
	glPushMatrix();
	glTranslatef(120, 50, 0);
	displayRightArm();
	glPopMatrix();

	// display left leg
	glPushMatrix();
	glTranslatef(-60, -200, 0);
	displayLeftLeg();
	glPopMatrix();

	// display right leg
	glPushMatrix();
	glTranslatef(60, -200, 0);
	displayRightLeg();
	glPopMatrix();

	//display body
	glPushMatrix();
	glTranslatef(0, -40, 0);
	displayBody();
	glPopMatrix();

}

/*
* The callback function when a redraw has been requested
*/
void DisplayFunc()
{
	// Create the screen for both colour and depth values
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// The modelview matrix is pushed before drawing the scene
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Set up the trackball transformation
	float rotmatrix[4][4];
	build_rotmatrix(rotmatrix, quat);
	glMultMatrixf(&rotmatrix[0][0]);

	// Draw the scene from the display list
	if (current_display == DISPLAY_TEAPOT)
		glCallList(teapot_list);
	else {
		displayRobot();
	}

	glPopMatrix();

	// Redraw the screen by swapping the buffer
	glutSwapBuffers();
}

/*
* The callback function when the window is resized
*/
void ReshapeFunc(int w, int h)
{
	// Set the viewport to be the same as the window
	glViewport(0, 0, w, h);

	// Set up the eye view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, eyez, 0, 0, 0, 0, 1, 0);

	// Set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 2000.0);

	window_width = w;
	window_height = h;
}

/*
* The callback function when a key is pressed
*/
void KeyboardFunc(unsigned char key, int x, int y)
{
	switch (key) {
	case 'e':
		angleHead = angleHead + 1;
		glutPostRedisplay();
		break;

	case 'w':
		angleHead = angleHead - 1;
		glutPostRedisplay();
		break;

	case 'a':
		left_arm_change_angle = left_arm_change_angle + 1;
		glutPostRedisplay();
		break;

	case 's':
		left_arm_change_angle = left_arm_change_angle - 1;
		glutPostRedisplay();
		break;

	case 'd':
		right_arm_change_angle = right_arm_change_angle + 1;
		glutPostRedisplay();
		break;

	case 'f':
		right_arm_change_angle = right_arm_change_angle - 1;
		glutPostRedisplay();
		break;

	case 'z':
		left_leg_change_angle = left_leg_change_angle + 1;
		glutPostRedisplay();
		break;

	case 'x':
		left_leg_change_angle = left_leg_change_angle - 1;
		glutPostRedisplay();
		break;

	case 'c':
		right_leg_change_angle = right_leg_change_angle + 1;
		glutPostRedisplay();
		break;

	case 'v':
		right_leg_change_angle = right_leg_change_angle - 1;
		glutPostRedisplay();
		break;

	case ',':
		body_change_angle = body_change_angle + 1;
		glutPostRedisplay();
		break;

	case '.':
		body_change_angle = body_change_angle - 1;
		glutPostRedisplay();
		break;

	case ' ':
		// Switch the scene between teapot/tin robot
		if (current_display == DISPLAY_TEAPOT)
			current_display = DISPLAY_ROBOT;
		else
			current_display = DISPLAY_TEAPOT;
		glutPostRedisplay();
		break;
	case 27:
		// Exit the program using the escape key
		exit(0);
		break;
	}
}

/*
* The callback function when a mouse button is pressed
*/
void MouseFunc(int button, int state, int x, int y)
{
	// Control the current motion of the mouse control
	if (state == GLUT_DOWN) {
		lastx = x;
		lasty = y;

		motion = MOTION_NONE;
		if (button == GLUT_LEFT_BUTTON)
			motion = MOTION_TRACKBALL;
		else if (button == GLUT_RIGHT_BUTTON) {
			if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
				motion = MOTION_PAN;
			else
				motion = MOTION_ZOOM;
		}
	}
}

/*
* The callback function when the mouse is moved
*/
void MotionFunc(int x, int y)
{
	// Calculate the transformation for trackball control
	if (motion == MOTION_TRACKBALL) {
		float dq[4];
		trackball(dq,
			(2.0 * lastx - window_width) / window_width,
			(window_height - 2.0 * lasty) / window_height,
			(2.0 * x - window_width) / window_width,
			(window_height - 2.0 * y) / window_height);
		add_quats(dq, quat, quat);

		lastx = x;
		lasty = y;

		glutPostRedisplay();
	}
	// Adjust the eye position for zooming
	else if (motion == MOTION_ZOOM) {
		eyez = eyez * (1.0 - (float)(lasty - y) / window_height);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eyex, eyey, eyez, eyex, eyey, 0, 0, 1, 0);

		lastx = x;
		lasty = y;

		glutPostRedisplay();
	}
	// Adjust the eye position for panning
	else if (motion == MOTION_PAN) {
		eyex -= 2.0 * (x - lastx);
		eyey += 2.0 * (y - lasty);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eyex, eyey, eyez, eyex, eyey, 0, 0, 1, 0);

		lastx = x;
		lasty = y;

		glutPostRedisplay();
	}
}

int main(int argc, char** argv)
{
	// Initialize the window with double and depth buffering
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Assignment 2 - Hierarchical Modeling");

	// Initialize the OpenGL settings
	InitSettings();

	// Set up the callback functions
	glutKeyboardFunc(KeyboardFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);
	glutReshapeFunc(ReshapeFunc);
	glutDisplayFunc(DisplayFunc);

	// Start the main loop
	glutMainLoop();
}

