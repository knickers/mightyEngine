// Slider class
// By Bart Stander
// October, 2008
// For CS 1410
// This class is for maintaining a slider bar in OpenGL.
// It remebers its own position, color, and value.
// The value is always between 0 and 1.
// Calling HandleMouseClick will update the slider's value, if the x,y are inside the slider's boundaries.
// In that case it returns true.
//
// Nick Cox
// February, 2011
// Reconstructed and converted to horizontal and vertical slider subclasses.
// HandleMouseClick converted into two different methods, Click and Drag, so that dragging a slider
//     won't grab an adjacent slider by accident.
// Can be drawn in different styles (types).

#pragma once

enum SliderType
{
	SLIDER_BASIC,
	SLIDER_SHINY,
	SLIDER_BUBBLE,
	SLIDER_NARROW,
	SLIDER_BLOCK,
	SLIDER_SIZE
};
enum SliderDir
{
	DIR_UP,
	DIR_DOWN,
	DIR_RIGHT,
	DIR_LEFT
};

class slider
{
private:
protected:
	SliderType mType;
	SliderDir mDir;
	bool mGrabbed;
	double mLeft, mTop, mRight, mBottom;
	double mWidth, mHeight, mValue;
	float  mColor[4];
	virtual void DrawBasic() =0;
	virtual void DrawShiny() =0;
	virtual void DrawBubble()=0;
	virtual void DrawNarrow()=0;
	virtual void DrawBlock() =0;
public:
	slider(double left, double bottom, double width, double height, double red, double green, double blue, double value);
	slider(double left, double bottom, double width, double height);
	bool Click(int x, int y);
	virtual bool Drag(int x, int y)=0;

	void Draw();
	double GetValue() {return mValue;}
	void SetColor(float red, float green, float blue){mColor[0]=red; mColor[1]=green; mColor[2]=blue; mColor[3]=0;}
	void SetSize(double width, double height){mRight=mLeft+width; mTop=mBottom+height; mWidth=width; mHeight=height;}
	void SetType(SliderType type)	{mType = type;}
	void SetPos(double x, double y)	{mLeft=x-mWidth/2; mRight=x+mWidth/2; mTop=y+mHeight/2; mBottom=y-mHeight/2;}
	void SetDir(SliderDir dir)		{mDir = dir;}
	void SetValue(double value)		{mValue = value;}
	bool IsGrabbed()				{return mGrabbed;}
	bool Increment(double d);
	void Resize(double xAmount, double yAmount){mWidth+=xAmount; mHeight+=yAmount; mRight+=xAmount; mTop+=yAmount;}
};

class horizontalSlider : public slider
{
private:
	void DrawBasic();
	void DrawShiny();
	void DrawBubble();
	void DrawNarrow();
	void DrawBlock();
public:
	horizontalSlider(int left, int bottom, int width, int height, double red, double green, double blue, double value);
	horizontalSlider(double left, double bottom, double width, double height);
	bool Drag(int x, int y);
};

class verticalSlider : public slider
{
private:
	void DrawBasic();
	void DrawShiny();
	void DrawBubble();
	void DrawNarrow();
	void DrawBlock();
public:
	verticalSlider(int left, int bottom, int width, int height, double red, double green, double blue, double value);
	verticalSlider(double left, double bottom, double width, double height);
	bool Drag(int x, int y);
};
