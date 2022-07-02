#pragma once
#include "shape.h"

class CLine : public CShape
{
private:
	CtrlPoint points[2];
	CtrlPoint *pointSelected = nullptr;

public:
	CLine(int x0, int y0, int x1, int y1, float r, float g, float b) : CShape(r, g, b) 
	{
		points[0] = CtrlPoint(x0, y0);
		points[1] = CtrlPoint(x1, y1);
	}

	~CLine()
	{
		cout << "Se destruyo una linea" << endl;
	}

	void update(int x1, int y1)
	{
		points[1].setPosition(x1, y1);
	}

	void setRefPoint(int x, int y)
	{

	}

	void release () override
	{
		selected = false;
		pointSelected = nullptr;
	}

	void render(const char* mode)
	{	
		int x0 = points[0].getX(), y0 = points[0].getY();
		int x1 = points[1].getX(), y1 = points[1].getY();

		glColor3f(fillColor[0], fillColor[1], fillColor[2]);
		// Render Line
		if(mode=="Hardware")
		{
			glBegin(GL_LINES);
				glVertex2i(x0, y0);
				glVertex2i(x1, y1);
			glEnd();
		}
		else 
		{	
			// Draw using Bresenham's algorithm
			int x, y, dx, dy, d, incN, incE, incNE, incSE;
			x = x0;
			y = y0;
			dx = abs(x1 - x);
			dy = abs(y1 - y);
			d = dx - (dy << 1);
			incN = (dx << 1);
			incE = -(dy << 1);
			incNE = (dx-dy) << 1;
			incSE = -incNE;
			
			// d is the middle point between pixels evaluated on the line
			// If -1 > m < 1 we iterate over x  dx > dy
			// If m < -1 or m >  1 we iterate over y
			//cout << dx << " " << dy << endl;
			putPixel(x0, y0);
			if (dx >= dy) {
				
				if (x > x1)
				{
					swap(x, x1);
					y = y1;
				}

				if (y1 < y)
				{
					x = x0;
					for (; x < x1; x++)
					{
						if (d > 0) {
							d += incSE;
							y--;
						}
						else
						{
							d += incE;
						}
						putPixel(x, y);
					}
				}
				else {
					for (; x < x1; x++)
					{
						if (d > 0)
							d += incE;
						else
						{
							d += incNE;
							y++;
						}
						putPixel(x, y);
					}
				}
			}
			else 
			{
				if (y > y1)
				{
					swap(y, y1);
					x = x1;
				}

				for (; y < y1; y++)
				{
					if (d > 0) {
						d += incNE;
						x++;
					}
					else
					{
						d += incN;
					}
					putPixel(x, y);
				}
			}
		}

		// Render control points if shape is select
		if (selected) {
			for (auto p : points)
				p.renderCtrlPoint();
		}
	}

	bool onClick(int x, int y) 
	{
		int x0 = points[0].getX(), y0 = points[0].getY();
		int x1 = points[1].getX(), y1 = points[1].getY();

		for (int i = 0; i < 2; i++)
		{
			if (points[i].distance(x, y) <= 4) {
				pointSelected = &points[i];
				return true;
			}
		}

		int a = y0 - y1;
		int b = x1 - x0;
		int c = x0 * y1 - x1 * y0;
		int distance = (int) abs(a * x + b * y + c) / sqrt(a * a + b * b);

		return distance <= 4;
	}

	void onMove(int x1, int y1)
	{
		if (pointSelected)
			pointSelected->setPosition(x1, y1);
	}

};