#pragma once
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

struct Color
{
	float r, g, b;
};

struct Point
{
	int x, y;

	Point() {}

	Point(int x, int y)
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
};

class CShape
{
protected:
	Color fillColor, borderColor;
	Point boxPoints[4];
	Point anchorPoint;
	Point *pointSelected = nullptr;
	string info;

	int level = 0;
	bool filled = true;

public:

	CShape(int x0, int y0, float r1, float g1, float b1, float r2, float g2, float b2, bool filled, string info)
	{	
		anchorPoint.x = x0; anchorPoint.y = y0;
		fillColor.r = r1; fillColor.g = g1; fillColor.b = b1;
		borderColor.r = r2; borderColor.g = g2; borderColor.b = b2;
		this->filled = filled;
		this->info = info;
	}

	CShape(float r, float g, float b, string info)
	{
		borderColor.r = r; borderColor.g = g; borderColor.b = b;
		this->info = info;
	}

	virtual ~CShape(){ cout << "Se destruyo un shape" << endl; }

	int getLayerLevel()
	{
		return level;
	}

	void setLayerLevel(int level)
	{
		this->level = level;
	}

	void setFilled(bool filled)
	{
		this->filled = filled;
	}

	bool getFillBool()
	{
		return filled;
	}

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
	void horizontalLine(int xmin, int xmax, int y, Color c)
	{
		for (int x = xmin; x <= xmax; x++)
			putPixel(x, y, c);
	}

	// Mark control point as deselected
	void release()
	{
		pointSelected = nullptr;
	}

	// Draw a line using Bresenham's algorithm
	void drawLine(int x0, int y0, int x1, int y1, Color c)
	{
		int x, y, dx, dy, d, incN, incE, incNE, varX, varY;
		x = x0;
		y = y0;
		dx = abs(x1 - x);
		dy = abs(y1 - y);
		d = dx - (dy << 1);
		incN = (dx << 1);
		incE = -(dy << 1);
		incNE = (dx - dy) << 1;

		// Draw the initial pixel
		putPixel(x0, y0, c);

		// If |m| < 1 (abs(dx) > abs(dy)) we iterate over the x-axis, otherwise we iterate over the y-axis
		if (dy < dx)
		{
			if (x >= x1)
			{
				x = x1; x1 = x0;
				y = y1; y1 = y0;
			}

			varY = y1 > y ? 1 : -1;
			for (; x < x1; x++)
			{
				if (d < 0)
				{
					d += incNE;
					y += varY;
				}
				else
					d += incE;
				putPixel(x, y, c);
			}
		}
		else
		{
			if (y >= y1)
			{
				x = x1; x1 = x0;
				y = y1; y1 = y0;
			}

			varX = x1 < x ? -1 : 1;
			for (; y < y1; y++)
			{
				if (d < 0)
					d += incN;
				else {
					d += incNE;
					x += varX;
				}
				putPixel(x, y, c);
			}
		}
	}

	// Set bounding box corner points
	void setBoundingBox(int x0, int y0, int x1, int y1)
	{
		if (x1 < x0)
			swap(x0, x1);
		if (y1 < y0)
			swap(y1, y0);

		boxPoints[0].x = x0; boxPoints[0].y = y1;
		boxPoints[1].x = x0; boxPoints[1].y = y0;
		boxPoints[2].x = x1; boxPoints[2].y = y0;
		boxPoints[3].x = x1; boxPoints[3].y = y1;
	}

	// Move the 4 points of the box by 'dx' and 'dy' respectably
	void moveBoundingBox(int dx, int dy)
	{
		for (int i = 0; i < 4; i++)
		{
			boxPoints[i].x += dx;
			boxPoints[i].y += dy;
		}
	}

	/*void resizeBoundingBox(int x, int y)
	{
		int i = iPointSelected;
		int op = (i + 2) % 4;

		int dx = boxPoints[i].x - boxPoints[op].x;
		int dy = boxPoints[i].y - boxPoints[op].y;

		if (dx * dx + dy * dy <= 4)
			return;

		boxPoints[i].x = x;
		boxPoints[i].y = y;

		boxPoints[i - ((i & 1) << 1) + 1].x = x;
		boxPoints[-(i - 3) % 4].y = y;
	}*/

	void renderBox()
	{
		// Set color and style of the line
		glColor3f(1.0f, 1.0f, 1.0f);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < 4; i++)
			glVertex2i(boxPoints[i].x, boxPoints[i].y);
		glEnd();
		glDisable(GL_LINE_STIPPLE);

		// Render vertex points
		for (int i = 0; i < 4; i++)
			boxPoints[i].renderCtrlPoint();
	}

	virtual void update(int x, int y) = 0;

	virtual void renderCtrlPoints() = 0;

	virtual void render(const bool modeHardware) = 0;

	virtual bool onClick(int x, int y) = 0;

	virtual void clickedCtrlPoint(int x, int y) = 0;
	
	virtual void onMove(int x1, int y1) = 0;

	virtual std::string getInfo()
	{
		// Add position info
		info += to_string(boxPoints[0].x) + " " + to_string(boxPoints[0].y) + " "
			+ to_string(boxPoints[2].x) + " " + to_string(boxPoints[2].y) + " ";

		// Add border info
		info += to_string(borderColor.r) + " " + to_string(borderColor.g) + " "
			+ to_string(borderColor.b);

		// Add filler info
		if (filled)
			info = "FILLED_" + info + " " + to_string(fillColor.r) + " " + to_string(fillColor.g) + " " + to_string(fillColor.b);

		info += "\n";

		return info;
	};

	// Overridden by Triangle and Bezier Curve methods
	virtual void newPoint(int x, int y){ return; }

	// Overridden by Triangle and Bezier Curve methods
	virtual bool finished(){ return true; }

	// Overridden by Bezier Curve's method
	virtual void forceFinish(int x, int y) { return; }
};