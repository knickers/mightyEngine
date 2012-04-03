#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
using namespace std;
#include <GL/glut.h>

const double PI = 3.141592653589793;

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
