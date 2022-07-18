#pragma once
#include "shape.h"

class CEllipse : public CShape
{
private:
	int a; // Radius in x-axis
	int b; // Radius in y-axis
	int cx, cy; // Center of ellipse

public:
	CEllipse(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2, bool filled)
		: CShape(r1, g1, b1, r2, g2, b2, filled, "ELLIPSE ")
	{
		anchorPoint.x = x0; anchorPoint.y = y0;
		update(x1, y1);
	}

	~CEllipse(){ cout << "Se destruyo un elipse" << endl; }

	void update(int x, int y)
	{
		int x0 = anchorPoint.x;
		int y0 = anchorPoint.y;

		if (y >= y0)
			swap(y0, y);

		// Update radii values
		a = ((x - x0) >> 1);
		b = ((y0 - y) >> 1);

		// Update the center of the ellipse
		cy = y + b;
		cx = x0 + a;

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
		incSE = (3 * bp2 - (ap2 * (b - 1)) << 1) << 2;

		varE = bp2  << 3;
		varSE = (ap2 + bp2) << 3;
		varS = ap2 << 3;

		if (filled)
		{
			// Draw initial 4 points
			ellipsePoints(x, y);

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
			// Draw initial 4 points
			ellipsePoints(x, y);

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
		if (x > boxPoints[0].x - 3 && x < boxPoints[2].x + 3 && y > boxPoints[0].y - 3 && y < boxPoints[2].y + 3)
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
					return true;
				}
			}
			dx = x - cx;
			dy = y - cy;

			return (((float)(dx * dx) / (a * a)) + ((float)(dy * dy) / (b * b))) <= 1.2;
		}
		return false;
	}


	void onMove(int x, int y)
	{
		if (pointSelected)
		{
			int x0 = boxPoints[0].x, y0 = boxPoints[0].y;
			int i = indexSelected;
			int op = (i + 2) % 4;

			resizeBoxX(i, op, x);
			resizeBoxY(i, op, y);

			// Update radii values
			a = ((x - x0) >> 1);
			b = ((y0 - y) >> 1);

			// Update the center of the ellipse
			cy = y + b;
			cx = x0 + a;
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