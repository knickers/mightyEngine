#include "MYT.h"

MYT::MYT() {
	mVanes = 4;   // Number of pistons per rotor
	mSize  = 100; // Length of arms
	SetNumVanes(mVanes);
}

MYT::~MYT() {
	delete mPlugAngles;
	delete mPortAngles;
}

/*	1.  size = large + small // Arm radius = large gear + small gear
	2. large = vanes * small // Large gear is vanes times bigger than small gear

	size-small = vanes*small
		  size = (vanes+1)*small
		 small = size/(vanes+1)*/
void MYT::CalcValues() {
	mSmallGear = mSize/(mVanes+1);    // Planet gear radius
	mLargeGear = mSize-mSmallGear;    // Sun gear radius
	mCamLength = mSmallGear - 3;      // Camshaft radius
	mCylRadius = mSize / 2.5;         // Inside radius of toroidal cylinder
	mGearWidth = mSize / 8;           // Width of the gears
	mArmWidth  = mSize * .15;         // Width of the arms
	mArmHeight = mSize * .075;        // Height of the arms
	mConLength = sqrt(2*mSize*mSize); // Length of connecting rod
	mConLength = sqrt(mCamLength*mCamLength+mConLength*mConLength);
	mRotate    = 0;                   // Total output rotation
	mFrame     = 0;                   // Counts the number of frames for RPM
	mTDC       = 0;                   // Top Dead Center angle for spark plug
	mRPM       = 0;                   // Current Revolutions Per Minute
	mTime      = clock();             // Used for calculating RPM
	mInnerAngle = 0;                  // Angle of blue rotor
	mOuterAngle = 0;                  // Angle of tan rotor
}

void MYT::SetOffset() {
	int p = 360/mNumPlugs;
	for (int i=0; i<mNumPlugs; i++) {
		mPlugAngles[i] = i*p;
		mPortAngles[i] = i*p + p/2;
		if(mVanes==4 || mVanes==8) {
			mPlugAngles[i] += p/2;
			mPortAngles[i] += p/2;
		}
	}
	if (mVanes == 2) {
		mInnerOffset = 24;
		mOuterOffset = -66;
		mGearOffset  = -1;
	} else if (mVanes == 4) {
		mInnerOffset = 11;
		mOuterOffset = -34;
		mGearOffset  = 5;
	} else if (mVanes == 6) {
		mInnerOffset = 10;
		mOuterOffset = -20;
		mGearOffset  = -4;
	} else if (mVanes == 8) {
		mInnerOffset = 8.5;
		mOuterOffset = -14;
		mGearOffset  = 9;
	} else if (mVanes == 10) {
		mInnerOffset = 7;
		mOuterOffset = -11;
		mGearOffset  = 13;
	}
}

void MYT::ResizePointers(int size) {
	delete mPlugAngles;
	delete mPortAngles;
	mPlugAngles = new double[size];
	mPortAngles = new double[size];
}

void MYT::DrawToothShape() {
	glBegin(GL_POLYGON);
	glVertex2d(-1.2,1.2);
	glVertex2d(1,.7);
	glVertex2d(1.5,.3);
	glVertex2d(1.5,-.3);
	glVertex2d(1,-.7);
	glVertex2d(-1.2,-1.2);
	glVertex2d(-1.5,-1.5);
	glVertex2d(-1.5,1.5);
	glEnd();
}

void MYT::DrawEdge(double x1,double y1,double x2,double y2,float nx,float ny) {
	glBegin(GL_QUADS);
	glVertex3d(x1,y1, mGearWidth);
	glVertex3d(x1,y1,-mGearWidth);
	glNormal3f(nx, ny, 0);
	glVertex3d(x2,y2,-mGearWidth);
	glVertex3d(x2,y2, mGearWidth);
	glEnd();
}

