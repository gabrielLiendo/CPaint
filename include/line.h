#pragma once
#include "shape.h"

class CLine : public CShape
{
private:
	CtrlPoint points[2];
	
public:
	CLine(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2)
		: CShape(x0, y0, r1, g1, b1, r2, g2, b2) 
	{
		points[0] = CtrlPoint(x0, y0);
		points[1] = CtrlPoint(x1, y1);
	}

	~CLine(){ cout << "Se destruyo una linea" << endl; }

	void update(int x1, int y1)
	{
		points[1].x = x1; points[1].y = y1;
	}

	void renderCtrlPoints()
	{
		// Render each vertex
		for (int i=0; i < 2; i++)
			points[i].renderCtrlPoint();
	}

	void render(const bool modeHardware)
	{	
		int x0 = points[0].x, y0 = points[0].y;
		int x1 = points[1].x, y1 = points[1].y;

		if(modeHardware) // Hardware
		{
			glColor3f(fillColor.r, fillColor.g, fillColor.b);
			glBegin(GL_LINES);
				glVertex2i(x0, y0);
				glVertex2i(x1, y1);
			glEnd();
		}
		else 
		{	
			// Draw line using Bresenham's algorithm
			int x, y, dx, dy, d, incN, incE, incNE, varX, varY;
			x = x0;
			y = y0;
			dx = abs(x1 - x);
			dy = abs(y1 - y);
			d = dx - (dy << 1);
			incN =  (dx << 1);
			incE = -(dy << 1);
			incNE = (dx-dy) << 1;
			
			// Draw the initial pixel
			putPixel(x0, y0, fillColor);

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
					putPixel(x, y, fillColor);
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
					putPixel(x, y, fillColor);
				}
			}
			
		}
	}

	bool onClick(int x, int y) 
	{
		// We check if the click fell close to the line, threshold: 4 pixels
		int x0 = points[0].x, y0 = points[0].y;
		int x1 = points[1].x, y1 = points[1].y;

		int a = y0 - y1;
		int b = x1 - x0;
		int c = x0 * y1 - x1 * y0;
		int distance = (int) abs(a * x + b * y + c) / sqrt(a * a + b * b);

		return distance <= 4;
	}

	void clickedCtrlPoint(int x, int y)
	{
		// We check if the click fell on a vertex
		int dx, dy;
		for (int i = 0; i < 2; i++)
		{
			dx = (x - points[i].x);
			dy = (y - points[i].y);
			// Check squared distance between vertex i and the click
			if ((dx * dx + dy * dy) <= 16)
			{
				pointSelected = &points[i];
				return;
			}
		}
	}

	void onMove(int x1, int y1)
	{
		if (pointSelected)
		{	// Only move the vertex selected
			pointSelected->x = x1;
			pointSelected->y = y1;
		}
		else
		{	// Move the whole line
			int dx = x1 - anchorPoint.x;
			int dy = y1 - anchorPoint.y;

			points[0].x += dx; points[0].y += dy;
			points[1].x += dx; points[1].y += dy;

			anchorPoint.x = x1; anchorPoint.y = y1;
		}
	}
};