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

	Point() { x = 0; y = 0; }

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
	Point anchorPoint;					// First point clicked when moving shape
	Point* pointHovered = nullptr;
	Point *pointSelected = nullptr;		// Pointer to selected ctrl point
	string name;						// Shape name

	int level = 0;						// Level in layers of canvas
	int indexSelected = -1;				// Index of the point selected of the bounding box
	bool filled = true;

public:
	// Contructor for figures without a filler (ex. line and bezier curves)
	CShape(float r, float g, float b, string name)
	{
		borderColor.r = r; borderColor.g = g; borderColor.b = b;
		this->name = name;
	}

	// Contructor for figures with filler
	CShape(float r1, float g1, float b1, float r2, float g2, float b2, bool filled, string name)
	{	
		fillColor.r = r1; fillColor.g = g1; fillColor.b = b1;
		borderColor.r = r2; borderColor.g = g2; borderColor.b = b2;
		this->filled = filled;
		this->name = name;
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

	// Mark control point as deselected
	void release()
	{
		pointSelected = nullptr;
	}

	void clickedCtrlPoint(int x, int y)
	{
		pointSelected = pointHovered;
	};

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

	// Draw a line using Bresenham's algorithm
	void drawLine(int x0, int y0, int x1, int y1, Color c)
	{
		int x, y, dx, dy, d, incN, incE, incNE, varX, varY, i;
		x = x0;
		y = y0;
		dx = abs(x1 - x0);
		dy = abs(y1 - y0);
		incN = (dx << 1);
		incE = -(dy << 1);
		incNE = (dx - dy) << 1;

		varX = x1 < x0 ? -1 : 1;
		varY = y1 < y0 ? -1 : 1;

		// Draw the initial pixel
		putPixel(x0, y0, c);

		// If |m| < 1 (abs(dx) > abs(dy)) we iterate over the x-axis, otherwise we iterate over the y-axis
		// The decision factor 'd' should be diferent for each type of iteration (I realized this after a looooong time)
		if (dy < dx)
		{
			d = dx - (dy << 1);
			for (i = 0; i < dx; i++)
			{
				if (d < 0)
				{
					d += incNE;
					y += varY;
				}
				else
					d += incE;
				x += varX;
				putPixel(x, y, c);
			}
		}
		else
		{
			d = (dx << 1) - dy; /*I hate this line*/
			for (i = 0; i < dy; i++)
			{
				if (d < 0)
					d += incN;
				else {
					d += incNE;
					x += varX;
				}
				y += varY;
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

		boxPoints[0].x = x0; boxPoints[0].y = y0;
		boxPoints[1].x = x0; boxPoints[1].y = y1;
		boxPoints[2].x = x1; boxPoints[2].y = y1;
		boxPoints[3].x = x1; boxPoints[3].y = y0;
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

	/* We change the xand y values of the adjecent points to the point i of the bounding box,
	   we allow the resize if the point i and its opposite are father than 2 pixels apart and the y axis */
	bool yResize(int i, int op, int y)
	{
		int dy = boxPoints[op].y - y;
		if ((i % 3 == 0 && dy < 2) || (i % 3 != 0 && dy > -2))
			return false;

		boxPoints[i].y = y;
		boxPoints[-(i - 3) % 4].y = y;	// Cool little indexing trick
		return true;
	}

	/* We change the xand y values of the adjecent points to the point i of the bounding box,
	   we allow the resize if the point i and its opposite are father than 2 pixels apart and the x axis */
	bool xResize(int i, int op, int x)
	{
		int dx = x - boxPoints[op].x;

		if ((i > 1 && dx < 2) || (i < 2 && dx > -2))
			return false;

		boxPoints[i].x = x;
		boxPoints[i - ((i & 1) << 1) + 1].x = x; // Disgusting and evil indexing
		return true;
	}

	// Render dashed bounding box
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
	}

	virtual void update(int x, int y) = 0;

	// Draw the respective ctrl structure to the figure (just the vertex, just the bounding box, etc.)
	virtual void drawCtrlStructure() = 0;

	virtual void render(const bool modeHardware) = 0;

	// Return true when user hovers over the figure
	virtual bool onClick(int x, int y) = 0;

	virtual bool hoveredCtrlPoint(int x, int y) = 0;

	
	
	virtual void onMove(int x1, int y1) = 0;

	// Return string with the info to write in file when savign scenes
	virtual std::string getInfo()
	{
		std::string info = name;

		// Add position info
		info += to_string(boxPoints[0].x) + " " + to_string(boxPoints[0].y) + " "
			+ to_string(boxPoints[2].x) + " " + to_string(boxPoints[2].y) + " ";

		// Add border info
		info += to_string(borderColor.r) + " " + to_string(borderColor.g) + " "
			+ to_string(borderColor.b);

		// Add filler info
		if (filled)
			info = "FILLED_" + info + " " + to_string(fillColor.r) + " " + to_string(fillColor.g) + " " + to_string(fillColor.b);

		return info + "\n";
	};

	/*  The following methods are not used by most shapes, so most of the times they will return meaningless values,
		except if they are called by the types of objects they are designed for, this helps keep things short and sweet  */
	
	// Overridden by Triangle and Bezier Curve methods
	virtual void newPoint(int x, int y){ return; }

	// Overridden by Triangle and Bezier Curve methods
	virtual bool finished(){ return true; }

	// Overridden by Bezier Curve's method
	virtual void forceFinish(int x, int y) { return; }
};