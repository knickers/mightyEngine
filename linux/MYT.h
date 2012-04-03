#pragma once

#include <cmath>
#include <vector>
#include <ctime>
using namespace std;
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

class MYT {
private:
	clock_t mTime;
	bool mShowStages;
	int mVanes, mFrame, mNumPlugs;
	double mSize, mRotate, mTDC, mRPM, mLargeGear, mSmallGear, mGearWidth;
	double mCamLength, mCylRadius, mArmWidth, mArmHeight, mConLength;
	double mInnerAngle, mOuterAngle, mInnerOffset, mOuterOffset, mGearOffset;
	double *mPlugAngles;
	double *mPortAngles;

	void CalcValues();
	void SetOffset();
	void ResizePointers(int size);
	void DrawToothShape();
	void DrawEdge(double x1, double y1, double x2, double y2);
	void DrawToothEdge();
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
