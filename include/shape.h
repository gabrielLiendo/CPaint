#pragma once
#include <iostream>
#include <math.h>

using namespace std;

struct Point2D
{
	int x; 
	int y;
};

struct Color
{
	float r, g, b;
};

class CtrlPoint
{
protected:
	int x0, x1, y0, y1;

public:
	int x, y;

	CtrlPoint() {}

	CtrlPoint(int x, int y)
	{
		setPosition(x, y);
	}

	void setPosition(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	int getX()
	{
		return x;
	}

	int getY()
	{
		return y;
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
	bool selected = false;

public:
	CShape(float r1, float g1, float b1, float r2, float g2, float b2)
	{	
		fillColor.r = r1; fillColor.g = g1; fillColor.b = b1;
		borderColor.r = r2; borderColor.g = g2; borderColor.b = b2;
	}

	virtual ~CShape()
	{
		cout << "Se destruyo un shape" << endl;
	}

	// Display one pixel in the position (x, y) with the color c
	void putPixel(int x, int y, Color c)
	{	
		glBegin(GL_POINTS);
			glColor3f(c.r, c.g, c.b);
			glVertex2i(x, y);
		glEnd();
	}

	void setBorderColor(float r, float g, float b)
	{
		borderColor.r = r; borderColor.g = g; borderColor.b = b;
	}

	void setFillColor(float r, float g, float b)
	{
		fillColor.r = r; fillColor.g = g; fillColor.b = b;
	}

	void hLine(int xmin, int xmax, int y, Color c)
	{
		for (int x = xmin; x <= xmax; x++)
			putPixel(x, y, c);
	}

	void setSelected(bool selected)
	{
		this->selected = selected;
	}

	virtual void release() 
	{
		this->selected = false;
	}

	virtual void update(int x, int y) = 0;

	virtual void setAnchorPoint(int x, int y) = 0;

	virtual void render(const char* mode) = 0;

	virtual bool onClick(int x, int y) = 0;
	
	virtual void onMove(int x1, int y1) = 0;
};