#pragma once

#include <cmath>
#include <vector>
#include <ctime>
using namespace std;
#include "shapes.h"
#include "colors.h"
#include "fraction.h"

enum displayList {
	MAINARM=100,
	PIVOTARM,
	CAM,
	LARGEGEAR,
	SMALLGEAR,
	PLUG,
	ROTOR
};

class tooth {
private:
	point2 mP[];
	double mA[];
	double mWidth;
	void DrawFace();
	void DrawEdge(int i, int j, float nx, float ny);
	void DrawEdges();
public:
	tooth();
	void Draw();
	void SetWidth(double w);
	double GetArcLength();
};

class MYT {
private:
	clock_t mTime;
	bool mShowStages;
	int mVanes, mFrame, mNumPlugs;
	tooth mTooth;
	double mSize, mRotate, mTDC, mRPM, mLargeGear, mSmallGear, mGearWidth;
	double mCylRadius, mArmWidth, mArmHeight, mCamLength, mConLength,mArcLength;
	double mBlueAngle, mTanAngle, mBlueOffset, mTanOffset, mGearOffset;
	double *mPlugAngles;
	double *mPortAngles;

	void CalcValues();
	void SetOffset();
	void ResizePointers(int size);
	void DrawToothFace();
	void DrawToothEdge(double x1,double y1,double x2,double y2,float nx,float ny);
	void DrawToothEdges();
	void DrawTooth();
	void DrawGear(double outRadius, double inRadius, int teeth);
	void DrawArmShape(double length);
	void DrawArm(double length);
	void DrawCam(double height);
	void DrawRotor();
	void DrawStages(double angle1, double angle2);
	vector<point2> createPlug();
	void DrawPlug(int slices);
	double DrawHalfSup(bool inside);
	void DrawHalfSup(double armAngle, double camAngle, double conAngle, bool inside);
public:
	MYT();
	~MYT();
	void Move(double speed);
	void Draw();
	void DrawMenu();
	void InitDisplayLists();
	void SetSize(double size);
	void SetStages(bool show);
	void SetNumVanes(int num);
	double GetRPM();
};
