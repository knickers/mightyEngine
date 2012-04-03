#ifndef SHAPES_H
#define SHAPES_H

#include <vector>
using namespace std;
#include "graphics.h"

struct point2 {double x,y;};
struct point3 {double x,y,z;};

void FindTriangleNormal(double x[], double y[], double z[], double n[]);
point3 FindTriangleNormal(point3 cp, point3 p1, point3 p2);
void SpherePoint(double radius, double upAngle, double panAngle, double point_out[3]);
void SpherePoint(double radius, double upAngle, double panAngle, point3 &point_out);
// Vector Math
void CrossProduct(double v1[],double v2[],double r[]);
void UnitVector(double v[], double dest[]);
double DotProduct(double v1[], double v2[]);
// Law of Cosines
double LOCangle(double sideA, double sideB, double sideC); // Returns angle (in radians) opposite to sideA;
double LOCside(double angleA, double sideB, double sideC); // Returns length opposite to angleA (radian angle);

void DrawAxis(double length, float xColor[], float yColor[], float zColor[]);
void DrawCircle(double x1, double y1, double radius);
void DrawCircle(double x1, double y1, double radius, float insideColor[], float outsideColor[]);
void DrawCircle2(double x1, double y1, double radius);
void DrawCircle3(double x1, double y1, double radius);
void DrawSolidCylinder(double length, double xR, double yR, double slices, bool hollow);
void DrawSolidCylinder(double length, double radius, double slices, bool hollow);
void DrawWireCylinder(double length, double radius, int slices, bool hollow);
void DrawTaperCylinder(double length, double bottomRadius, double topRadius, int slices, bool hollow);
void DrawCurveCylinder(double innerRadius, double outerRadius, double angle, int stacks, int slices, bool hollow);
void DrawPartialCylinder(double length, double radius, double angle, double slices, bool hollow);
void DrawSolidBox(double X_length, double Y_width, double Z_height);
void DrawWireBox(double X_length, double Y_width, double Z_height);
void DrawCurveBox(double width, double depth, double radius, double degreeAngle, double slices, bool hollow);
void DrawTextureBox(double X_length, double Y_width, double Z_height, GLuint TexNum);
void DrawRectangle(double x1, double y1, double x2, double y2);
void FadeRectangle(double x1, double y1, double x2, double y2, bool horizontal);
void DrawTextureRectangle(double x1, double y1, double x2, double y2, int texNum);
void DrawRectangle3d(point3 p1, point3 p2);
void DrawRectangle2(double x1, double y1, double x2, double y2, int status);
void DrawRoundSquare(double x, double y, double width, double height, double curve);
void RotateRectangle(double x1, double y1, double x2, double y2, double width);
void RotateRectangle2(double x1, double y1, double x2, double y2, double width);
bool RotatePolygon(char file[], double radAngle, int slices, bool smoothVert, bool smoothHor);
bool RotatePolygon(vector<point2> outline, double radAngle, int slices, bool smoothVert, bool smoothHor);
void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3);
void DrawLine(double x1, double y1, double x2, double y2);
void DrawLine3d(double x1, double y1, double z1, double x2, double y2, double z2);

#endif