void MYT::DrawToothEdge() {
	//   4_
	//  3/ \5
	//  2| |6
	// 1/___\7
	glNormal3d(Cos(0), Sin(0), 0);
	DrawEdge(-1.5, +1.5, -1.2, +1.2, Cos(+57), Sin(+57)); // 1 - 45 deg
	glNormal3d(Cos(+57), Sin(+57), 0);
	DrawEdge(-1.2, +1.2, +1.0, +0.7, Cos(+60), Sin(+60)); // 2 - 70 deg
	glNormal3d(Cos(+60), Sin(+60), 0);
	DrawEdge(+1.0, +0.7, +1.5, +0.3, Cos(+25), Sin(+25)); // 3 - 50 deg
	glNormal3d(Cos(+25), Sin(+25), 0);
	DrawEdge(+1.5, +0.3, +1.5, -0.3, Cos(-25), Sin(-25)); // 4 - 00 deg
	glNormal3d(Cos(-25), Sin(-25), 0);
	DrawEdge(+1.5, -0.3, +1.0, -0.7, Cos(-60), Sin(-60)); // 5 - 50 deg
	glNormal3d(Cos(-60), Sin(-60), 0);
	DrawEdge(-1.2, -1.2, +1.0, -0.7, Cos(-57), Sin(-57)); // 6 - 70 deg
	glNormal3d(Cos(-57), Sin(-57), 0);
	DrawEdge(-1.2, -1.2, -1.5, -1.5, Cos(0), Sin(0)); // 7 - 45 deg
}

void MYT::DrawTooth() {
	// Bottom
	glNormal3d(0,0,-1);
	glTranslated(0,0,-mGearWidth);
		DrawToothShape();
	glTranslated(0,0,mGearWidth);
	// Top
	glNormal3d(0,0,1);
	glTranslated(0,0,mGearWidth);
		DrawToothShape();
	glTranslated(0,0,-mGearWidth);
	// Edges
	DrawToothEdge();
}

void MYT::DrawGear(double outRadius, double inRadius, int teeth) {
	double angle = 360.0/(double)teeth;
	double rad = outRadius - inRadius;
	// Body
	glTranslated(0,0,-mGearWidth/2);
		DrawCurveBox(rad, mGearWidth, rad/2, 360, teeth, 0);// Body
	glTranslated(0,0,-mGearWidth/2);
	DrawCurveBox(rad/4, mGearWidth*2, inRadius+rad/8, 360, teeth, 0);// Bearing Collar
	DrawCurveBox(rad/4, mGearWidth*2, outRadius-rad/8-1.4, 360, teeth*2, 0);// Tooth Collar
	glTranslated(0, 0, mGearWidth);
	// Teeth
	for (double i=0; i<360.0; i+=angle) {
		glPushMatrix();
		glRotated(i,0,0,1);
			glTranslated(outRadius,0,0);
			DrawTooth();
		glPopMatrix();
	}
}

void MYT::DrawArmShape(double length) {
	glBegin(GL_POLYGON);
	glVertex3d(length*.35, 0, mArmHeight);
	glVertex3d(0         , 0, mArmHeight);
	glVertex3d(0         , 0, 0);
	glVertex3d(length*.4 , 0, 0);
	glVertex3d(length*.5 , 0, mArmHeight);
	glVertex3d(length    , 0, mArmHeight);
	glVertex3d(length    , 0, mArmHeight*2);
	glVertex3d(length*.45, 0, mArmHeight*2);
	glEnd();
}

