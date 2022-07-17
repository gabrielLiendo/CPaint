#pragma once
#include "shape.h"

class CCircle : public CShape
{
private:
	int r;      // Radius
	int cx, cy; // Center;

public:
	CCircle(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2, bool filled, bool fromFile)
		: CShape(x0, y0, r1, g1, b1, r2, g2, b2, filled, "CIRCLE ")
	{
		if (!fromFile)
		{
			cx = x0; cy = y0;
			r = (int)sqrt(pow(x1 - anchorPoint.x, 2) + pow(y1 - anchorPoint.y, 2) * 1.0);
		}
		else
		{
			setBoundingBox(x0, y0, x1, y1);
			r = (boxPoints[0].y - boxPoints[1].y) >> 1;
			cx = boxPoints[0].x + r;
			cy = boxPoints[0].y - r;
		}
	}
		
	~CCircle(){ cout << "Se destruyo un circulo" << endl; }

	void update(int x1, int y1)
	{
		r = (int)sqrt(pow(x1 - anchorPoint.x, 2) + pow(y1 - anchorPoint.y, 2) * 1.0);
		setBoundingBox(anchorPoint.x-r, anchorPoint.y+r, anchorPoint.x+r , anchorPoint.y-r);
	}

	// Render the bounding box
	void renderCtrlPoints()
	{
		renderBox();
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
		// We check if the click fell on a vertex
		int dx, dy;
		for (int i = 0; i < 4; i++)
		{
			dx = (x - boxPoints[i].x);
			dy = (y - boxPoints[i].y);
			// Check squared distance between vertex i and the click, threshold: 4 pixels
			if ((dx * dx + dy * dy) <= 16)
			{
				pointSelected = &boxPoints[i];
				return true;
			}
		}

		return (int)sqrt(pow(x - cx, 2) + pow(y - cy, 2) * 1.0) <= r + 3;
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
		{	// Move the whole circle
			int dx = x1 - anchorPoint.x;
			int dy = y1 - anchorPoint.y;

			anchorPoint.x = x1; 
			anchorPoint.y = y1;

			cx += dx; cy += dy;
			moveBoundingBox(dx, dy);
		}
	}
};