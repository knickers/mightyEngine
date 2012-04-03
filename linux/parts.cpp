#include <cmath>
#include <vector>
#include <ctime>
using namespace std;
#include "parts.h"
#include "shapes.h"

void DrawTooth(double width) {
	width /= 2;
	// Bottom
	glNormal3d(0,0,-1);
	glTranslated(0,0,-width);
	DrawTriangle(-1.5,-1.7,1.5,0,-1.5,1.7);
	glTranslated(0,0,width);
	// Top
	glNormal3d(0,0,1);
	glTranslated(0,0,width);
	DrawTriangle(-1.5,-1.7,1.5,0,-1.5,1.7);
	glTranslated(0,0,-width);
	// Close
	glBegin(GL_QUADS);
	glNormal3d(.5,-.866,0);
	glVertex3d(-1.5,-1.7,-width);
	glVertex3d(1.5,0,-width);
	glVertex3d(1.5,0,width);
	glVertex3d(-1.5,-1.7,width);
	glEnd();
	// Far
	glBegin(GL_QUADS);
	glNormal3d(.5,.866,0);
	glVertex3d(-1.5,1.7,-width);
	glVertex3d(1.5,0,-width);
	glVertex3d(1.5,0,width);
	glVertex3d(-1.5,1.7,width);
	glEnd();
}