void MYT::DrawArm(double length) {
	glPushMatrix();
	glTranslated(0, -mArmWidth, 0);
		glNormal3d(0,-1,0);
		DrawArmShape(length); // Close
		glTranslated(0, mArmWidth*2, 0);
			glNormal3d(0,1,0);
			DrawArmShape(length); // Far
	glPopMatrix();
	glNormal3d(0,0,-1);
	DrawRectangle(0, -mArmWidth, length*.4, mArmWidth); // Low Bottom
	glTranslated(0,0,mArmHeight);
		DrawRectangle(length*.5, -mArmWidth, length, mArmWidth); // High Bottom
		glNormal3d(0,0,1);
		DrawRectangle(0, -mArmWidth, length*.35, mArmWidth); // Low Top
		glTranslated(0,0,mArmHeight);
			DrawRectangle(length*.45, -mArmWidth, length, mArmWidth); // High Top
	glTranslated(0,0,-mArmHeight*2);
	glBegin(GL_QUADS); // Low Diagonal
	glNormal3d(.766,0,-.643);
	glVertex3d(length*.4,-mArmWidth,0);
	glVertex3d(length*.5,-mArmWidth,mArmHeight);
	glVertex3d(length*.5,mArmWidth,mArmHeight);
	glVertex3d(length*.4,mArmWidth,0);
	glEnd();
	glBegin(GL_QUADS); // High Diagonal
	glNormal3d(-.766,0,.643);
	glVertex3d(length*.35,-mArmWidth,mArmHeight);
	glVertex3d(length*.45,-mArmWidth,mArmHeight*2);
	glVertex3d(length*.45,mArmWidth,mArmHeight*2);
	glVertex3d(length*.35,mArmWidth,mArmHeight);
	glEnd();
	glPushMatrix();
	glRotated(90, 0,1,0);
		glNormal3d(0,0,-1);
		DrawRectangle(-mArmHeight, -mArmWidth, 0, mArmWidth); // Long End
		glTranslated(-mArmHeight,0,length);
			glNormal3d(0,0,1);
			DrawRectangle(-mArmHeight, -mArmWidth, 0, mArmWidth); // Short End
	glPopMatrix();
}

void MYT::DrawCam(double height) {
	glPushMatrix();
	glTranslated(0,0,height/2);
		DrawSolidCylinder(5,mCamLength+6,35,0);// top disc
		DrawSolidCylinder(23,5,mSize/5,0);// top shaft
		glTranslated(0,0,-height);
			DrawSolidCylinder(-mSize/2,5,mSize/5,0);// bottom shaft
			DrawSolidCylinder(-5,mCamLength+6,35,0);// bottom disc
			glTranslated(mCamLength,0,0);
				DrawSolidCylinder(height,5,mSize/5,0);// cam shaft
	glPopMatrix();
}

void MYT::DrawRotor() {
	double deg = 360.0/mVanes;
	double wid = (10.0-mVanes)/10*13 + 6;
	glPushMatrix();
	glRotated(wid/2, 1,0,0);
		glRotated(-90, 0,1,0);
			glTranslated(0,0,mCylRadius*.2);
			DrawTaperCylinder(mCylRadius/1.75,mCylRadius*1.04,mCylRadius*1.37,30,0);
			glTranslated(0,0,-mCylRadius*.2);
			for (int i=0; i<mVanes; i++) {
				glRotated(deg,0,0,1);
				DrawCurveCylinder(mCylRadius,mSize*1.2-mCylRadius,wid,20,2,0);
			}
	glPopMatrix();
}

