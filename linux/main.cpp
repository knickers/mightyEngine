//
// 3D Animation of the MYT engine
// Nick Cox
// Started June 2010
// Completed July 2011
//
// Glut32 Start-Up Template
// Created by Bart Stander
// Modified and enhanced by Nick Cox
//

#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <sstream>
using namespace std;
#include <GL/glut.h>
#include "graphics.h"
//#include "save.h"
#include "shapes.h"
#include "slider.h"
#include "colors.h"
#include "MYT.h"

double screen_x = 1024;//720;//480;// Width of the screen
double screen_y = 680; //480;//320;// Height of the screen
//Global Variables for Camera
double EYE[3];             // Position of camera
double AT[3]={0,0,0};      // Looking AT point for camera
double UP[3];              // Up vector for camera
double gUpAngle   = PI/7;  // Sphere coordinates of camera //PI/14;
double gPanAngle  = PI*5/4;// Sphere coordinates of camera //PI*3/2;
double gCamAmount = PI/56; // Ammount the camera moves on a key press
double gDist      = 425;   // Camera distance from the origin
double gZoom      = 40;    // Zoom angle, half on each side
double gNear      = 100;   // Near cutpff plane
double gFar       = 500;   // Far cutoff plane
double dX         = 0;     // Delta X for mouse panning
double dY         = 0;     // Delta Y for mouse panning
bool   gGrabbed   = false; // If scene is grabbed by mouse
// MYT
MYT gMYT;
double gSpeed    = 4; // Max degree rotation per frame
bool gShowStages = 0;
bool gShowMenu   = 0;
verticalSlider gSpeedSlider(15,45,50,600);
verticalSlider gZoomSlider( 45,45,50,600);

//Functions difinitions
//**** User code ****
void display(void);
void keyboard(unsigned char c, int x, int y);
void specialKeys(int key, int x, int y);
void mouseClick(int mouse_button, int state, int x, int y);
void mouseDrag(int x, int y);
void mouseMove(int x, int y);
void mouseWheel(int button, int dir, int x, int y);
void idle(void);
void InitializeMyStuff();
//**** Permanent code ****
//void SaveAnim(int frames);
void MoveCamera();
void text_output(double x, double y, char *string);
void stroke_text(double x, double y, int scale, int rotate, string string);
void number_output(double x, double y, double num);
void stroke_number(double x, double y, int scale, int rotate, double num);
int  random(int low, int high);
void reshape(int w, int h);
void Set2DView();
void Set3DView();
double GetFPS();
double GetFramesPerSecond();

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(10, 10);

	bool fullscreen = 0;
	if(fullscreen)
	{
		glutGameModeString("1280x800:32");
		glutEnterGameMode();
	}
	else
		glutCreateWindow("8 Cylinder Massive Yet Tiny Engine (MYT)"); // Title of the window

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseDrag);
	glClearColor(.8,.8,.8, 0); // Background color

	InitializeMyStuff();
	glutMainLoop();
	return 0;
}

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/**********************************************************************/
	Set3DView();                 // 3D VIEW
	/**********************************************************************/
	gMYT.Draw();
	gMYT.Move(gSpeedSlider.GetValue()*gSpeed);

	/**********************************************************************/
	Set2DView();                 // 2D View
	/**********************************************************************/
	gSpeedSlider.Draw();
	gZoomSlider.Draw();
	glColor3f(0,0,0);
	text_output(10, 7, "RPM:");
	number_output(50, 7, gMYT.GetRPM());
	DrawRoundSquare(screen_x/32  , screen_y-25, screen_x/30+14, 30, 10);
	DrawRoundSquare(screen_x/32*3, screen_y-25, screen_x/30+14, 30, 10);
	glColor3f(1,1,1);
	text_output(screen_x/32-22, screen_y-30, "Speed");
	text_output(screen_x/32*3-18, screen_y-30, "Zoom");
	if(gShowMenu)
		gMYT.DrawMenu();

	// Swap the image to the front screen
	glutSwapBuffers();
	// Loop over the display function
	glutPostRedisplay();
}

// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y) {
	if(c >= '0' && c <= '9')
		gMYT.SetNumVanes(c - '0');
	switch (c) {
		case 27: // escape character to quit the program
			exit(0);
			break;
		case 9: // tab
			gShowMenu = !gShowMenu;
			break;
		case ',': // <
			gSpeedSlider.Increment(-.0625);
			break;
		case '.': // >
			gSpeedSlider.Increment(.0625);
			break;
		case 'c':
			gShowStages = !gShowStages;
			gMYT.SetStages(gShowStages);
			break;
//		case 's':
//			Save("ppm");
//			break;
		case '-': // -
			gZoomSlider.Increment(-0.0625);
			break;
		case'+': // +
			gZoomSlider.Increment(0.0625);
			break;
		case 13: // return
			break;
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}
	MoveCamera();
	glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_LEFT:
			gPanAngle -= gCamAmount;
			break;
		case GLUT_KEY_DOWN:
			gUpAngle -= gCamAmount;
			break;
		case GLUT_KEY_RIGHT:
			gPanAngle += gCamAmount;
			break;
		case GLUT_KEY_UP:
			gUpAngle += gCamAmount;
			break;
		default:
			return;
	}
	MoveCamera();
	glutPostRedisplay();
}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouseClick(int mouse_button, int state, int x, int y) {
	y = screen_y - y;
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if(!gSpeedSlider.Click(x, y) && !gZoomSlider.Click(x, y)) {
			dX=x;
			dY=y;
			gGrabbed = true;
		}
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		gGrabbed = false;
	}
	MoveCamera();
	glutPostRedisplay();
}

// This callback function gets called by the Glut
// system whenever any mouse button is drug.
void mouseDrag(int x, int y) {
	y = screen_y - y;

	if(!gSpeedSlider.Drag(x, y) && !gZoomSlider.Drag(x, y) && gGrabbed) {
		if(gUpAngle > PI/2 && gUpAngle < 3*PI/2)
			gPanAngle += (x-dX)/500.0;
		else
			gPanAngle -= (x-dX)/500.0;
		gUpAngle  -= (y-dY)/500.0;
		if(gUpAngle > 2*PI)
			gUpAngle -= 2*PI;
		if(gUpAngle < 0)
			gUpAngle += 2*PI;
		if(gPanAngle > 2*PI)
			gPanAngle -= 2*PI;
		if(gPanAngle < 0)
			gPanAngle += 2*PI;
		dX=x;
		dY=y;
	}
	MoveCamera();
	glutPostRedisplay();
}

// Initialization code goes here.
void InitializeMyStuff() {
	// Slider bars
	gSpeedSlider.SetType(SLIDER_BLOCK);
	gSpeedSlider.SetColor(.5,.5,.5);
	gSpeedSlider.SetValue(.1);
	gZoomSlider.SetType(SLIDER_BLOCK);
	gZoomSlider.SetColor(.5,.5,.5);
	gZoomSlider.SetValue(.275);
	// MYT
	gMYT.InitDisplayLists();
	// Camera Position
	MoveCamera();
	// set material's specular properties
	float mat_specular[] = {0.5, 0.5, 0.5, 1};
	float mat_shininess[] = {25};
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	// set light properties
	float light_position[] = {0, 0, 300, 1}; // right(x), forward(y), up(z), alpha(transparancy)
	float white_light[] = {1, 1, 1, 1};
	float low_light[] = {.2, .2, .2, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position); // first light's position
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  white_light);    // first light's full color
	glLightfv(GL_LIGHT0, GL_SPECULAR, low_light);      // first light's shadow color
}

//****************************************************************************************************//
//****************************************************************************************************//
//****************************************** Permanent Code ******************************************//
//****************************************************************************************************//
//****************************************************************************************************//

/*void SaveAnim(int frames)
{
	static int count = 0;
	Save("ppm");
	count++;
	if(count > frames)
		exit(0);
}*/

