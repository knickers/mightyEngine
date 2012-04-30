#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
#include "shapes.h"

double dtor(double degrees) { return degrees*PI/180; }
double rtod(double radians) { return radians*180/PI; }
double Sin(double degrees) { return sin(dtor(degrees)); }
double Cos(double degrees) { return cos(dtor(degrees)); }
double Tan(double degrees) { return tan(dtor(degrees)); }

// Given the three triangle points x[0],y[0],z[0],
//		x[1],y[1],z[1], and x[2],y[2],z[2],
//		Finds the normal vector n[0], n[1], n[2].
void FindTriangleNormal(double x[], double y[], double z[], double n[])
{
	// Convert the 3 input points to 2 vectors, v1 and v2.
	double v1[3], v2[3];
	v1[0] = x[1] - x[0];
	v1[1] = y[1] - y[0];
	v1[2] = z[1] - z[0];
	v2[0] = x[2] - x[0];
	v2[1] = y[2] - y[0];
	v2[2] = z[2] - z[0];
	
	// Take the cross product of v1 and v2, to find the vector perpendicular to both.
	n[0] = v1[1]*v2[2] - v1[2]*v2[1];
	n[1] = -(v1[0]*v2[2] - v1[2]*v2[0]);
	n[2] = v1[0]*v2[1] - v1[1]*v2[0];

	double length = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
	n[0] /= -length;
	n[1] /= -length;
	n[2] /= -length;
}
// Given the three triangle points;
//		CenterPoint, Point1, Point2
// Finds the normal vector n.x, n.y, n.z,
point3 FindTriangleNormal(point3 cp, point3 p1, point3 p2)
{
	// Convert the 3 input points to 2 vectors, v1 and v2.
	double v1[3], v2[3];
	v1[0] = p1.x - cp.x;
	v1[1] = p1.y - cp.y;
	v1[2] = p1.z - cp.z;
	v2[0] = p2.x - cp.x;
	v2[1] = p2.y - cp.y;
	v2[2] = p2.z - cp.z;
	
	// Take the cross product of v1 and v2, to find the vector perpendicular to both.
	point3 n;
	n.x = v1[1]*v2[2] - v1[2]*v2[1];
	n.y = -(v1[0]*v2[2] - v1[2]*v2[0]);
	n.z = v1[0]*v2[1] - v1[1]*v2[0];

	// Convert to unit length
	double length = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
	n.x /= -length;
	n.y /= -length;
	n.z /= -length;

	return n;
}
// Fine a point on a sphere given the up and pan angles
void SpherePoint(double radius, double upAngle, double panAngle, double point_out[3])
{
	double temp  = radius*cos(upAngle);
	point_out[0] = temp*cos(panAngle);
	point_out[1] = temp*sin(panAngle);
	point_out[2] = radius*sin(upAngle);
}
// Fine a point on a sphere given the up and pan angles
void SpherePoint(double radius, double upAngle, double panAngle, point3 &point_out)
{
	double temp = radius*cos(upAngle);
	point_out.x = temp*cos(panAngle);
	point_out.y = temp*sin(panAngle);
	point_out.z = radius*sin(upAngle);
}
void CrossProduct(double v1[],double v2[],double r[])
{	/*
		i     j     k
	   v1[0] v1[1] v1[2]
	   v2[0] v2[1] v2[2]
	*/
	r[0] = (v1[1]*v2[2])-(v1[2]*v2[1]);
	r[1] = -((v1[0]*v2[2])-(v1[2]*v2[0]));
	r[2] = (v1[0]*v2[1])-(v1[1]*v2[0]);
}
void UnitVector(double v[], double dest[])
{
	double length = 0;
	for(int i=0; i<3; i++)
		length += v[i]*v[i];
	length = sqrt(length);

	if(length == 0)
		for(int i=0; i<3; i++)
			dest[i] = 0;
	else
		for(int i=0; i<3; i++)
			dest[i] = v[i]/length;
}
double DotProduct(double v1[], double v2[])
{
	double answer = 0;
	for(int i=0; i<3; i++)
		answer += v1[i]*v2[i];
	return answer;
}
double LOCangle(double sideA, double sideB, double sideC)
{
	return acos((sideB*sideB+sideC*sideC-sideA*sideA)/(2*sideB*sideC));
}

double LOCside(double angleA, double sideB, double sideC)
{	// angleA in radians
	return sqrt(sideB*sideB + sideC*sideC - 2*sideB*sideC*cos(angleA));
}

