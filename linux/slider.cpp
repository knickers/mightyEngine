#include <iostream>
using namespace std;
#include "slider.h"
#include "shapes.h"
#include "graphics.h"

slider::slider(double left, double bottom, double width, double height, 
               double red, double green, double blue, double value)
{
	mLeft=left;        mTop=bottom+height;
	mRight=left+width; mBottom=bottom;
	mWidth=width;      mHeight=height;
	mColor[0]=red;     mColor[1]=green;    mColor[2]=blue;
	mValue=value;      mType=SLIDER_BASIC; mDir=DIR_RIGHT;
}

slider::slider(double left, double bottom, double width, double height)
{
	mLeft=left;        mTop=bottom+height;
	mRight=left+width; mBottom=bottom;
	mWidth=width;      mHeight=height;
	mColor[0]=.5;      mColor[1]=.5;       mColor[2]=.5;
	mValue=0;          mType=SLIDER_BASIC; mDir=DIR_RIGHT;
}

void slider::Draw()
{
	     if(mType == SLIDER_BASIC)  DrawBasic();
	else if(mType == SLIDER_SHINY)  DrawShiny();
	else if(mType == SLIDER_BUBBLE) DrawBubble();
	else if(mType == SLIDER_NARROW) DrawNarrow();
	else if(mType == SLIDER_BLOCK)  DrawBlock();
}

bool slider::Click(int x, int y)
{
	if(x>=mLeft && x<=mRight && y>=mBottom && y<=mTop)
	{
		mGrabbed = true;
		Drag(x, y);
	}
	else
		mGrabbed = false;
	return mGrabbed;
}

bool slider::Increment(double d)
{
	if(mValue+d > 1)
	{
		mValue = 1;
		return false;
	}
	else if(mValue+d < 0)
	{
		mValue = 0;
		return false;
	}
	else
	{
		mValue += d;
		return true;
	}
}
horizontalSlider::horizontalSlider(int left, int bottom, int width, int height,
	                               double red, double green, double blue, double value)
: slider(left,bottom,width,height,red,green,blue,value)
{
}
horizontalSlider::horizontalSlider(double left, double bottom, double width, double height)
: slider(left,bottom,width,height)
{
}
void horizontalSlider::DrawBasic()
{
	double middle = mBottom + mHeight*.6;
	double left = mLeft;
	double right = mLeft + mWidth*mValue;
	if(mDir==DIR_LEFT)
	{
		left = mRight - mWidth*mValue;
		right = mRight;
	}
	glColor3f(mColor[0], mColor[1], mColor[2]);
	// The lower solid color
	DrawRectangle(left,mBottom,right,mTop);
	// Draw the outline
	glColor3f(.7, .7, .7);
	DrawLine(mLeft,mBottom,mRight,mBottom);
	DrawLine(mLeft,mTop,mRight,mTop);
	DrawLine(mLeft,mBottom,mLeft,mTop);
	DrawLine(mRight,mBottom,mRight,mTop);
}
void horizontalSlider::DrawShiny()
{
	double middle = mBottom + mHeight*.6;
	double left = mLeft;
	double right = mLeft + mWidth*mValue;
	if(mDir==DIR_LEFT)
	{
		left = mRight - mWidth*mValue;
		right = mRight;
	}
	glColor3f(mColor[0], mColor[1], mColor[2]);
	// The lower solid color
	DrawRectangle(left,mBottom,right,mTop);
	// The upper faded color
	FadeRectangle(left,mTop,right,middle,1);
	// Draw the outline
	glColor3f(.7, .7, .7);
	DrawLine(mLeft,mBottom,mRight,mBottom);
	DrawLine(mLeft,mTop,mRight,mTop);
	DrawLine(mLeft,mBottom,mLeft,mTop);
	DrawLine(mRight,mBottom,mRight,mTop);
}
void horizontalSlider::DrawBubble()
{
	float in[3]  = {1,1,1};
	double middle = mBottom + mHeight/2;
	double top    = mTop-mHeight/3;
	double bottom = mBottom+mHeight/3;
	double left   = mLeft - mHeight/2;
	double right  = mLeft + mWidth*mValue;
	double xPos   = right;
	if(mDir==DIR_LEFT)
	{
		left  = mRight - mWidth*mValue;
		right = mRight + mHeight/2;
		xPos  = left;
	}
	// Draw Slider color
	glColor3f(mColor[0], mColor[1], mColor[2]);
	DrawRectangle(left,bottom,right,top);
	// Draw upper and lower bars
	glColor3f(.5,.5,.5);
	DrawLine(mLeft-mHeight/2,top,mRight+mHeight/2,top);
	DrawLine(mLeft-mHeight/2,bottom,mRight+mHeight/2,bottom);
	// Draw end bubbles
	DrawCircle(mLeft-mHeight, middle, mHeight/2, in, mColor);
	DrawCircle(mRight+mHeight, middle, mHeight/2, in, mColor);
	// Draw position bubble
	DrawCircle(xPos, middle, mHeight/2, in, mColor);
}
void horizontalSlider::DrawNarrow()
{
	float in[3]  = {1,1,1};
	double middle = mBottom + mHeight/2;
	double top    = mTop-mHeight/3;
	double bottom = mBottom+mHeight/3;
	double left   = mLeft;
	double right  = mLeft + mWidth*mValue;
	double xPos   = right;
	if(mDir==DIR_LEFT)
	{
		left  = mRight - mWidth*mValue;
		right = mRight;
		xPos  = left;
	}
	// Draw Slider color
	glColor3f(mColor[0], mColor[1], mColor[2]);
	DrawRectangle(left,bottom,right,top);
	glColor3f(.5,.5,.5);
	DrawLine(mLeft,top,mRight,top);      // Upper bar
	DrawLine(mLeft,bottom,mRight,bottom);// Lower bar
	DrawLine(mRight,bottom,mRight,top);  // Right bar
	DrawLine(mLeft,bottom,mLeft,top);    // Left bar
	// Draw position bubble
	DrawCircle(xPos, middle, mHeight/2, in, mColor);
}

