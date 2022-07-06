#pragma once
#include <iostream>
#include <math.h>

using namespace std;

struct Color
{
	float r, g, b;
};

struct CtrlPoint
{
	int x, y;

	CtrlPoint() {}

	CtrlPoint(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	void renderCtrlPoint()
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
			glVertex2i(x, y);
		glEnd();
		glPointSize(1.0f);
	}

	int distance(int x, int y)
	{
		return (int)sqrt(pow(x - this->x, 2) + pow(y - this->y, 2) * 1.0);
	}
};

class CShape
{
protected:
	Color fillColor, borderColor;
	CtrlPoint anchorPoint;
	CtrlPoint *pointSelected = nullptr;
	int level = 0;

public:
	CShape(int x0, int y0, float r1, float g1, float b1, float r2, float g2, float b2)
	{	
		anchorPoint.x = x0; anchorPoint.y = y0;
		fillColor.r = r1; fillColor.g = g1; fillColor.b = b1;
		borderColor.r = r2; borderColor.g = g2; borderColor.b = b2;
	}

	virtual ~CShape(){ cout << "Se destruyo un shape" << endl; }

	void setAnchorPoint(int x, int y)
	{
		anchorPoint.x = x;
		anchorPoint.y = y;
	}

	void setBorderColor(float r, float g, float b)
	{
		borderColor.r = r; borderColor.g = g; borderColor.b = b;
	}

	void setFillColor(float r, float g, float b)
	{
		fillColor.r = r; fillColor.g = g; fillColor.b = b;
	}

	// Display one pixel in the position (x, y) with the color c
	void putPixel(int x, int y, Color c)
	{	
		glBegin(GL_POINTS);
			glColor3f(c.r, c.g, c.b);
			glVertex2i(x, y);
		glEnd();
	}

	// Display horizontal line from xmin to xmax with the color c
	void hLine(int xmin, int xmax, int y, Color c)
	{
		for (int x = xmin; x <= xmax; x++)
			putPixel(x, y, c);
	}

	// Mark control point as deselected
	void release() 
	{
		pointSelected = nullptr;
	}

	virtual void update(int x, int y) = 0;

	virtual void renderCtrlPoints() = 0;

	virtual void render(const bool modeHardware) = 0;

	virtual bool onClick(int x, int y) = 0;

	virtual void clickedCtrlPoint(int x, int y) = 0;
	
	virtual void onMove(int x1, int y1) = 0;

	virtual bool finished()
	{
		return true;
	}
};