void DrawAxis(double length, float xColor[], float yColor[], float zColor[])
{
	// x axis
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, xColor);
	text_output(length+1,0,"X");
	DrawLine(0,0,length,0);
	// y axis
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yColor);
	text_output(0,length+1,"Y");
	DrawLine(0,0,0,length);
	// z axis
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, zColor);
	glBegin(GL_LINES);
	glVertex3d(0,0,0);
	glVertex3d(0,0,length);
	glEnd();
	glTranslated(0,0,length+1);
		text_output(0,0,"Z");
	glTranslated(0,0,-length-1);
}

// Draws a 32 sided polygon (circle) at the specified location
void DrawCircle(double x1, double y1, double radius)
{
	glBegin(GL_POLYGON);
	glNormal3d(0,0,0);
	for(int i=0; i<32; i++)
	{
		double theta = (double)i/32.0 * 2.0 * PI;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

// Draws a 32 sided polygon (circle) at the specified location,
// with the specified colors
void DrawCircle(double x1, double y1, double radius, float insideColor[], float outsideColor[])
{
	glBegin(GL_POLYGON);
	glColor3fv(insideColor);
	glVertex2d(x1,y1);
	glColor3fv(outsideColor);
	for(int i=0; i<=32; i++)
	{
		double theta = (double)i/32.0 * 2.0 * 3.1415926;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

void DrawCircle2(double x1, double y1, double radius)
{
	// Gradiated
	glBegin(GL_POLYGON);
	glColor3f(.5,.5,.5);
	glVertex2d(x1,y1);
	for(int i=0; i<32; i++)
	{
		double theta = (double)i/31.0 * 2.0 * PI;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glColor3f(.2,.2,.2);
		glVertex2d(x, y);
	}
	glEnd();
}

void DrawCircle3(double x1, double y1, double radius)
{
	// Just an outline
	glBegin(GL_LINE_STRIP);
	glVertex2d(x1,y1);
	for(int i=0; i<32; i++)
	{
		double theta = (double)i/31.0 * 2.0 * PI;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

// Draws a solid Elipse from the origin to the positive z axis
void DrawSolidCylinder(double length, double xR, double yR, double slices, bool hollow)
{
	double x,y,inc=2*PI/slices;
	glPushMatrix();
	if(length < 0)
	{
		length = -length;
		glTranslated(0, 0, -length);
	}
	if(!hollow)
	{
		// Bottom circle
		glBegin(GL_POLYGON);
		glNormal3d(0,0,-1);
		glVertex3d(0,0,0);
		for(double i=0; i<=2*PI; i+=inc)
		{
			x = xR * cos(i);
			y = yR * sin(i);
			glVertex3d(x, y, 0);
		}
		glEnd();
		// Top circle
		glBegin(GL_POLYGON);
		glNormal3d(0,0,1);
		glVertex3d(0,0,length);
		for(double i=0; i<=2*PI; i+=inc)
		{
			x = xR * cos(i);
			y = yR * sin(i);
			glVertex3d(x, y, length);
		}
		glEnd();
	}
	// Walls
	for(double i=0; i<=2*PI; i+=inc)
	{
		// Bottom circle x and y
		x = xR * cos(i);
		y = yR * sin(i);
		// Walls
		glBegin(GL_QUADS);
		glNormal3d(cos(i),sin(i),0);
		glVertex3d(x,y,0);
		glVertex3d(x,y,length);
		// Next x and y
		x = xR * cos(i+inc);
		y = yR * sin(i+inc);
		// Next Walls
		glNormal3d(cos(i+inc),sin(i+inc),0);
		glVertex3d(x,y,length);
		glVertex3d(x,y,0);
		glEnd();
	}
	glPopMatrix();
}

// Draws a solid cylinder from the origin to the positive z axis
void DrawSolidCylinder(double length, double radius, double slices, bool hollow)
{
	double theta,x,y;
	if(!hollow)
	{	// Bottom circle
		glBegin(GL_POLYGON);
		if(length < 0)
			glNormal3d(0,0,1);
		else
			glNormal3d(0,0,-1);
		glVertex3d(0,0,0);
		for(int i=0; i<=slices; i++)
		{
			theta = (double)i/slices * 2.0 * PI;
			x = radius * cos(theta);
			y = radius * sin(theta);
			glVertex3d(x, y, 0);
		}
		glEnd();
		// Top circle
		glBegin(GL_POLYGON);
		if(length < 0)
			glNormal3d(0,0,-1);
		else
			glNormal3d(0,0,1);
		glVertex3d(0,0,length);
		for(int i=0; i<=slices; i++)
		{
			theta = (double)i/slices * 2.0 * PI;
			x = radius * cos(theta);
			y = radius * sin(theta);
			glVertex3d(x, y, length);
		}
		glEnd();
	}
	// Walls
	for(int i=0; i<=slices; i++)
	{
		// Bottom circle x and y
		theta = (double)i/slices * 2.0 * PI;
		x = radius * cos(theta);
		y = radius * sin(theta);
		// Walls
		glBegin(GL_QUADS);
		glNormal3d(cos(theta),sin(theta),0);
		glVertex3d(x,y,0);
		glVertex3d(x,y,length);
		// Next x and y
		theta = (double)(i+1)/slices * 2.0 * PI;
		x = radius * cos(theta);
		y = radius * sin(theta);
		// Next Walls
		glNormal3d(cos(theta),sin(theta),0);
		glVertex3d(x,y,length);
		glVertex3d(x,y,0);
		glEnd();
	}
}

void DrawWireCylinder(double length, double radius, int slices, bool hollow)
{
	double theta,x,y;
	if(!hollow)
	{
		// Bottom circle
		glBegin(GL_LINE_LOOP);
		for(int i=0; i<=slices; i++)
		{
			theta = (double)i/(double)slices * 2.0 * PI;
			x = radius * cos(theta);
			y = radius * sin(theta);
			glVertex3d(x, y, 0);
		}
		glEnd();
		// Top circle
		glBegin(GL_LINE_LOOP);
		for(int i=0; i<=slices; i++)
		{
			theta = (double)i/(double)slices * 2.0 * 3.1415926;
			x = radius * cos(theta);
			y = radius * sin(theta);
			glVertex3d(x, y, length);
		}
		glEnd();
	}
	// Walls
	for(int i=0; i<=slices; i++)
	{
		// Bottom circle x and y
		theta = (double)i/(double)slices * 2.0 * 3.1415926;
		x = radius * cos(theta);
		y = radius * sin(theta);
		// Walls
		glBegin(GL_LINE_LOOP);
		glVertex3d(x,y,0);
		glVertex3d(x,y,length);
		// Next x and y
		theta = (double)(i+1)/(double)slices * 2.0 * 3.1415926;
		x = radius * cos(theta);
		y = radius * sin(theta);
		// Next Walls
		glVertex3d(x,y,length);
		glVertex3d(x,y,0);
		glEnd();
	}
}

void DrawTaperCylinder(double length, double bottomRadius, double topRadius, int slices, bool hollow)
{
	double theta,x,y;
	double angle = sin(atan2(length,topRadius-bottomRadius) - PI/2);
	if(!hollow)
	{
		// Bottom circle
		glBegin(GL_POLYGON);
		glNormal3d(0,0,-1);
		glVertex3d(0,0,0);
		for(int i=0; i<=slices; i++)
		{
			theta = (double)i/(double)slices * 2.0 * PI;
			x = bottomRadius * cos(theta);
			y = bottomRadius * sin(theta);
			glVertex3d(x, y, 0);
		}
		glEnd();
		// Top circle
		glBegin(GL_POLYGON);
		glNormal3d(0,0,1);
		glVertex3d(0,0,length);
		for(int i=0; i<=slices; i++)
		{
			theta = (double)i/(double)slices * 2.0 * PI;
			x = topRadius * cos(theta);
			y = topRadius * sin(theta);
			glVertex3d(x, y, length);
		}
		glEnd();
	}
	// Walls
	for(int i=0; i<slices; i++)
	{
		// Bottom circle x and y
		theta = (double)i/(double)slices * 2.0 * PI;
		x = bottomRadius * cos(theta);
		y = bottomRadius * sin(theta);
		// Walls
		glBegin(GL_QUADS);
		glNormal3d(cos(theta),sin(theta),sin(angle));
		glVertex3d(x,y,0);
		x = topRadius * cos(theta);
		y = topRadius * sin(theta);
		glVertex3d(x,y,length);
		// Next x and y
		theta = (double)(i+1)/(double)slices * 2.0 * PI;
		x = topRadius * cos(theta);
		y = topRadius * sin(theta);
		// Next Walls
		glNormal3d(cos(theta),sin(theta),sin(angle));
		glVertex3d(x,y,length);
		x = bottomRadius * cos(theta);
		y = bottomRadius * sin(theta);
		glVertex3d(x,y,0);
		glEnd();
	}
}

void DrawCurveCylinder(double innerRadius, double outerRadius, double angle, int stacks, int slices, bool hollow)
{
	angle = angle*PI/180.0;
	double a = angle/(double)slices;
	double b = 2.0*PI/(double)stacks;
	for(double i=0; i<angle; i+=a)
	{
		for(double j=0; j<2*PI; j+=b)
		{
			glBegin(GL_QUADS);
			glNormal3d(cos(j)*cos(i),cos(j)*sin(i),sin(j));
			glVertex3d( outerRadius*cos(i)+innerRadius*cos(j)*cos(i), // x
						outerRadius*sin(i)+innerRadius*cos(j)*sin(i), // y
						innerRadius*sin(j));						  // z
			glNormal3d(cos(j+b)*cos(i),cos(j+b)*sin(i),sin(j+b));
			glVertex3d( outerRadius*cos(i)+innerRadius*cos(j+b)*cos(i), // x
						outerRadius*sin(i)+innerRadius*cos(j+b)*sin(i), // y
						innerRadius*sin(j+b));							// z
			glNormal3d(cos(j+b)*cos(i+a),cos(j+b)*sin(i+a),sin(j+b));
			glVertex3d( outerRadius*cos(i+a)+innerRadius*cos(j+b)*cos(i+a), // x
						outerRadius*sin(i+a)+innerRadius*cos(j+b)*sin(i+a), // y
						innerRadius*sin(j+b));								// z
			glNormal3d(cos(j)*cos(i+a),cos(j)*sin(i+a),sin(j));
			glVertex3d( outerRadius*cos(i+a)+innerRadius*cos(j)*cos(i+a), // x
						outerRadius*sin(i+a)+innerRadius*cos(j)*sin(i+a), // y
						innerRadius*sin(j));							  // z
			glEnd();
		}
	}
	if(!hollow)
	{
		glBegin(GL_POLYGON);
		glNormal3d(0,-1,0);
		for(double i=0; i<2*PI; i+=b)
		{
			glVertex3d(outerRadius+innerRadius*cos(i), 0, innerRadius*sin(i));
		}
		glEnd();
		glBegin(GL_POLYGON);
		glNormal3d(-sin(angle),cos(angle),0);
		for(double i=0; i<2*PI; i+=b)
		{
			glVertex3d( outerRadius*cos(angle)+innerRadius*cos(i)*cos(angle), // x
						outerRadius*sin(angle)+innerRadius*cos(i)*sin(angle), // y
						innerRadius*sin(i));								  // z
		}
		glEnd();
	}
}

void DrawPartialCylinder(double length, double radius, double angle, double slices, bool hollow)
{
	double x,y;
	angle = angle*PI/180;
	slices = angle/slices;
	if(!hollow)
	{
		// Bottom circle
		glBegin(GL_POLYGON);
		glNormal3d(0,0,-1);
		glVertex3d(0,0,0);
		for(double i=0; i<=angle; i+=slices)
		{
			x = radius * cos(i);
			y = radius * sin(i);
			glVertex3d(x, y, 0);
		}
		glEnd();
		// Top circle
		glBegin(GL_POLYGON);
		glNormal3d(0,0,1);
		glVertex3d(0,0,length);
		for(double i=0; i<=angle; i+=slices)
		{
			x = radius * cos(i);
			y = radius * sin(i);
			glVertex3d(x, y, length);
		}
		glEnd();
	}
	// Walls
	for(double i=0; i<angle; i+=slices)
	{
		// Bottom circle x and y
		x = radius * cos(i);
		y = radius * sin(i);
		// Walls
		glBegin(GL_QUADS);
		glNormal3d(cos(i),sin(i),0);
		glVertex3d(x,y,0);
		glVertex3d(x,y,length);
		// Next x and y
		x = radius * cos(i+slices);
		y = radius * sin(i+slices);
		// Next Walls
		glNormal3d(cos(i+slices),sin(i+slices),0);
		glVertex3d(x,y,length);
		glVertex3d(x,y,0);
		glEnd();
	}
}

void DrawSolidBox(double X_length, double Y_width, double Z_height)
{
	double l = X_length/2;
	double w = Y_width/2;
	// bottom
	glBegin(GL_QUADS);
	glNormal3d(0,0,-1);
	glVertex3d(-l,-w,0);
	glVertex3d(-l,w,0);
	glVertex3d(l,w,0);
	glVertex3d(l,-w,0);
	glEnd();
	// top
	glBegin(GL_QUADS);
	glNormal3d(0,0,1);
	glVertex3d(-l,-w,Z_height);
	glVertex3d(-l,w,Z_height);
	glVertex3d(l,w,Z_height);
	glVertex3d(l,-w,Z_height);
	glEnd();
	// left
	glBegin(GL_QUADS);
	glNormal3d(-1,0,0);
	glVertex3d(-l,w,0);
	glVertex3d(-l,w,Z_height);
	glVertex3d(-l,-w,Z_height);
	glVertex3d(-l,-w,0);
	glEnd();
	// right
	glBegin(GL_QUADS);
	glNormal3d(1,0,0);
	glVertex3d(l,w,0);
	glVertex3d(l,w,Z_height);
	glVertex3d(l,-w,Z_height);
	glVertex3d(l,-w,0);
	glEnd();
	// front
	glBegin(GL_QUADS);
	glNormal3d(0,1,0);
	glVertex3d(-l,w,0);
	glVertex3d(-l,w,Z_height);
	glVertex3d(l,w,Z_height);
	glVertex3d(l,w,0);
	glEnd();
	// back
	glBegin(GL_QUADS);
	glNormal3d(0,-1,0);
	glVertex3d(-l,-w,0);
	glVertex3d(-l,-w,Z_height);
	glVertex3d(l,-w,Z_height);
	glVertex3d(l,-w,0);
	glEnd();
}

void DrawWireBox(double X_length, double Y_width, double Z_height)
{
	double l = X_length/2;
	double w = Y_width/2;
	// bottom
	glBegin(GL_LINE_LOOP);
	glVertex3d(-l,-w,0);
	glVertex3d(-l,w,0);
	glVertex3d(l,w,0);
	glVertex3d(l,-w,0);
	glEnd();
	// top
	glBegin(GL_LINE_LOOP);
	glVertex3d(-l,-w,Z_height);
	glVertex3d(-l,w,Z_height);
	glVertex3d(l,w,Z_height);
	glVertex3d(l,-w,Z_height);
	glEnd();
	// left
	glBegin(GL_LINE_LOOP);
	glVertex3d(-l,w,0);
	glVertex3d(-l,w,Z_height);
	glVertex3d(-l,-w,Z_height);
	glVertex3d(-l,-w,0);
	glEnd();
	// right
	glBegin(GL_LINE_LOOP);
	glVertex3d(l,w,0);
	glVertex3d(l,w,Z_height);
	glVertex3d(l,-w,Z_height);
	glVertex3d(l,-w,0);
	glEnd();
}

void DrawCurveBox(double width, double depth, double radius, double degreeAngle, double slices, bool hollow)
{
	double x,y;
	width /= 2;
	degreeAngle = degreeAngle*PI/180;
	slices = degreeAngle/slices;
	glBegin(GL_QUADS);
	if(!hollow)
	{
		// upper
		glNormal3d(0,0,1);
		for(double i=0; i<degreeAngle; i+=slices)
		{
			x = (radius+width)*cos(i);
			y = (radius+width)*sin(i);
			glVertex3d(x,y,depth);// 1
			x = (radius-width)*cos(i);
			y = (radius-width)*sin(i);
			glVertex3d(x,y,depth);// 2
			x = (radius-width)*cos(i+slices);
			y = (radius-width)*sin(i+slices);
			glVertex3d(x,y,depth);// 3
			x = (radius+width)*cos(i+slices);
			y = (radius+width)*sin(i+slices);
			glVertex3d(x,y,depth);// 4
		}
		// lower
		glNormal3d(0,0,-1);
		for(double i=0; i<degreeAngle; i+=slices)
		{
			x = (radius+width)*cos(i);
			y = (radius+width)*sin(i);
			glVertex3d(x,y,0);// 1
			x = (radius-width)*cos(i);
			y = (radius-width)*sin(i);
			glVertex3d(x,y,0);// 2
			x = (radius-width)*cos(i+slices);
			y = (radius-width)*sin(i+slices);
			glVertex3d(x,y,0);// 3
			x = (radius+width)*cos(i+slices);
			y = (radius+width)*sin(i+slices);
			glVertex3d(x,y,0);// 4
		}
	}
	// outside
	for(double i=0; i<degreeAngle; i+=slices)
	{
		x = (radius+width)*cos(i);
		y = (radius+width)*sin(i);
		glNormal3d(cos(i),sin(i),0);
		glVertex3d(x,y,depth);// 1
		glVertex3d(x,y,0);// 2
		x = (radius+width)*cos(i+slices);
		y = (radius+width)*sin(i+slices);
		glNormal3d(cos(i+slices),sin(i+slices),0);
		glVertex3d(x,y,0);// 3
		glVertex3d(x,y,depth);// 4
	}
	// inside
	for(double i=0; i<degreeAngle; i+=slices)
	{
		x = (radius-width)*cos(i);
		y = (radius-width)*sin(i);
		glNormal3d(-cos(i),-sin(i),0);
		glVertex3d(x,y,depth);// 1
		glVertex3d(x,y,0);// 2
		x = (radius-width)*cos(i+slices);
		y = (radius-width)*sin(i+slices);
		glNormal3d(-cos(i+slices),-sin(i+slices),0);
		glVertex3d(x,y,0);// 3
		glVertex3d(x,y,depth);// 4
	}
	// bottom
	glNormal3d(0,-1,0);
	glVertex3d(radius-width,0,0);
	glVertex3d(radius-width,0,depth);
	glVertex3d(radius+width,0,depth);
	glVertex3d(radius+width,0,0);
	// top
	glNormal3d(cos(degreeAngle+PI/2),sin(degreeAngle+PI/2),0);
	x = (radius+width)*cos(degreeAngle);
	y = (radius+width)*sin(degreeAngle);
	glVertex3d(x,y,0);// 1
	glVertex3d(x,y,depth);// 2
	x = (radius-width)*cos(degreeAngle);
	y = (radius-width)*sin(degreeAngle);
	glVertex3d(x,y,depth);// 3
	glVertex3d(x,y,0);// 4
	glEnd();
}

void DrawTextureBox(double X_length, double Y_width, double Z_height, GLuint TexNum)
{
	double l = X_length/2;
	double w = Y_width/2;
	double h = Z_height/2;
	// bottom
	glNormal3d(0,0,-1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TexNum);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0); glVertex2d(-l,-w);
	glTexCoord2d(0,1); glVertex2d(-l,w);
	glTexCoord2d(1,1); glVertex2d(l,w);
	glTexCoord2d(1,0); glVertex2d(l,-w);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	// top
	glNormal3d(0,0,1);
	glTranslated(0,0,Z_height);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, TexNum);
		glBegin(GL_QUADS);
		glTexCoord2d(0,0); glVertex2d(-l,-w);
		glTexCoord2d(0,1); glVertex2d(-l,w);
		glTexCoord2d(1,1); glVertex2d(l,w);
		glTexCoord2d(1,0); glVertex2d(l,-w);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	glTranslated(0,0,-Z_height);
	// left
	glNormal3d(-1,0,0);
	glTranslated(-l,0,h);
		glRotated(-90,0,1,0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, TexNum);
			glBegin(GL_QUADS);
			glTexCoord2d(0,0); glVertex2d(-h,w);
			glTexCoord2d(0,1); glVertex2d(h,w);
			glTexCoord2d(1,1); glVertex2d(h,-w);
			glTexCoord2d(1,0); glVertex2d(-h,-w);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		glRotated(90,0,1,0);
	glTranslated(l,0,-h);
	// right
	glNormal3d(1,0,0);
	glTranslated(l,0,h);
		glRotated(-90,0,1,0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, TexNum);
			glBegin(GL_QUADS);
			glTexCoord2d(0,0); glVertex2d(-h,w);
			glTexCoord2d(0,1); glVertex2d(h,w);
			glTexCoord2d(1,1); glVertex2d(h,-w);
			glTexCoord2d(1,0); glVertex2d(-h,-w);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		glRotated(90,0,1,0);
	glTranslated(-l,0,-h);/**/
	// front
	glNormal3d(0,1,0);
	glTranslated(0,-w,h);
		glRotated(-90,1,0,0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, TexNum);
			glBegin(GL_QUADS);
			glTexCoord2d(0,0); glVertex2d(l,h);
			glTexCoord2d(0,1); glVertex2d(l,-h);
			glTexCoord2d(1,1); glVertex2d(-l,-h);
			glTexCoord2d(1,0); glVertex2d(-l,h);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		glRotated(90,1,0,0);
	glTranslated(0,w,-h);
	// back
	glNormal3d(0,-1,0);
	glTranslated(0,w,h);
		glRotated(90,1,0,0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, TexNum);
			glBegin(GL_QUADS);
			glTexCoord2d(0,0); glVertex2d(-l,-h);
			glTexCoord2d(0,1); glVertex2d(-l,h);
			glTexCoord2d(1,1); glVertex2d(l,h);
			glTexCoord2d(1,0); glVertex2d(l,-h);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		glRotated(-90,1,0,0);
	glTranslated(0,-w,-h);
}

// Draws a rectangle with the specified coordinates
void DrawRectangle(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
{
	glBegin(GL_QUADS);
	glVertex2d(x1,y1);
	glVertex2d(x2,y1);
	glVertex2d(x2,y2);
	glVertex2d(x1,y2);
	glEnd();
}

// Draws a faded rectangle with the specified coordinates
void FadeRectangle(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2, bool horizontal)
{
	glBegin(GL_QUADS);
	if(horizontal)
	{
		glVertex2d(x1,y1);
		glVertex2d(x2,y1);
		glColor3f(1,1,1);
		glVertex2d(x2,y2);
		glVertex2d(x1,y2);
	}
	else
	{
		glVertex2d(x2,y2);
		glVertex2d(x2,y1);
		glColor3f(1,1,1);
		glVertex2d(x1,y1);
		glVertex2d(x1,y2);
	}
	glEnd();
}

void DrawTextureRectangle(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2, int texNum)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texNum);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0); glVertex2d(x1,y1);
	glTexCoord2d(0,1); glVertex2d(x1,y2);
	glTexCoord2d(1,1); glVertex2d(x2,y2);
	glTexCoord2d(1,0); glVertex2d(x2,y1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void DrawRectangle3d(point3 p1, point3 p2)
{
	/*glBegin(GL_POLYGON);
	glVertex3d(p1.x,p1.y,p1.z);
	glVertex3d(p2.x,p1.y,p1.z);
	glVertex3d(p2.x,p1.y,p1.z);
	glVertex3d(p1.x,p1.y,p2.z);
	glVertex3d(p1.x,p2.y,p2.z);
	glVertex3d(p2.x,p2.y,p2.z);
	glVertex3d(p2.x,p2.y,p1.z);
	glVertex3d(p1.x,p2.y,p1.z);
	glEnd();*/
	glBegin(GL_QUADS);
	glVertex3d(p1.x,p1.y,p1.z);
	glVertex3d(p2.x,p2.y,p1.z);
	glVertex3d(p2.x,p2.y,p2.z);
	glVertex3d(p1.x,p1.y,p2.z);
	glEnd();
}

void DrawRectangle2(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2, int status)
{
	// Fade black to left
	if (status == 1)
	{
		glBegin(GL_QUADS);
		glColor3f(.2,.2,.2);
		glVertex2d(x1,y1);
		glColor3f(.5,.5,.5);
		glVertex2d(x2,y1);
		glVertex2d(x2,y2);
		glColor3f(.2,.2,.2);
		glVertex2d(x1,y2);
		glEnd();
	}
	// Fade black to right
	if (status == 2)
	{
		glBegin(GL_QUADS);
		glColor3f(.5,.5,.5);
		glVertex2d(x1,y1);
		glColor3f(.2,.2,.2);
		glVertex2d(x2,y1);
		glVertex2d(x2,y2);
		glColor3f(.5,.5,.5);
		glVertex2d(x1,y2);
		glEnd();
	}
	// Fade black to top
	if (status == 3)
	{
		glBegin(GL_QUADS);
		glColor3f(.5,.5,.5);
		glVertex2d(x1,y1);
		glVertex2d(x2,y1);
		glColor3f(.2,.2,.2);
		glVertex2d(x2,y2);
		glVertex2d(x1,y2);
		glEnd();
	}
	// Fade black to bottom
	if (status == 4)
	{
		glBegin(GL_QUADS);
		glColor3f(.2,.2,.2);
		glVertex2d(x1,y1);
		glVertex2d(x2,y1);
		glColor3f(.5,.5,.5);
		glVertex2d(x2,y2);
		glVertex2d(x1,y2);
		glEnd();
	}
	// Silver reflection
	if (status == 5)
	{
		glBegin(GL_QUADS);
		glColor3f(1,1,1);
		glVertex2d(x1,y1);
		glColor3f(.8,.8,.8);
		glVertex2d(x2,y1);
		glColor3f(1,1,1);
		glVertex2d(x2,y2);
		glColor3f(.8,.8,.8);
		glVertex2d(x1,y2);
		glEnd();
	}
}

void DrawRoundSquare(double x, double y, double width, double height, double curve)
{
	width  /= 2;
	height /= 2;
	DrawCircle(x-width+curve,y+height-curve,curve); // Top Left
	DrawCircle(x+width-curve,y+height-curve,curve); // Top Right
	DrawCircle(x-width+curve,y-height+curve,curve); // Bottom Left
	DrawCircle(x+width-curve,y-height+curve,curve); // Bottom Right
	DrawRectangle(x-width+curve,y+height,x+width-curve,y-height); // Ver. Middle
	DrawRectangle(x-width,y+height-curve,x+width,y-height+curve); // Hor. Middle
//	FadeRectangle(x-width,y+height-curve,x+width,y,1); // Faded
}

// Draws a rectangle with rotation using two points
void RotateRectangle(double x1, double y1, double x2, double y2, double width)
{
	double theta = atan2((y2-y1),(x2-x1));
	double lengthX = sin(theta)*(width/2);
	double lengthY = cos(theta)*(width/2);
	glBegin(GL_QUADS);// Bottom solid color
	glVertex2d(x1+lengthX,y1-lengthY);	// Bottom Left
	glVertex2d(x1-lengthX,y1+lengthY);	// Top Left
	glVertex2d(x2-lengthX,y2+lengthY);	// Top Right
	glVertex2d(x2+lengthX,y2-lengthY);	// Bottom Right
	glEnd();
}

// Draws a fancy rectangle with rotation using two points
void RotateRectangle2(double x1, double y1, double x2, double y2, double width)
{
	double theta = atan2((y2-y1),(x2-x1));
	double lengthX = sin(theta)*(width/2);
	double lengthY = cos(theta)*(width/2);
	glBegin(GL_QUADS);// Bottom solid color
	glVertex2d(x1+lengthX,y1-lengthY);	// Bottom Left
	glVertex2d(x1,y1);					// Top Left
	glVertex2d(x2,y2);					// Top Right
	glVertex2d(x2+lengthX,y2-lengthY);	// Bottom Right
	glEnd();
	glBegin(GL_QUADS);// Top gradiated color
	glVertex2d(x1-lengthX,y1+lengthY);	// Top Left
	glVertex2d(x2-lengthX,y2+lengthY);	// Top Right
	glColor3f(1,1,1);
	glVertex2d(x2,y2);					// Bottom Right
	glVertex2d(x1,y1);					// Bottom Left
	glEnd();
}

bool RotatePolygon(char file[], double radAngle, int slices, bool smoothVert, bool smoothHor)
{
	ifstream fin(file);
	// Check file validity
	if(!fin)
		return 0;
	// Read in the file
	vector<point2> outline;
	point2 p;
	while(!fin.eof())
	{
		fin >> p.x >> ws >> p.y >> ws;
		outline.push_back(p);
	}
	// Draw the polygon
	RotatePolygon(outline,radAngle,slices,smoothVert,smoothHor);
	return 1;
}

bool RotatePolygon(vector<point2> outline, double radAngle, int slices, bool smoothVert, bool smoothHor)
{
	double theta, inc = radAngle/(double)slices;
	point3 LL,UR,UL,LR;
	point3 nLL, nUL, nUR, nLR;
	glBegin(GL_QUADS);
	for(double i=0; i<radAngle; i+=inc)
	{
		for(unsigned int j=0; j<outline.size()-1; j++)
		{
			LL.x=outline[j].x*cos(i);       LL.y=outline[j].x*sin(i);       LL.z=outline[j].y;   // Lower left
			UL.x=outline[j+1].x*cos(i);     UL.y=outline[j+1].x*sin(i);     UL.z=outline[j+1].y; // Upper left
			UR.x=outline[j+1].x*cos(i+inc); UR.y=outline[j+1].x*sin(i+inc); UR.z=outline[j+1].y; // Upper right
			LR.x=outline[j].x*cos(i+inc);   LR.y=outline[j].x*sin(i+inc);   LR.z=outline[j].y;   // Lower right
			nLL = nUL = nUR = nLR = FindTriangleNormal(LL, LR, UL);
			if(smoothHor)
			{
				nLL.x = outline[j].x - outline[j+1].x;
				nLL.y = outline[j].y - outline[j+1].y;
				theta = -atan2(nLL.x,nLL.y);

				nLL.x = nUL.x = cos(theta)*cos(i);
				nLL.y = nUL.y = cos(theta)*sin(i);
				nLL.z = nUL.z = sin(theta);

				nLR.x = nUR.x = cos(theta)*cos(i+inc);
				nLR.y = nUR.y = cos(theta)*sin(i+inc);
				nLR.z = nUR.z = sin(theta);
			}
			if(smoothVert)
			{
				if(j < outline.size()-1)
				{
					nUL.z = (nUL.z + sin(outline[j].y - outline[j+1].y))/2;
					nUR.z = (nUR.z + sin(outline[j].x - outline[j+1].x))/2;
					theta = atan2(nUL.z, nUR.z);
					nUL.z = nUR.z = sin(theta);
				}
				if(j > 0)
				{
					nLL.z = (nLL.z + sin(outline[j-1].y - outline[j].y))/2;
					nLR.z = (nLR.z + sin(outline[j-1].x - outline[j].x))/2;
					theta = atan2(nLL.z, nLR.z);
					nUL.z = nUR.z = sin(theta);
				}
			}
			glNormal3d(nLL.x, nLL.y, nLL.z);
			glVertex3d( LL.x,  LL.y,  LL.z);// Lower left
			glNormal3d(nUL.x, nUL.y, nUL.z);
			glVertex3d( UL.x,  UL.y,  UL.z);// Upper left
			glNormal3d(nUR.x, nUR.y, nUR.z);
			glVertex3d( UR.x,  UR.y,  UR.z);// Upper right
			glNormal3d(nLR.x, nLR.y, nLR.z);
			glVertex3d( LR.x,  LR.y,  LR.z);// Lower right
		}
	}
	glEnd();
	return 1;
}

// Draws a triangle with the specified coordinates
void DrawTriangle(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2, GLdouble x3, GLdouble y3)
{
	glBegin(GL_TRIANGLES);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
	glEnd();
}

// Draws a line with the specified coordinates
void DrawLine(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
{
	glBegin(GL_LINES);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glEnd();
}

void DrawLine3d(GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2)
{
	glBegin(GL_LINES);
	glVertex3d(x1,y1,z1);
	glVertex3d(x2,y2,z2);
	glEnd();
}
