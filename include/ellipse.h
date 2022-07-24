#pragma once
#include "shape.h"

class CEllipse : public CShape
{
private:
	int a;		// Radius in x-axis
	int b;		// Radius in y-axis
	int cx, cy; // Center of ellipse

public:
	CEllipse(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2, bool filled)
		: CShape(r1, g1, b1, r2, g2, b2, filled, "ELLIPSE ")
	{
		anchorPoint.x = x0; 
		anchorPoint.y = y0;
		update(x1, y1);
	}

	~CEllipse(){ cout << "Se destruyo un elipse" << endl; }

	void update(int x, int y)
	{
		int y0 = anchorPoint.y;

		if (y >= y0)
			swap(y0, y);

		// Update radii values
		a = (x - anchorPoint.x) >> 1;
		b = (y0 - y) >> 1;

		// Update the center of the ellipse
		cy = y + b;
		cx = anchorPoint.x + a;

		// Update position of the bounding box
		setBoundingBox(cx - a, cy - b, cx + a, cy + b);
	}

	// Render the bounding box
	void renderCtrlPoints()
	{
		renderBox();
	}

	// Draw the 4 symmetrical points of the point (x,y)
	void ellipsePoints(int x, int y)
	{
		putPixel(cx + x, cy + y, borderColor);
		putPixel(cx - x, cy + y, borderColor);
		putPixel(cx + x, cy - y, borderColor);
		putPixel(cx - x, cy - y, borderColor);
	}

	void fillEllipse(int x, int y)
	{
		horizontalLine(cx - x, cx + x, cy + y, fillColor);
		horizontalLine(cx - x, cx + x, cy - y, fillColor);
	}

	// Draw border and filler in the same iteration
	void render(const bool mode)
	{
		int x, y, d;

		x = 0;
		y = b;

		int ap2, bp2;
		int  incE, incS, incSE, varE, varSE,  varS;
		ap2 = a * a; bp2 = b * b;
		
		// Initial values
		d = b * ((b - ap2) << 2) + ap2;
		incE = 12 * bp2;
		incSE = (3 * bp2 - ((ap2 * (b - 1)) << 1)) << 2;

		varE = bp2 << 3;
		varSE = (ap2 + bp2) << 3;
		varS = ap2 << 3;


		// Draw initial 4 points
		ellipsePoints(x, y);

		if (filled)
		{
			// Mode 1: Draw and fill while the tangent line to the point has slope [-1,0]
			while (((bp2 * (x + 1)) << 1) < ap2 * ((y << 1) - 1))
			{
				if (d < 0)
				{
					d += incE;	
					incSE += varE;
				}
				else
				{
					d += incSE;
					incSE += varSE;
					y -= 1;
					fillEllipse(x, y);
				}
				x += 1;
				incE += varE;
				ellipsePoints(x, y);
			}

			// Mode 2: Draw and fill while the tangent line to the point has slope (-inf,-1]
			incS = (ap2 * (3 - (y << 1))) << 2;
			incSE = ((bp2*(x+1)) << 3) + ((ap2*(3 - (y << 1))) << 2);
			d = bp2 * (((x * x + x) << 2) + 1) + ap2 * ((y * y - (y << 1) + 1 - bp2) << 2);
			while (y > 0)
			{
				if (d < 0)
				{
					d += incSE;
					incSE += varSE;
					x += 1;
				}
				else
				{
					d += incS;
					incSE += varS;
				}
				incS += varS;
				y -= 1;
				fillEllipse(x, y);
				ellipsePoints(x, y);
			}
		}
		else
		{
			// Mode 1: Draw and fill while the tangent line to the point has slope [-1,0]
			while (((bp2 * (x + 1)) << 1) < ap2 * ((y << 1) - 1))
			{
				if (d < 0)
				{
					d += incE;
					incSE += varE;
				}
				else
				{
					d += incSE;
					incSE += varSE;
					y -= 1;
				}
				incE += varE;
				x += 1;
				ellipsePoints(x, y);
			}

			// Mode 2: Draw and fill while the tangent line to the point has slope (-inf,-1]
			incS = (ap2 * (3 - (y << 1))) << 2;
			incSE = ((bp2 * (x + 1)) << 3) + ((ap2 * (3 - (y << 1))) << 2);
			d = bp2 * (((x * x + x) << 2) + 1) + ap2 * ((y * y - (y << 1) + 1 - bp2) << 2);

			while (y > 0)
			{
				if (d < 0)
				{
					d += incSE;
					incSE += varSE;
					x += 1;
				}
				else
				{
					d += incS;
					incSE += varS;
				}
				incS += varS;
				y -= 1;
				ellipsePoints(x, y);
			}
		}
	}

	bool onClick(int x, int y)
	{
		int dx = x - cx;
		int dy = y - cy;

		return (((float)(dx * dx) / (a * a)) + ((float)(dy * dy) / (b * b))) <= 1.2;
	}

	// We check if the click fell on a vertex
	bool clickedCtrlPoint(int x, int y)
	{
		int dx, dy;
		for (int i = 0; i < 4; i++)
		{
			dx = (x - boxPoints[i].x);
			dy = (y - boxPoints[i].y);
			// Check squared distance between vertex i and the click, threshold: 4 pixels
			if ((dx * dx + dy * dy) <= 16)
			{
				pointSelected = &boxPoints[i];
				indexSelected = i;
				return true ;
			}
		}
		return false;
	}

	bool xResize(int i, int op, int x)
	{
		int dx = x - boxPoints[op].x;

		if ((i > 1 && dx < 5) || (i < 2 && dx > -5))
			return false;

		boxPoints[i].x = x;
		boxPoints[i - ((i & 1) << 1) + 1].x = x;

		return true;
	}

	bool yResize(int i, int op, int y)
	{
		int dy = boxPoints[op].y - y;
		if ((i % 3 == 0 && dy < 5) || (i % 3 != 0 && dy > -5))
			return false;

		boxPoints[i].y = y;
		boxPoints[-(i - 3) % 4].y = y;

		return true;
	}

	void onMove(int x, int y)
	{
		if (pointSelected)
		{
			int i = indexSelected;
			int op = (i + 2) % 4;

			bool resize = false;
			resize |= xResize(i, op, x);
			resize |= yResize(i, op, y);

			if (resize)
			{
				a = (boxPoints[2].x - boxPoints[0].x) >> 1;
				b = (boxPoints[2].y - boxPoints[0].y) >> 1;
				cx = boxPoints[0].x + a;
				cy = boxPoints[0].y + b;
			}
		}
		else
		{	// Move the whole ellipse
			int dx = x - anchorPoint.x;
			int dy = y - anchorPoint.y;

			anchorPoint.x = x;
			anchorPoint.y = y;

			cx += dx; cy += dy;
			moveBoundingBox(dx, dy);
		}
	}
};