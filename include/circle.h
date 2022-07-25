#pragma once
#include "shape.h"

class CCircle : public CShape
{
private:
	int r;				// Radius
	int cx, cy;			// Center
	Point ctrlRadius;	// Control point for resize

public:
	CCircle(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2, bool filled, bool fromFile)
		: CShape(r1, g1, b1, r2, g2, b2, filled, "CIRCLE ")
	{
		if (!fromFile)
		{
			cx = x0; 
			cy = y0;
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
			cx = x0 + r; 
			cy = y0 + r;
		}
		ctrlRadius = Point(cx + r, cy);
	}
		
	~CCircle(){ cout << "Se destruyo un circulo" << endl; }

	void update(int x1, int y1)
	{
		r = (int)sqrt(pow(x1 - cx, 2) + pow(y1 - cy, 2));
		ctrlRadius.x = cx + r;
	}

	// Render the ctrl radius line and point
	void drawCtrlStructure()
	{
		drawLine(cx, cy, ctrlRadius.x, ctrlRadius.y, borderColor);

		glColor3f(borderColor.r, borderColor.g, borderColor.b);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
			glVertex2i(ctrlRadius.x, ctrlRadius.y);
		glEnd();
		glPointSize(1.0f);
	}

	// Draw 8 symetrical points
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
		int x = 1, y = r, d = 1 - r;

		// Draw initial 8 points 
		circlePoints(0, r);

		if (filled)
		{	// Draw border and filler in the same iteration

			horizontalLine(cx - r + 1, cx + r - 1, cy, fillColor);
			// Draw one octant and reflect it's points
			for (; y > x; x++)
			{
				if (d < 0)
					d += (x << 1) + 3;
				else
				{
					d += ((x - y) << 1) + 5;
					y--;
					// We only draw thing filler lines when the y value changes
					horizontalLine(cx - x + 1, cx + x - 1, cy + y, fillColor);
					horizontalLine(cx - x + 1, cx + x - 1, cy - y, fillColor);
				}

				horizontalLine(cx - y + 1, cx + y - 1, cy - x, fillColor);
				horizontalLine(cx - y + 1, cx + y - 1, cy + x, fillColor);
				circlePoints(x, y);
			}
		}
		else
		{	
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
	bool hoveredCtrlPoint(int x, int y)
	{
		int dx = (x - ctrlRadius.x);
		int dy = (y - ctrlRadius.y);

		// Check squared distance between vertex i and the click, threshold: 4 pixels
		if ((dx * dx + dy * dy) <= 16)
		{
			pointHovered = &ctrlRadius;
			return true;
		}

		return false;
	}

	void onMove(int x, int y)
	{
		if (pointSelected)
		{
			// We adjust the new radius
			int oldR = static_cast<int>(sqrt(pow(pointSelected->x - cx, 2) + pow(pointSelected->y - cy, 2)));
			int newR = static_cast<int>(sqrt(pow(x - cx, 2) + pow(y - cy, 2)));
			int diff = newR - oldR;

			if(r + diff >= 1)
				r += diff;

			pointSelected->x = x;
			pointSelected->y = y;
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
		std::string info = name;

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