void DrawToothShape() {
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

void DrawEdge(double x1, double y1, double x2, double y2, double width) {
	glBegin(GL_QUADS);
	glVertex3d(x1,y1,-width);
	glVertex3d(x2,y2,-width);
	glVertex3d(x2,y2,width);
	glVertex3d(x1,y1,width);
	glEnd();
}

void DrawToothEdge(double width) {
	//    _4_
	//  3/   \5
	//  2|   |6
	// 1/_____\7

	// 1
	glNormal3d(.707,.707,0);
	DrawEdge(-1.5,1.5,-1.2,1.2,width);
	// 2
	glNormal3d(.342,.9397,0);
	DrawEdge(-1.2,1.2,1,.7,width);
	// 3
	glNormal3d(.643,.766,0);
	DrawEdge(1,.7,1.5,.3,width);
	// 4
	glNormal3d(1,0,0);
	DrawEdge(1.5,.3,1.5,-.3,width);
	// 5
	glNormal3d(.643,-.766,0);
	DrawEdge(1,-.7,1.5,-.3,width);
	// 6
	glNormal3d(.342,-.9397,0);
	DrawEdge(-1.2,-1.2,1,-.7,width);
	// 7
	glNormal3d(.707,-.707,0);
	DrawEdge(-1.5,-1.5,-1.2,-1.2,width);
}

void DrawToothBetter(double width) {
	width /= 2;

	// Bottom
	glNormal3d(0,0,-1);
	glTranslated(0,0,-width);
		DrawToothShape();
	glTranslated(0,0,width);
	// Top
	glNormal3d(0,0,1);
	glTranslated(0,0,width);
		DrawToothShape();
	glTranslated(0,0,-width);
	// Edges
	DrawToothEdge(width);
}

void DrawGear(double outRadius, double inRadius, double width, int teeth) {
	double angle = 360.0/(double)teeth;

	// Body
	glTranslated(0,0,-width/4);
	DrawCurveBox(outRadius-10,width/2,outRadius*.5,360,teeth,0);// Body
	glTranslated(0,0,-width/4);
	DrawCurveBox(inRadius,width,inRadius*1.5,360,teeth,0);// Bearing Collar
	DrawCurveBox(5,width,outRadius-3.8,360,teeth*2,0);// Tooth Support
	glTranslated(0,0,width/2);

	// Teeth
	for(double i=0; i<360.0; i+=angle) {
		glPushMatrix();
		glRotated(i,0,0,1);
			glTranslated(outRadius,0,0);
			DrawToothBetter(width);
		glPopMatrix();
	}
}

void DrawTexArm(double length, double width, double height, int texNum) {
	double w = width/2;
	double h = height/2;
	// Short bottom
	glNormal3d(0,0,-1);
	DrawTextureRectangle(0,-w,length*.4,w,texNum+1);
	// Short top
	glNormal3d(0,0,1);
	glPushMatrix();
		glTranslated(0,0,h);
			DrawTextureRectangle(0,-w,length*.35,w,texNum+1);
	glPopMatrix();
	// Short end
	glPushMatrix();
		glRotated(-90,0,1,0);
			glNormal3d(0,0,1);
			DrawTextureRectangle(0,w,h,-w,texNum);
	glPopMatrix();
	// Short close
	glPushMatrix();
		glTranslated(0,-w,0);
			glRotated(90,1,0,0);
				glNormal3d(0,0,1);
				DrawTextureRectangle(0,0,length*.4,h,texNum+2);
	glPopMatrix();
	// Short far
	glPushMatrix();
		glTranslated(0,w,0);
			glRotated(90,1,0,0);
				glNormal3d(0,0,-1);
				DrawTextureRectangle(0,0,length*.4,h,texNum+2);
	glPopMatrix();
	// Long bottom
	glNormal3d(0,0,-1);
	glPushMatrix();
		glTranslated(0,0,h);
			DrawTextureRectangle(length*.5,-w,length,w,texNum+2);
	glPopMatrix();
	// Long top
	glNormal3d(0,0,1);
	glPushMatrix();
		glTranslated(0,0,height);
			DrawTextureRectangle(length*.45,-w,length,w,texNum+2);
	glPopMatrix();
	// Long end
	glPushMatrix();
		glTranslated(length,0,h);
			glRotated(-90,0,1,0);
				glNormal3d(0,0,-1);
				DrawTextureRectangle(0,w,h,-w,texNum);
	glPopMatrix();
	// Long close
	glPushMatrix();
		glTranslated(0,-w,h);
			glRotated(90,1,0,0);
				glNormal3d(0,0,1);
				DrawTextureRectangle(length*.45,0,length,h,texNum+3);
	glPopMatrix();
	// Long far
	glPushMatrix();
		glTranslated(0,w,h);
			glRotated(90,1,0,0);
				glNormal3d(0,0,-1);
				DrawTextureRectangle(length*.45,0,length,h,texNum+3);
	glPopMatrix();
	// Diagonal bottom
	glPushMatrix();
		glTranslated(length*.4,0,0);
			glRotated(-45,0,1,0);
				glNormal3d(0,0,-1);
				DrawTextureRectangle(0,-w,length*.15,w,texNum);
	glPopMatrix();
	// Diagonal top
	glPushMatrix();
		glTranslated(length*.45,0,height);
			glRotated(-45,0,1,0);
				glNormal3d(0,0,1);
				DrawTextureRectangle(-length*.15,-w,0,w,texNum);
	glPopMatrix();
}

void DrawArm(double length, double width, double height) {
	// close
	glBegin(GL_POLYGON);
	glNormal3d(0,-1,0);
	glVertex3d(length*.35,-width/2,height/2);
	glVertex3d(0,-width/2,height/2);
	glVertex3d(0,-width/2,0);
	glVertex3d(length*.4,-width/2,0);
	glVertex3d(length*.5,-width/2,height/2);
	glVertex3d(length,-width/2,height/2);
	glVertex3d(length,-width/2,height);
	glVertex3d(length*.45,-width/2,height);
	glEnd();
	// far
	glBegin(GL_POLYGON);
	glNormal3d(0,1,0);
	glVertex3d(length*.35,width/2,height/2);
	glVertex3d(0,width/2,height/2);
	glVertex3d(0,width/2,0);
	glVertex3d(length*.4,width/2,0);
	glVertex3d(length*.5,width/2,height/2);
	glVertex3d(length,width/2,height/2);
	glVertex3d(length,width/2,height);
	glVertex3d(length*.45,width/2,height);
	glEnd();
	// low bottom
	glBegin(GL_QUADS);
	glNormal3d(0,0,-1);
	glVertex2d(0,-width/2);
	glVertex2d(length*.4,-width/2);
	glVertex2d(length*.4,width/2);
	glVertex2d(0,width/2);
	glEnd();
	// high bottom
	glTranslated(0,0,height/2);
	glBegin(GL_QUADS);
	glNormal3d(0,0,-1);
	glVertex2d(length*.5,-width/2);
	glVertex2d(length,-width/2);
	glVertex2d(length,width/2);
	glVertex2d(length*.5,width/2);
	glEnd();
	glTranslated(0,0,-height/2);
	// low top
	glTranslated(0,0,height/2);
	glBegin(GL_QUADS);
	glNormal3d(0,0,1);
	glVertex2d(0,-width/2);
	glVertex2d(length*.35,-width/2);
	glVertex2d(length*.35,width/2);
	glVertex2d(0,width/2);
	glEnd();
	glTranslated(0,0,-height/2);
	// high top
	glTranslated(0,0,height);
	glBegin(GL_QUADS);
	glNormal3d(0,0,1);
	glVertex2d(length*.45,-width/2);
	glVertex2d(length,-width/2);
	glVertex2d(length,width/2);
	glVertex2d(length*.45,width/2);
	glEnd();
	glTranslated(0,0,-height);
	// low diagonal
	glBegin(GL_QUADS);
	glNormal3d(.766,0,-.643);
	glVertex3d(length*.4,-width/2,0);
	glVertex3d(length*.5,-width/2,height/2);
	glVertex3d(length*.5,width/2,height/2);
	glVertex3d(length*.4,width/2,0);
	glEnd();
	// high diagonal
	glBegin(GL_QUADS);
	glNormal3d(-.766,0,.643);
	glVertex3d(length*.35,-width/2,height/2);
	glVertex3d(length*.45,-width/2,height);
	glVertex3d(length*.45,width/2,height);
	glVertex3d(length*.35,width/2,height/2);
	glEnd();
	// short end
	glBegin(GL_QUADS);
	glNormal3d(1,0,0);
	glVertex3d(length,-width/2,height/2);
	glVertex3d(length,-width/2,height);
	glVertex3d(length,width/2,height);
	glVertex3d(length,width/2,height/2);
	glEnd();
	// long end
	glBegin(GL_QUADS);
	glNormal3d(-1,0,0);
	glVertex3d(0,-width/2,0);
	glVertex3d(0,-width/2,height/2);
	glVertex3d(0,width/2,height/2);
	glVertex3d(0,width/2,0);
	glEnd();
}

void DrawCam(double radius, double height) {
	glPushMatrix();
	glTranslated(0,0,height/2);
		DrawSolidCylinder(5,radius+6,35,0);// top disc
		DrawSolidCylinder(23,5,20,0);// top shaft
		glTranslated(0,0,-height);
			DrawSolidCylinder(-45,5,20,0);// bottom shaft
			DrawSolidCylinder(-5,radius+6,35,0);// bottom disc
			glTranslated(radius,0,0);
				DrawSolidCylinder(height,5,20,0);// cam shaft
	glPopMatrix();
}

void DrawRotor(double radius) {
	glPushMatrix();
		glRotated(-90,0,1,0);
			glTranslated(0,0,radius*.2);
			DrawTaperCylinder(radius/1.75,radius*1.04,radius*1.37,30,0);
			glTranslated(0,0,-radius*.2);
			for(int i=0; i<4; i++) {
				glRotated(90*i,0,0,1);
				DrawCurveCylinder(radius,120-radius,14,20,2,0);
			}
	glPopMatrix();
}

void DrawStages(double angle1, double angle2, double radius) {
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

			DrawCurveCylinder(radius,120-radius,s,20,t,0);
			glRotated(180,0,0,1);
			DrawCurveCylinder(radius,120-radius,s,20,t,0);
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

			DrawCurveCylinder(radius,120-radius,s,20,t,0);
			glRotated(-180,0,0,1);
			DrawCurveCylinder(radius,120-radius,s,20,t,0);
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

			DrawCurveCylinder(radius,120-radius,s,20,t,0);
			glRotated(180,0,0,1);
			DrawCurveCylinder(radius,120-radius,s,20,t,0);
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

			DrawCurveCylinder(radius,120-radius,s,20,t,0);
			glRotated(-180,0,0,1);
			DrawCurveCylinder(radius,120-radius,s,20,t,0);
		glDisable(GL_BLEND);
	glPopMatrix();
}

vector<point2> createPlug() {
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

void DrawPlug(int slices) {
	static vector<point2> outline = createPlug();
	RotatePolygon(outline, 2*PI, slices, 0, 1);
}

double DrawArms(double armLength, double camLength, double rtt, bool inside) {
	// Connecting rod length
	static double conLength = sqrt(camLength*camLength+sqrt(2*armLength*armLength)*sqrt(2*armLength*armLength))-3;
//	static double conLength = 139;
	double camAngle = rtt;
	if(inside)
		camAngle += 180;
	while(camAngle >= 360) // Keep camAngle less that 360
		camAngle -= 360;

	// Arm angle math
	double h, armAngle, conAngle;
	h = LOCside((camAngle+90)*PI/180,armLength,camLength);	// Hypotinus length
	armAngle  = LOCangle(camLength,armLength,h)*180/PI;		// Degree angle from arm to hypotinus
	if(camAngle >= 90 && camAngle <= 270)					// Check if cam is left of y axis
		armAngle = -armAngle;
	armAngle += LOCangle(conLength,armLength,h)*180/PI;		// Add degree angle from hypotinus to pivot arm
	armAngle  = 90 - armAngle;								// Degree armAngle for glut glRotated()
	conAngle  = LOCangle(h,armLength,conLength)*180/PI;		// Degree conAngle for glut glRotated()
	conAngle  = 180 + armAngle - conAngle;
	
	DrawArms(armLength,armAngle,camLength,camAngle,conLength,conAngle,inside);

	return armAngle;
}

void DrawArms(double armLength, double armAngle, double camLength, double camAngle, double conLength, double conAngle, bool inside) {
	// Pivot Arm
	glPushMatrix();
		glRotated(armAngle,0,0,1);
			glTranslated(-15,0,0);
			if(inside)
				glCallList(PIVOTARM);
				//DrawArm(armLength+30,30,15);
			else { // outside
				glTranslated(0,0,-27.5);
					glCallList(PIVOTARM);
					//DrawArm(armLength+30,30,15);
				glTranslated(0,0,55);
			}
			glRotated(180,1,0,0);
			glCallList(PIVOTARM);
			//DrawArm(armLength+30,30,15);
			//DrawTexArm(armLength+30,30,15,tex);
	glPopMatrix();
	// Main Arm
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightGray);
	glPushMatrix();
		glRotated(90,0,0,1);
			glTranslated(0,0,10);
				glCallList(MAINARM);
				//DrawArm(armLength+15,30,15);
				//DrawTexArm(armLength+15,30,15,(txtrs)MetalDiamond);
				glTranslated(0,0,-20);
					glRotated(180,1,0,0);
						glCallList(MAINARM);
						//DrawArm(armLength+15,30,15);
						//DrawTexArm(armLength+15,30,15,(txtrs)MetalDiamond);
	glPopMatrix();
	// Cam shaft
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, silver);
	glPushMatrix();
		glTranslated(0,armLength,0);
			glRotated(camAngle,0,0,1);
				glCallList(CAM);
				//DrawCam(camLength,20);
	glPopMatrix();
	// Pivot Arm pin
	glPushMatrix();
	glRotated(armAngle,0,0,1);
		glTranslated(armLength,0,-14);
			DrawSolidCylinder(28,5,10,1);
	glPopMatrix();
	// Connecting rod
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
	glPushMatrix();
		glTranslated(armLength*cos(armAngle*PI/180),armLength*sin(armAngle*PI/180),0);
			glRotated(conAngle-90,0,0,1);
				glTranslated(-10,conLength/2,0);
					glRotated(90,0,1,0);
						DrawSolidBox(8,conLength+20,20);
	glPopMatrix();
}