void MoveCamera()
{	// Set near and far planes
	double dist = (1-gZoomSlider.GetValue()) * gDist + 100;
	gFar  = dist+100;
	gNear = dist-160;
	if(gNear <= 0)
		gNear = 1;
	// Set camera position
	SpherePoint(dist,gUpAngle,gPanAngle,EYE);
	// Set up position
	double v1[3],v2[3];
	v1[0]=cos(gPanAngle+PI/2);v1[1]=sin(gPanAngle+PI/2);v1[2]=0;//right
	v2[0]=AT[0]-EYE[0];v2[1]=AT[1]-EYE[1];v2[2]=AT[2]-EYE[2];//forward
	CrossProduct(v1,v2,UP);
	for(int i=0; i<3; i++)
		UP[i] = UP[i]/dist;
}

// Outputs a string of text at the specified location.
void text_output(double x, double y, char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	int len, i;
	glRasterPos2d(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, string[i]);

	glDisable(GL_BLEND);
}

// Outputs text to the screen in 'stroke' format
void stroke_text(double x, double y, int scale, int rotate, string string)
{
	void *font = GLUT_STROKE_ROMAN;
	int length = (int) string.size();
	glPushMatrix();
		glTranslatef(x, y, 0);
			glScalef(screen_x/1024*((double)scale/100),screen_y/786*((double)scale/100),1);
				glRotatef(rotate,0,0,1);
					for (int i=0; i<length; i++)
						glutStrokeCharacter(font, string[i]);
	glPopMatrix();
}

// Outputs a number to the screen at the specified location
void number_output(double x, double y, double num)
{
	void *font = GLUT_BITMAP_9_BY_15;
	string temp;
	stringstream out;
	out << num;
	temp = out.str();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	int len, i;
	glRasterPos2d(x, y);
	len = (int)temp.length();
	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, temp[i]);
	glDisable(GL_BLEND);
}

// Outputs a number in 'stroke' text format
void stroke_number(double x, double y, int scale, int rotate, double num)
{
	void *font = GLUT_STROKE_ROMAN;
	string temp;
	stringstream out;
	out << num;
	temp = out.str();

	int length = (int)temp.size();
	glPushMatrix();
		glTranslatef(x, y, 0);
			glScalef(screen_x/1024*((double)scale/100),screen_y/786*((double)scale/100),1);
				glRotatef(rotate,0,0,1);
					for (int i=0; i<length; i++)
						glutStrokeCharacter(font, temp[i]);
	glPopMatrix();
}

// Return a random number between a low and a high
int random(int low, int high)
{
	return rand() % (high-low+1) + low;
}

// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{	// Reset the global variables to the new width and height.
	screen_x = w;
	screen_y = h;
	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);
	// Resize the sliders
	w = screen_x/30+14; h = screen_y-110;
	gSpeedSlider.SetSize(w, h);
	 gZoomSlider.SetSize(w, h);
	w = screen_x/32; h = h/2 + 45;
	gSpeedSlider.SetPos(w  , h);
	 gZoomSlider.SetPos(w*3, h);
	// Redraw the screen
	glutPostRedisplay();
}

void Set2DView()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set the world coordinates.
	glOrtho(0, screen_x, 0, screen_y, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Set3DView()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (double)screen_x / (double)screen_y;
	gluPerspective(gZoom, aspectRatio, gNear, gFar);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(EYE[0],EYE[1],EYE[2],  AT[0],AT[1],AT[2],  UP[0],UP[1],UP[2]);
}

double GetFPS()
{
	static clock_t startTime = clock();

	double fps = clock() - startTime;
	startTime  = clock();
	return fps;
}

// Returns the number of frames used devided by the time passed
double GetFramesPerSecond()
{
	static int frames = 0;
	static clock_t startTime = clock();

	frames++;
	clock_t currentTime = clock();
	double elapsedTime = (double)(currentTime - startTime)/((double)CLOCKS_PER_SEC);
	return frames/elapsedTime;
}
