#pragma once
#include "shape.h"

class CCircle : public CShape
{
private:
	int r;				// Radius
	int cx, cy;			// Center
	Point ctrlRadius;

public:
	CCircle(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2, bool filled, bool fromFile)
		: CShape(r1, g1, b1, r2, g2, b2, filled, "CIRCLE ")
	{
		if (!fromFile)
		{
			cx = x0; cy = y0;
			r = (int)sqrt(pow(x1 - cx, 2) + pow(y1 - cy, 2));
		}
		else
		{
			if (x0 > x1)
			{
				swap(x0, x1); 
				swap(y0, y1);
			}

			r =  (x1 - x0) >> 1;
			cx = x0 + r; cy = y0 + r;
		}
		ctrlRadius = Point(cx + r, cy - r);
	}
		
	~CCircle(){ cout << "Se destruyo un circulo" << endl; }

	void update(int x1, int y1)
	{
		r = (int)sqrt(pow(x1 - cx, 2) + pow(y1 - cy, 2));
		ctrlRadius = { cx + r, cy - r };
	}

	// Render the bounding box
	void renderCtrlPoints()
	{
		drawLine(cx, cy, ctrlRadius.x, ctrlRadius.y, borderColor);

		glColor3f(borderColor.r, borderColor.g, borderColor.b);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
			glVertex2i(ctrlRadius.x, ctrlRadius.y);
		glEnd();
		glPointSize(1.0f);
	}

	void circlePoints(int x, int y)
	{	
		for(int i=0; i<2; i++)
		{
			putPixel(cx+x, cy+y, borderColor);
			putPixel(cx-x, cy+y, borderColor);
			putPixel(cx-y, cy+x, borderColor);
			putPixel(cx+y, cy+x, borderColor);
			x = -x; y = -y;
		}
	}

	void render(const bool mode)
	{	
		int x = 0, y = r, d = 1 - r;

		if (filled)
		{	// Draw border and filler in the same iteration

			// Draw initial 8 points and middle line filler
			circlePoints(x, y);
			horizontalLine(cx - r, cx + r, cy, fillColor);

			// Draw one octant and reflect it's points
			for (; y > x; x++)
			{
				if (d < 0)
					d += (x << 1) + 3;
				else
				{
					d += ((x - y) << 1) + 5;
					y--;

					horizontalLine(cx - x, cx + x, cy + y, fillColor);
					horizontalLine(cx - x, cx + x, cy - y, fillColor);
				}

				horizontalLine(cx - y, cx + y, cy - x, fillColor);
				horizontalLine(cx - y, cx + y, cy + x, fillColor);
				circlePoints(x, y);
			}
		}
		else
		{	// Draw initial 8 points 
			circlePoints(x, y);

			// Draw one octant and reflect it's points
			for (; y > x; x++)
			{
				if (d < 0)
					d += (x << 1) + 3;
				else
				{
					d += ((x - y) << 1) + 5;
					y--;
				}
				circlePoints(x, y);
			}
		}
	}

	bool onClick(int x, int y)
	{
		return ((int)sqrt(pow(x - cx, 2) + pow(y - cy, 2)) <= r + 3);
	}

	// We check if the click fell on the control point
	bool clickedCtrlPoint(int x, int y)
	{
		int dx = (x - ctrlRadius.x);
		int dy = (y - ctrlRadius.y);

		// Check squared distance between vertex i and the click, threshold: 4 pixels
		if ((dx * dx + dy * dy) <= 16)
		{
			pointSelected = &ctrlRadius;
			return true;
		}
			
		return false;
	}

	void onMove(int x, int y)
	{
		if (pointSelected)
		{
			pointSelected->x = x;
			pointSelected->y = y;

			r = static_cast<int>(sqrt(pow(x - cx, 2) + pow(y - cy, 2)) * 0.75);
		}
		else
		{	// Move the whole circle
			int dx = x - anchorPoint.x;
			int dy = y - anchorPoint.y;

			cx += dx; cy += dy;
			ctrlRadius.x += dx; ctrlRadius.y += dy;
			anchorPoint.x = x; anchorPoint.y = y;
		}
	}

	std::string getInfo() override
	{
		info += to_string(cx - r) + " " + to_string(cy - r) + " " + to_string(cx + r) + " " + to_string(cy + r) + " ";
		
		// Add border info
		info += to_string(borderColor.r) + " " + to_string(borderColor.g) + " "
			+ to_string(borderColor.b);

		// Add filler info
		if (filled)
			info = "FILLED_" + info + " " + to_string(fillColor.r) + " " + to_string(fillColor.g) + " " + to_string(fillColor.b);

		return info + "\n";
	}
};