void MYT::DrawStages(double angle1, double angle2) {
	double s = angle1-angle2;
	int t = 2;
	glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glRotated(-90,0,1,0);
			if ((angle1 <= -5 && angle1 > -16)    || (angle1 <= -185 && angle1 > -196) ||
				(angle1 <= -180 && angle1 > -185) || (angle1 <= -360 && angle1 > -365))    // Intake
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transBlue);
			else if((angle1 <= -16 && angle1 > -85) || (angle1 <= -196 && angle1 > -265))  // Compression
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transNavy);
			else if((angle1 <= -85 && angle1 > -90) || (angle1 <= -265 && angle1 > -270))  // Ignition
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
			else if((angle1 <= -90 && angle1 > -105) || (angle1 <= -270 && angle1 > -285)) // Expansion
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transRed);
			else if((angle1 <= -105 && angle1 > -175) || (angle1 <= -285 && angle1 > -355))// Exaust
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transOrange);
			else if((angle1 <= -175 && angle1 > -180) || (angle1 <= -290 && angle1 > -360))// Between
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, clear);

			DrawCurveCylinder(mCylRadius,120-mCylRadius,s,20,t,0);
			glRotated(180,0,0,1);
			DrawCurveCylinder(mCylRadius,120-mCylRadius,s,20,t,0);
			glRotated(-90,0,0,1);

			if ((angle1 <= -5 && angle1 > -16)    || (angle1 <= -185 && angle1 > -196) ||
				(angle1 <= -180 && angle1 > -185) || (angle1 <= -360 && angle1 > -365))    // Expansion
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transRed);
			else if((angle1 <= -16 && angle1 > -85) || (angle1 <= -196 && angle1 > -265))  // Exaust
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transOrange);
			else if((angle1 <= -85 && angle1 > -90) || (angle1 <= -265 && angle1 > -270))  // Between
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, clear);
			else if((angle1 <= -90 && angle1 > -105) || (angle1 <= -270 && angle1 > -285)) // Intake
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transBlue);
			else if((angle1 <= -105 && angle1 > -175) || (angle1 <= -285 && angle1 > -355))// Compression
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transNavy);
			else if((angle1 <= -175 && angle1 > -180) || (angle1 <= -290 && angle1 > -360))// Ignition
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);

			DrawCurveCylinder(mCylRadius,120-mCylRadius,s,20,t,0);
			glRotated(-180,0,0,1);
			DrawCurveCylinder(mCylRadius,120-mCylRadius,s,20,t,0);
			// Change to the opposite angles
			glRotated(s+14,0,0,1);
			s = 62-s;

			if ((angle1 <= 0 && angle1 > -14)    || (angle1 <= -180 && angle1 > -194) ||
				(angle1 <= -180 && angle1 > -185) || (angle1 <= -360 && angle1 > -365))    // Compression
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transNavy);
			else if((angle1 <= -14 && angle1 > -20) || (angle1 <= -194 && angle1 > -200))  // Ignition
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
			else if((angle1 <= -20 && angle1 > -85) || (angle1 <= -200 && angle1 > -265))  // Expansion
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transRed);
			else if((angle1 <= -85 && angle1 > -100) || (angle1 <= -265 && angle1 > -280)) // Exaust
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transOrange);
			else if((angle1 <= -100 && angle1 > -110) || (angle1 <= -280 && angle1 > -290))// Between
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, clear);
			else if((angle1 <= -110 && angle1 > -180) || (angle1 <= -290 && angle1 > -360))// Intake
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transBlue);

			DrawCurveCylinder(mCylRadius,120-mCylRadius,s,20,t,0);
			glRotated(180,0,0,1);
			DrawCurveCylinder(mCylRadius,120-mCylRadius,s,20,t,0);
			glRotated(-90,0,0,1);

			if ((angle1 <= 0 && angle1 > -14)    || (angle1 <= -180 && angle1 > -194) ||
				(angle1 <= -180 && angle1 > -185) || (angle1 <= -360 && angle1 > -365))    // Exaust
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transOrange);
			else if((angle1 <= -14 && angle1 > -20) || (angle1 <= -194 && angle1 > -200))  // Between
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, clear);
			else if((angle1 <= -20 && angle1 > -85) || (angle1 <= -200 && angle1 > -265))  // Intake
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transBlue);
			else if((angle1 <= -85 && angle1 > -100) || (angle1 <= -265 && angle1 > -280)) // Compression
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transNavy);
			else if((angle1 <= -100 && angle1 > -110) || (angle1 <= -280 && angle1 > -290))// Ignition
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
			else if((angle1 <= -110 && angle1 > -180) || (angle1 <= -290 && angle1 > -360))// Expansion
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transRed);

			DrawCurveCylinder(mCylRadius,120-mCylRadius,s,20,t,0);
			glRotated(-180,0,0,1);
			DrawCurveCylinder(mCylRadius,120-mCylRadius,s,20,t,0);
		glDisable(GL_BLEND);
	glPopMatrix();
}

