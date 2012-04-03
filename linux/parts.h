#ifndef PARTS_H
#define PARTS_H

#include "shapes.h"
#include "colors.h"

enum displayList {
	MAINARM=100,
	PIVOTARM,
	CAM,
	LARGEGEAR,
	SMALLGEAR,
	PLUG,
	ROTOR
};

void DrawToothShape();// gear tooth shape
void DrawEdge(double x1, double y1, double x2, double y2);
void DrawToothEdge(double width);
void DrawTooth(double width);
void DrawToothBetter(double width);
void DrawGear(double outRadius, double inRadius, double width, int teeth);

void DrawArm(double length, double width, double height);
void DrawTexArm(double length, double width, double height, int texNum);
void DrawCam(double radius, double height);
void DrawRotor(double radius);
void DrawStages(double angle1, double angle2, double radius);

vector<point2> createPlug();
void DrawPlug(int slices);

double DrawArms(double armLength, double camLength, double rtt, bool inside);
void DrawArms(double armLength, double armAngle, double camLength, double camAngle, double conLength, double conAngle, bool inside);

double DrawMYT(double speed, bool stages);
void InitDisplayLists();

// Law of cosines
//double LOCangle(double sideA, double sideB, double sideC); // Returns angle (in radians) opposite to sideA;
//double LOCside(double angleA, double sideB, double sideC); // Returns length opposite to angleA (radian angle);
#endif