void horizontalSlider::DrawBlock()
{
	double top    = mTop-mHeight/3;
	double bottom = mBottom+mHeight/3;
	double xPos   = mLeft + mWidth*mValue;
	if(mDir==DIR_LEFT)
		xPos  = mRight - mWidth*mValue;
	glColor3f(mColor[0],mColor[1],mColor[2]);
	// Trolly
	DrawRectangle(xPos-mWidth*.01,mBottom+mHeight*.3,xPos+mWidth*.01,mTop-mHeight*.3);
	// Upper rail
	glColor3f(.7,.7,.7);
	DrawRectangle(mLeft-mWidth*.035,mTop-mHeight*.3,mRight+mWidth*.035,mTop-mHeight*.1);
	FadeRectangle(mLeft-mWidth*.035,mTop-mHeight*.1,mRight+mWidth*.035,mTop-mHeight*.2,1);
	// Lower rail
	glColor3f(.7,.7,.7);
	DrawRectangle(mLeft-mWidth*.035,mBottom+mHeight*.1,mRight+mWidth*.035,mBottom+mHeight*.3);
	FadeRectangle(mLeft-mWidth*.035,mBottom+mHeight*.3,mRight+mWidth*.035,mBottom+mHeight*.2,1);
	glColor3f(.4,.4,.3);
	// Left block
	glColor3f(mColor[0],mColor[1],mColor[2]);
	DrawRectangle(mLeft-mWidth*.025,mBottom,mLeft-mWidth*.015,mTop);
	// Right block
	DrawRectangle(mRight+mWidth*.015,mBottom,mRight+mWidth*.025,mTop);
	// Trolly bearings
	glColor3f(.3,.3,.2);
	DrawRectangle(xPos-mWidth*.015,mTop-mHeight*.4,xPos-mWidth*.005,mTop);
	DrawRectangle(xPos+mWidth*.005,mTop-mHeight*.4,xPos+mWidth*.015,mTop);
	DrawRectangle(xPos-mWidth*.015,mBottom,xPos-mWidth*.005,mBottom+mHeight*.4);
	DrawRectangle(xPos+mWidth*.005,mBottom,xPos+mWidth*.015,mBottom+mHeight*.4);
}