vector<point2> MYT::createPlug() {
	vector<point2> outline;
	point2 p;
	p.x=0;   p.y=20; outline.push_back(p);
	p.x=2;   p.y=20; outline.push_back(p);
	p.x=3;   p.y=19; outline.push_back(p);
	p.x=3;   p.y=18; outline.push_back(p);
	p.x=2;   p.y=17; outline.push_back(p);
	p.x=2;   p.y=13; outline.push_back(p);
	p.x=3.5; p.y=12; outline.push_back(p);
	p.x=3.5; p.y=4;  outline.push_back(p);
	p.x=5;   p.y=4;  outline.push_back(p);
	p.x=5;   p.y=0;  outline.push_back(p);
	return outline;
}

void MYT::DrawPlug(int slices) {
	static vector<point2> outline = createPlug();
	RotatePolygon(outline,2*PI,slices, 0, 1);
}

double MYT::DrawHalfSup(bool inside) {
	double camAngle = mRotate * mVanes;
	if (inside)
		camAngle += 180;
	while(camAngle >= 360) // Keep camAngle less that 360
		camAngle -= 360;
	// Arm angle math
	double h, armAngle, conAngle;
	h = LOCside((camAngle+90)*PI/180,mSize,mCamLength);	// Hypotinus length
	armAngle  = LOCangle(mCamLength,mSize,h)*180/PI;	// Degree angle from arm to hypotinus
	if (camAngle >= 90 && camAngle <= 270)				// Check if cam is left of y axis 
		armAngle = -armAngle;
	armAngle += LOCangle(mConLength,mSize,h)*180/PI;	// Add degree angle from hypotinus to pivot arm
	armAngle  = 90 - armAngle;							// Degree armAngle for glut glRotated()
	conAngle  = LOCangle(h,mSize,mConLength)*180/PI;	// Degree conAngle for glut glRotated()
	conAngle  = 180 + armAngle - conAngle;
	
	DrawHalfSup(armAngle,camAngle,conAngle,inside);
	return armAngle;
}

void MYT::DrawHalfSup(double armAngle, double camAngle, double conAngle, bool inside) {
	// Pivot Arm
	glPushMatrix();
		glRotated(armAngle,0,0,1);
			glTranslated(-15,0,0);
			if (inside)
				glCallList(PIVOTARM);
			else { // outside
				glTranslated(0,0,-27.5);
					glCallList(PIVOTARM);
				glTranslated(0,0,55);
			}
			glRotated(180,1,0,0);
			glCallList(PIVOTARM);
	glPopMatrix();
	// Main Arm
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightGray);
	glPushMatrix();
		glRotated(90,0,0,1);
			glTranslated(0,0,10);
				glCallList(MAINARM);
				glTranslated(0,0,-20);
					glRotated(180,1,0,0);
						glCallList(MAINARM);
	glPopMatrix();
	// Cam shaft
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, silver);
	glPushMatrix();
		glTranslated(0,mSize,0);
			glRotated(camAngle,0,0,1);
				glCallList(CAM);
	glPopMatrix();
	// Pivot Arm pin
	glPushMatrix();
	glRotated(armAngle,0,0,1);
		glTranslated(mSize,0,-14);
			DrawSolidCylinder(28,5,10,1);
	glPopMatrix();
	// Connecting rod
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
	glPushMatrix();
		glTranslated(mSize*cos(armAngle*PI/180),mSize*sin(armAngle*PI/180),0);
			glRotated(conAngle-90,0,0,1);
				glTranslated(-mSize/10,mConLength/2,0);
					glRotated(90,0,1,0);
						DrawSolidBox(8,mConLength+20,20);
	glPopMatrix();
}

