#pragma once
#include <iostream>
#include <math.h>

using namespace std;

struct Point2D
{
	int x; 
	int y;
};

class CtrlPoint
{
protected:
	//float borderColor[3];
	int x0, x1, y0, y1;
	float fillColor[3];
	Point2D refPoint;

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
	float fillColor[3];
	bool selected = false;
	//float borderColor[3];

public:
	CShape(float r, float g, float b)
	{	
		fillColor[0] = r; fillColor[1] = g; fillColor[2] = b;
	}

	virtual ~CShape()
	{
		cout << "Se destruyo un shape" << endl;
	}

	void putPixel(int x, int y)
	{	
		glBegin(GL_POINTS);
			glVertex2i(x, y);
		glEnd();
	}

	void setColor(float r, float g, float b)
	{
		fillColor[0] = r; fillColor[1] = g; fillColor[2] = b;
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

	virtual void setRefPoint(int x, int y) = 0;

	virtual void render(const char* mode) = 0;

	virtual bool onClick(int x, int y) = 0;
	
	virtual void onMove(int x1, int y1) = 0;
};

class BoxableShape
{
protected:
	CtrlPoint boxPoints[4];
	Point2D refPoint;

public:
	BoxableShape(int x0, int y0, int x1, int y1)
	{
		setRefPoint(x0, y0);

		boxPoints[0] = CtrlPoint(x0, y1);
		boxPoints[1] = CtrlPoint(x0, y0);
		boxPoints[2] = CtrlPoint(x1, y0);
		boxPoints[3] = CtrlPoint(x1, y1);
	}

	void setRefPoint(int x, int y)
	{
		refPoint.x = x;
		refPoint.y = y;
	}

	// Set bounding box corner points
	void setBoundingBox(int x1, int y1)
	{
		int x0 = refPoint.x, y0 = refPoint.y;

		if (x1 < x0)
			swap(x0, x1);
		if (y1 < y0)
			swap(y1, y0);

		boxPoints[0].x = x0; boxPoints[0].y = y1;
		boxPoints[1].x = x0; boxPoints[1].y = y0;
		boxPoints[2].x = x1; boxPoints[2].y = y0;
		boxPoints[3].x = x1; boxPoints[3].y = y1;
	}

	void moveBoundingBox(int x1, int y1)
	{
		int dx = x1 - refPoint.x;
		int dy = y1 - refPoint.y;

		for (int i = 0; i < 4; i++)
		{
			boxPoints[i].x += dx;
			boxPoints[i].y += dy;
		}

		setRefPoint(x1, y1);
	}

	void renderBox()
	{
		// Set color and style of the line
		glColor3f(1.0f, 1.0f, 1.0f);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
			for (auto p : boxPoints)
				glVertex2i(p.x, p.y);
		glEnd();
		glDisable(GL_LINE_STIPPLE);

		// Render vertex points
		for (auto p : boxPoints)
			p.renderCtrlPoint();
	}
};