bool horizontalSlider::Drag(int x, int y)
{
	if(mGrabbed && x>=mLeft && x<=mRight)
	{
		if(mDir == DIR_RIGHT)
		{
			if(x > mRight)
				mValue = 1.0;
			else if(x < mLeft)
				mValue = 0.0;
			else
				mValue = ((double)x-mLeft)/mWidth;
		}
		else //if(mDir == DIR_LEFT)
			if(x > mRight)
				mValue = 0.0;
			else if(x < mLeft)
				mValue = 1.0;
			else
				mValue = (mRight-(double)x)/mWidth;
		return true;
	}
	else
		return false;
}

//**************************************************************//
//**************************************************************//
//**************************************************************//

verticalSlider::verticalSlider(int left, int bottom, int width, int height,
	                               double red, double green, double blue, double value)
: slider(left,bottom,width,height,red,green,blue,value)
{
	mDir = DIR_UP;
}

verticalSlider::verticalSlider(double left, double bottom, double width, double height)
: slider(left,bottom,width,height)
{
	mDir = DIR_UP;
}

void verticalSlider::DrawBasic()
{
	double middle = mLeft + mWidth*.6;
	double top    = mBottom + mHeight*mValue;
	double bottom = mBottom;
	if(mDir==DIR_DOWN)
	{
		top    = mTop;
		bottom = mTop - mHeight*mValue;
	}
	glColor3f(mColor[0], mColor[1], mColor[2]);
	// The lower solid color
	DrawRectangle(mLeft,bottom,mRight,top);
	// Draw the outline
	glColor3f(.7, .7, .7);
	DrawLine(mLeft,mBottom,mRight,mBottom);
	DrawLine(mLeft,mTop,mRight,mTop);
	DrawLine(mLeft,mBottom,mLeft,mTop);
	DrawLine(mRight,mBottom,mRight,mTop);
}
void verticalSlider::DrawShiny()
{
	double middle = mLeft + mWidth/2;
	double top    = mBottom + mHeight*mValue;
	double bottom = mBottom;
	if(mDir==DIR_DOWN)
	{
		top    = mTop;
		bottom = mTop - mHeight*mValue;
	}
	// The left color
	glColor3f(mColor[0], mColor[1], mColor[2]);
	FadeRectangle(middle,bottom,mLeft,top,0);
	// The right faded color
	glColor3f(mColor[0], mColor[1], mColor[2]);
	FadeRectangle(middle,bottom,mRight,top,0);
	// Draw the outline
	glColor3f(.7, .7, .7);
	DrawLine(mLeft,mBottom,mRight,mBottom);
	DrawLine(mLeft,mTop,mRight,mTop);
	DrawLine(mLeft,mBottom,mLeft,mTop);
	DrawLine(mRight,mBottom,mRight,mTop);
}
void verticalSlider::DrawBubble()
{
	float in[3]  = {1,1,1};
	double middle = mLeft + mWidth/2;
	double top    = mBottom + mHeight*mValue;
	double bottom = mBottom - mWidth;
	double left   = mLeft + mWidth/3;
	double right  = mRight - mWidth/3;
	double yPos   = top;
	if(mDir==DIR_DOWN)
	{
		top    = mTop + mWidth;
		bottom = mTop - mHeight*mValue;
		yPos   = bottom;
	}
	// Draw Slider color
	glColor3f(mColor[0], mColor[1], mColor[2]);
	DrawRectangle(left,bottom,right,top);
	// Draw left and right bars
	glColor3f(.5,.5,.5);
	DrawLine(left,mBottom-mWidth/2,left,mTop+mWidth/2);
	DrawLine(right,mBottom-mWidth/2,right,mTop+mWidth/2);
	// Draw end bubbles
	DrawCircle(middle, mBottom-mWidth, mWidth/2, in, mColor);
	DrawCircle(middle, mTop+mWidth, mWidth/2, in, mColor);
	// Draw position bubble
	DrawCircle(middle, yPos, mWidth/2, in, mColor);
}
void verticalSlider::DrawNarrow()
{
	float in[3]  = {1,1,1};
	double middle = mLeft + mWidth/2;
	double top    = mBottom + mHeight*mValue;
	double bottom = mBottom;
	double left   = mLeft + mWidth/3;
	double right  = mRight - mWidth/3;
	double yPos   = top;
	if(mDir==DIR_DOWN)
	{
		top    = mTop;
		bottom = mTop - mHeight*mValue;
		yPos   = bottom;
	}
	// Draw Slider color
	glColor3f(mColor[0], mColor[1], mColor[2]);
	DrawRectangle(left,bottom,right,top);
	// Draw left and right bars
	glColor3f(.5,.5,.5);
	DrawLine(left,mBottom,left,mTop);    // Left bar
	DrawLine(right,mBottom,right,mTop);  // Right bar
	DrawLine(left,mTop,right,mTop);      // Upper bar
	DrawLine(left,mBottom,right,mBottom);// Lower bar
	// Draw position bubble
	DrawCircle(middle, yPos, mWidth/2, in, mColor);
}