void MYT::InitDisplayLists() {
	glNewList(MAINARM,GL_COMPILE);
	DrawArm(mSize * 1.15); // 115
	glEndList();

	glNewList(PIVOTARM,GL_COMPILE);
	DrawArm(mSize * 1.3); // 130
	glEndList();

	glNewList(CAM,GL_COMPILE);
	DrawCam(mSize / 5); // 20
	glEndList();

	glNewList(SMALLGEAR,GL_COMPILE);
	DrawGear(mSmallGear, mSize/20, mSmallGear*1.8); // 4 vanes = (20, 5, 36)
	glEndList();

	glNewList(LARGEGEAR,GL_COMPILE);
	DrawGear(mLargeGear, mSize/10, mLargeGear*1.8); // 4 vanes = (80,10,144)
	glEndList();

	glNewList(PLUG,GL_COMPILE);
	DrawPlug(36);
	glEndList();

	glNewList(ROTOR,GL_COMPILE);
	DrawRotor();
	glEndList();
}

void MYT::Move(double speed) {
	mRotate += speed;
	if (mRotate >= 360.0)
		mRotate -= 360.0;
	if (mFrame >= 10) {
		mFrame = 0;
		mRPM   = clock() - mTime;
		mRPM   = 600000*speed/mRPM/360;
		mTime  = clock();
	} else
		mFrame++;
}

void MYT::Draw() {
	// Gears
	glPushMatrix();
	glRotated(90,0,1,0);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, silver);
		glPushMatrix();
			glTranslated(0,0,3);
				glCallList(LARGEGEAR);
		glPopMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
		glPushMatrix();
			glRotated(-mRotate,0,0,1);
				glTranslated(mSize,0,3);
					glRotated(-mVanes*mRotate+mGearOffset,0,0,1);
						glCallList(SMALLGEAR);
		glPopMatrix();
		glPushMatrix();
			glRotated(-mRotate,0,0,1);
				glTranslated(-mSize,0,3);
					glRotated(-mVanes*mRotate+mGearOffset,0,0,1);
						glCallList(SMALLGEAR);
		glPopMatrix();
	glPopMatrix();
	// Arms, Cams, Connecting rods
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
	glPushMatrix();
	glTranslated(-40,0,0);
		glRotated(-90,0,1,0);
			glRotated(mRotate+90,0,0,1);
				mInnerAngle = DrawHalfSup(1);
				mInnerAngle = -mRotate-mInnerAngle;
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ivery);
	glPushMatrix();
	glTranslated(-40,0,0);
		glRotated(-90,0,1,0);
			glRotated(mRotate+270,0,0,1);
				mOuterAngle = DrawHalfSup(0);
				mOuterAngle = -mRotate-mOuterAngle;
	glPopMatrix();
	// torus cylinder
	glPushMatrix();
		glTranslated(mCylRadius+20,0,0);
			glRotated(90,0,1,0);
				glutWireTorus(mCylRadius,mSize*1.2-mCylRadius,15,40);
	glPopMatrix();
	// rotors
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ivery);
	glPushMatrix();
		glRotated(mOuterAngle + mOuterOffset,1,0,0);
			glTranslated(60,0,0);
				glCallList(ROTOR);
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
	glPushMatrix();
		glRotated(mInnerAngle + mInnerOffset,1,0,0);
			glTranslated(60,0,0);
				glRotated(180,0,0,1);
					glCallList(ROTOR);
	glPopMatrix();
	// Cumbustion cycles
	if (mShowStages) {
		glPushMatrix();
			glRotated(mOuterAngle,1,0,0);
				glTranslated(60,0,0);
					glRotated(180,0,0,1);
						DrawStages(mInnerAngle, mOuterAngle);
		glPopMatrix();
	}
	glPopMatrix();
	// ports
	double wid = (10.0-mVanes)/10*30 + 7;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightBlue);
	for (int i=0; i<mNumPlugs; i++) { // intake
		glPushMatrix();
			glRotated(mPortAngles[i]-wid/1.68,1,0,0);
				glTranslated(mCylRadius+20,0,mSize*1.18);
					DrawSolidCylinder(10,15,wid,15,1);
		glPopMatrix();
	}
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
	for (int i=0; i<mNumPlugs; i++) { // exhaust
		glPushMatrix();
			glRotated(mPortAngles[i]+wid/1.68,1,0,0);
				glTranslated(mCylRadius+20,0,mSize*1.18);
					DrawSolidCylinder(10,15,wid,15,1);
		glPopMatrix();
	}
	for (int i=0; i<mNumPlugs; i++) { // spark plugs
		glPushMatrix();
			glRotated(mPlugAngles[i]+mTDC,1,0,0);
				glTranslated(mCylRadius+20,0,mSize*1.2);
					glCallList(PLUG);
		glPopMatrix();
	}
	// Output shaft
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightGray);
	glPushMatrix();
		glRotated(90,0,1,0);
			glTranslated(0,0,-80);
				DrawSolidCylinder(mSize*2,mSize/20,20,1);
	glPopMatrix();
	glPushMatrix();
		glTranslated(mSize*1.2,0,0);
			glutSolidSphere(mSize/20,20,20);
			glTranslated(-mSize*2,0,0);
				glutSolidSphere(mSize/20,20,20);
	glPopMatrix();
	glPushMatrix();
		glRotated(-mRotate,1,0,0);
			glTranslated(-80,0,0);
				DrawSolidCylinder(30,mSize/20,20,0);
				glTranslated(mSize*2,0,0);
					DrawSolidCylinder(30,mSize/20,20,0);
	glPopMatrix();
	// Blue shaft
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightBlue);
	glPushMatrix();
		glRotated(90,0,1,0);
			glTranslated(0,0,-40);
				DrawSolidCylinder(mSize*1.2,mSize/10,20,1);
	glPopMatrix();
	// Tan shaft
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ivery);
	glPushMatrix();
		glRotated(90,0,1,0);
			glTranslated(0,0,-20);
				DrawSolidCylinder(60,15,20,1);
	glPopMatrix();
}

