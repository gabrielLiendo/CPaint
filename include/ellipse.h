#pragma once
#include "shape.h"
#include "boxableShape.h"

class CEllipse : public CShape, public BoxableShape
{
private:
	int a; // Radius in x-axis
	int b; // Radius in y-axis
	int cx, cy; // Center of ellipse

public:
	CEllipse(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2)
		: CShape(x0, y0, r1, g1, b1, r2, g2, b2), BoxableShape(x0, y0, x1, y1) {}

	~CEllipse(){ cout << "Se destruyo un elipse" << endl; }

	void update(int x1, int y1)
	{
		int x0 = anchorPoint.x;
		int y0 = anchorPoint.y;

		if (y1 >= y0)
			swap(y0, y1);

		// Update radii values
		a = ((x1 - x0) >> 1);
		b = ((y0 - y1) >> 1);

		// Update the center of the ellipse
		cy = y1 + b;
		cx = x0 + a;

		// Update position of the bounding box
		setBoundingBox(cx + a, cy - b, cx - a, cy + b);
	}

	// Render the bounding box
	void renderCtrlPoints()
	{
		renderBox();
	}

	void ellipseFill(int x, int y, Color c)
	{
		hLine(cx - x, cx + x, cy + y, c);
		hLine(cx - x, cx + x, cy - y, c);
	}

	// Draw the 4 symmetrical points of the point (x,y)
	void ellipsePoints(int x, int y, Color c)
	{
		putPixel(cx + x, cy + y, c);
		putPixel(cx - x, cy + y, c);
		putPixel(cx + x, cy - y, c);
		putPixel(cx - x, cy - y, c);
	}

	void render(const bool mode)
	{
		int x, y, d, ap2, bp2;
		ap2 = a * a; bp2 = b * b;
		x = 0;
		y = b;
		d = b * ((b - ap2) << 2) + ap2;

		// Draw initial 4 points
		ellipsePoints(x, y, borderColor);

		// Mode 1: Draw and fill while the tangent line to the point has slope [-1,0]
		while ( ((bp2*(x+1)) << 1) < ap2*((y << 1) - 1))
		{
			if (d < 0)
				d += (bp2*((x << 1) + 3)) << 2;
			else
			{
				d += (bp2*((x+3) << 1) + (ap2*(1-y) << 1)) << 2;
				y -= 1;
				ellipseFill(x, y, fillColor);
			}
			x += 1;
			ellipsePoints(x, y, borderColor);
		}

		// Mode 2: Draw and fill while the tangent line to the point has slope (-inf,-1]
		d = bp2 * (((x*x + x) << 2) + 1) + ap2 * ((y * y - (y << 1) + 1 - bp2) << 2);
		while (y > 0)
		{
			if (d < 0)
			{
				d += (((bp2 * (x + 1)) << 1) + ap2 * (3 - (y << 1))) << 2;
				x += 1;
			}
			else
				d += (ap2 * (3 - (y << 1))) << 2;
			y -= 1;

			ellipseFill(x, y, fillColor);
			ellipsePoints(x, y, borderColor);
		}
	}

	bool onClick(int x, int y)
	{
		int dx = x - cx;
		int dy = y - cy;

		return (((float)(dx * dx)/(a * a)) + ((float)(dy * dy) / (b * b))) <= 1.2;
	}

	void clickedCtrlPoint(int x, int y)
	{
	}

	void onMove(int x1, int y1)
	{
		if (pointSelected)
		{
			//int dx = x1 - anchorPoint.x;
			//anchorPoint.x = x1;
			//anchorPoint.y = y1;
			//r = r + dx;

			//setBoundingBox(boxPoints[0].x - r, boxPoints[0].y + r, boxPoints[2].y + r, boxPoints[2].y - r);
		}
		else
		{	// Move the whole ellipse
			int dx = x1 - anchorPoint.x;
			int dy = y1 - anchorPoint.y;

			anchorPoint.x = x1;
			anchorPoint.y = y1;

			cx += dx; cy += dy;
			moveBoundingBox(dx, dy);
		}
	}
};