double DrawMYT(double speed, bool stages) {
	static int largeGear = 80;  // Sun gear radius
	static int smallGear = 20;  // Planet gear radius
	static int camLength = smallGear-3; // Camshaft radius
	static int cylRadius = 40;  // Inside radius of toroidal cylinder
	static int frames    = 0;   // Counts the number of frames
	static double rtt	 = 0;   // Total rotation
	static double TDC	 = 0;   // Top Dead Center angle
	static double rpm    = 0;   // Current Revolutions Per Minute
	static clock_t sTime = clock();
	static double blueAngle = 0; // Angle of blue rotor 
	static double tanAngle  = 0; // Angle of tan rotor

	rtt += speed;
	if(rtt > 360.0)
		rtt -= 360.0;
	if(frames == 10) {
		frames = 0;
		rpm   = (clock()-sTime);
		rpm   = 600000*speed/rpm/360;
		sTime = clock();
	} else
		frames++;

	// Gears
	glPushMatrix();
	glRotated(90,0,1,0);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, silver);
		glPushMatrix();
				glCallList(LARGEGEAR);
				//DrawGear(largeGear,20,144);
		glPopMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
		glPushMatrix();
			glRotated(-rtt,0,0,1);
				glTranslated(100,0,0);
					glRotated(-4*rtt+5,0,0,1);
						glCallList(SMALLGEAR);
						//DrawGear(smallGear,20,36);
		glPopMatrix();
		glPushMatrix();
			glRotated(-rtt,0,0,1);
				glTranslated(-100,0,0);
					glRotated(-4*rtt+5,0,0,1);
						glCallList(SMALLGEAR);
						//DrawGear(smallGear,20,36);
		glPopMatrix();
	glPopMatrix();
	// Arms, Cams, Connecting rods
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
	glPushMatrix();
	glTranslated(-40,0,0);
		glRotated(-90,0,1,0);
			glRotated(rtt+90,0,0,1);
				blueAngle = DrawArms(largeGear+smallGear,camLength,rtt*4,1);
				blueAngle = -rtt-blueAngle+15-8;
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ivery);
	glPushMatrix();
	glTranslated(-40,0,0);
		glRotated(-90,0,1,0);
			glRotated(rtt+270,0,0,1);
				tanAngle = DrawArms(largeGear+smallGear,camLength,rtt*4,0);
				tanAngle = -rtt-tanAngle-15-9;
	glPopMatrix();
	// torus cylinder
	glPushMatrix();
		glTranslated(cylRadius+20,0,0);
			glRotated(90,0,1,0);
				glutWireTorus(cylRadius,120-cylRadius,15,40);
	glPopMatrix();
	// rotors
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ivery);
	glPushMatrix();
		glRotated(tanAngle,1,0,0);
			glTranslated(60,0,0);
				glCallList(ROTOR);
				//DrawRotor(cylRadius);
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
	glPushMatrix();
		glRotated(blueAngle,1,0,0);
			glTranslated(60,0,0);
				glRotated(180,0,0,1);
					glCallList(ROTOR);
					//DrawRotor(cylRadius);
	glPopMatrix();
	// Cumbustion cycles
	if (stages) {
		glPushMatrix();
			glRotated(tanAngle,1,0,0);
				glTranslated(60,0,0);
					glRotated(180,0,0,1);
						DrawStages(blueAngle, tanAngle, cylRadius);
		glPopMatrix();
	}
	glPopMatrix();
	// intake
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightBlue);
	glPushMatrix();
		glRotated(-15,1,0,0);
			glTranslated(cylRadius+20,0,118);
				DrawSolidCylinder(10,15,25,15,1);
	glPopMatrix();
	glPushMatrix();
		glRotated(-195,1,0,0);
			glTranslated(cylRadius+20,0,118);
				DrawSolidCylinder(10,15,25,15,1);
	glPopMatrix();
	// exhaust
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
	glPushMatrix();
		glRotated(15,1,0,0);
			glTranslated(cylRadius+20,0,118);
				DrawSolidCylinder(10,15,25,15,1);
	glPopMatrix();
	glPushMatrix();
		glRotated(195,1,0,0);
			glTranslated(cylRadius+20,0,118);
				DrawSolidCylinder(10,15,25,15,1);
	glPopMatrix();
	// spark plugs
	glPushMatrix();
		glRotated(90-TDC,1,0,0);
			glTranslated(cylRadius+20,0,120);
				glCallList(PLUG);
	glPopMatrix();
	glPushMatrix();
		glRotated(-90-TDC,1,0,0);
			glTranslated(cylRadius+20,0,120);
				glCallList(PLUG);
	glPopMatrix();
	// Output shaft
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightGray);
	glPushMatrix();
		glRotated(90,0,1,0);
			glTranslated(0,0,-80);
				DrawSolidCylinder(200,5,20,1);
	glPopMatrix();
	glPushMatrix();
		glTranslated(120,0,0);
			glutSolidSphere(5,20,20);
			glTranslated(-200,0,0);
				glutSolidSphere(5,20,20);
	glPopMatrix();
	glPushMatrix();
		glRotated(-rtt,1,0,0);
			glTranslated(-80,0,0);
				DrawSolidCylinder(30,5,20,0);
				glTranslated(200,0,0);
					DrawSolidCylinder(30,5,20,0);
	glPopMatrix();
	// Blue shaft
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightBlue);
	glPushMatrix();
		glRotated(90,0,1,0);
			glTranslated(0,0,-40);
				DrawSolidCylinder(120,10,20,1);
	glPopMatrix();
	// Tan shaft
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ivery);
	glPushMatrix();
		glRotated(90,0,1,0);
			glTranslated(0,0,-20);
				DrawSolidCylinder(60,15,20,1);
	glPopMatrix();
	return rpm;
}

void InitDisplayLists() {
	glNewList(MAINARM,GL_COMPILE);
	DrawArm(115,30,15);
	glEndList();

	glNewList(PIVOTARM,GL_COMPILE);
	DrawArm(130,30,15);
	glEndList();

	glNewList(CAM,GL_COMPILE);
	DrawCam(17,20);
	glEndList();

	glNewList(LARGEGEAR,GL_COMPILE);
	DrawGear(80,10,20,144);
	glEndList();

	glNewList(SMALLGEAR,GL_COMPILE);
	DrawGear(20,5,20,36);
	glEndList();

	glNewList(PLUG,GL_COMPILE);
	DrawPlug(36);
	glEndList();

	glNewList(ROTOR,GL_COMPILE);
	DrawRotor(40);
	glEndList();
}

/*double LOCangle(double sideA, double sideB, double sideC) {
	return acos((sideB*sideB+sideC*sideC-sideA*sideA)/(2*sideB*sideC));
}

double LOCside(double angleA, double sideB, double sideC) {
	// angleA in radians
	return sqrt(sideB*sideB + sideC*sideC - 2*sideB*sideC*cos(angleA));
}*/