void MYT::DrawMenu() { // Menu to display instructions
	double L = screen_x * 1/5;
	double R = screen_x * 4/5;
	double T = screen_y * 4/5;
	double B = screen_y * 1/5;
	double P = screen_y / 50;
	if(screen_x < screen_y)
		P = screen_y / 50;
	// Border circles
	DrawCircle(L+P, T-P, P, red, white);
	DrawCircle(R-P, T-P, P, white, red);
	DrawCircle(R-P, B+P, P, white, red);
	DrawCircle(L+P, B+P, P, white, red);
	// Border bars
	glColor3fv(red); // Left
	FadeRectangle(L, B+P, L+P, T-P, 0);
	glColor3fv(red); // Right
	FadeRectangle(R-P, B+P, R, T-P, 0);
	glColor3fv(red); // Top
	FadeRectangle(L+P, T-P, R-P, T, 1);
	glColor3fv(red); // Bottom
	FadeRectangle(L+P, B, R-P, B+P, 1);
	// Background
	glColor3fv(black);
	DrawRectangle(L+P, B+P, R-P, T-P);
	// Text
	glColor3fv(white);
	text_output(L+2.25*P,  T-3*P, "- Press 'tab' to toggle this menu.");
	text_output(L+2.25*P,  T-5*P, "- Use the arrow keys or click and drag to rotate the model.");
	text_output(L+2.25*P,  T-7*P, "- Drag the 'speed' and 'zoom' slider bars at the left up and down");
	text_output(L+2.25*P,  T-9*P, "- Choose the number of vanes with the number keys.");
	text_output(L+2.25*P, T-11*P, "");
}

void MYT::SetSize(double size) {
	mSize=size;
}

void MYT::SetStages(bool show) {
	mShowStages=show;
}

void MYT::SetNumVanes(int num) {
	if (num % 2 == 0) {
		mVanes = num;
		if (num == 0)
			mVanes = 10;
		mNumPlugs = mVanes/2;
		ResizePointers(mNumPlugs);
		CalcValues();
		SetOffset();
		InitDisplayLists();
	}
}

double MYT::GetRPM() {
	return mRPM;
}
