#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
using namespace std;
#include <GL/glut.h>

// Platform-dependent sleep routines.
#if defined(__WINDOWS_ASIO__) || defined(__WINDOWS_DS__)
	#include <windows.h>
	#define SLEEP(milliseconds) Sleep((DWORD)milliseconds)
#else // Unix variants
	#include <unistd.h>
	#define SLEEP(milliseconds) usleep((unsigned long)(milliseconds * 1000.0))
#endif

#define PI 3.141592653589793

// "Prototypes" for functions
int random(int low, int high);
void text_output(double x, double y, char *string);
void stroke_text(double x, double y, int scale, int rotate, string string);
void number_output(double x, double y, double num);
void stroke_number(double x, double y, int scale, int rotate, double num);
void Set2DView(int w, int h);
void Set3DView(int w, int h);

// "Prototypes" for global variables
extern double screen_x;
extern double screen_y;

#endif