void verticalSlider::DrawBlock()
{
	double left   = mLeft + mWidth/3;
	double right  = mRight - mWidth/3;
	double yPos   = mBottom + mHeight*mValue;
	if(mDir==DIR_DOWN)
		yPos   = mTop - mHeight*mValue;
	glColor3fv(mColor);
	// Trolly
	DrawRectangle(mLeft+mWidth*.3,yPos-mHeight*.01,mRight-mWidth*.3,yPos+mHeight*.01);
	// Left rail
	glColor3f(.7,.7,.7);
	FadeRectangle(mLeft+mWidth*.2,mBottom-mHeight*.035,mLeft+mWidth*.1,mTop+mHeight*.035,0);
	glColor3f(.7,.7,.7);
	FadeRectangle(mLeft+mWidth*.2,mBottom-mHeight*.035,mLeft+mWidth*.3,mTop+mHeight*.035,0);
	// Right rail
	glColor3f(.7,.7,.7);
	FadeRectangle(mRight-mWidth*.2,mBottom-mHeight*.035,mRight-mWidth*.3,mTop+mHeight*.035,0);
	glColor3f(.7,.7,.7);
	FadeRectangle(mRight-mWidth*.2,mBottom-mHeight*.035,mRight-mWidth*.1,mTop+mHeight*.035,0);
	// Lower block
	glColor3f(mColor[0],mColor[1],mColor[2]);
	DrawRectangle(mLeft,mBottom-mHeight*.025,mRight,mBottom-mHeight*.015);
	// Upper block
	DrawRectangle(mLeft,mTop+mHeight*.015,mRight,mTop+mHeight*.025);
	// Trolly bearings
	glColor3f(.6,.6,.6);
	DrawRectangle(mLeft,yPos-mHeight*.015,mLeft+mWidth*.4,yPos-mHeight*.005);
	DrawRectangle(mLeft,yPos+mHeight*.005,mLeft+mWidth*.4,yPos+mHeight*.015);
	DrawRectangle(mRight-mWidth*.4,yPos-mHeight*.015,mRight,yPos-mHeight*.005);
	DrawRectangle(mRight-mWidth*.4,yPos+mHeight*.005,mRight,yPos+mHeight*.015);
}

bool verticalSlider::Drag(int x, int y)
{
	if(mGrabbed)
	{
		if(mDir == DIR_UP)
		{
			if(y > mTop)
			{
				mValue = 1.0;
			}
			else if(y < mBottom)
			{
				mValue = 0.0;
			}
			else
			{
				mValue = ((double)y-mBottom)/mHeight;
			}
		}
		else //if(mDir == DIR_DOWN)
		{
			if(y > mTop)
			{
				mValue = 0.0;
			}
			else if(y < mBottom)
			{
				mValue = 1.0;
			}
			else
			{
				mValue = (mTop-(double)y)/mHeight;
			}
		}
		return true;
	}
	else
		return false;
}