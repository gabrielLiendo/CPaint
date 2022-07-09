#pragma once
#include "shape.h"
#include "boxableShape.h"

class CCircle : public CShape, public BoxableShape
{
private:
	int r; // Radius
	CtrlPoint center;

public:
	CCircle(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2)
		: CShape(x0, y0, r1, g1, b1, r2, g2, b2), BoxableShape(x0, y0, x1, y1)
	{
		r = anchorPoint.distance(x1, y1);
		center.x = x0; center.y = y0;
	}

	~CCircle(){ cout << "Se destruyo un circulo" << endl; }

	void update(int x1, int y1)
	{
		r = anchorPoint.distance(x1, y1);
		setBoundingBox(anchorPoint.x-r, anchorPoint.y+r, anchorPoint.x+r , anchorPoint.y-r);
	}

	// Render the bounding box
	void renderCtrlPoints()
	{
		renderBox();
	}

	void circlePoints(int x, int y, Color color)
	{	
		int cx = center.x, cy = center.y;

		for(int i=0; i<2; i++)
		{
			putPixel(cx+x, cy+y, color);
			putPixel(cx-x, cy+y, color);
			putPixel(cx-y, cy+x, color);
			putPixel(cx+y, cy+x, color);
			x = -x; y = -y;
		}
	}

	void render(const bool mode)
	{	
		int x, y, d;
		int cx = center.x, cy = center.y;
		
		x = 0;
		y = r;

		// Draw content
		int x0 = cx - r, x1 = cx + r;
		int y0 = cy + r, y1 = cy + r;
		int xDec = 0;
		// x = cx , y = cy;
		drawLine(x0, cy, x1, cy, fillColor);
		//for (int i = 1; i < 4; i++) {
		//	drawLine(x0 + xDec, cy - i, x1 - xDec, cy - i, fillColor);
		//	drawLine(x0 + xDec, cy + i, x1 - xDec, cy + i, fillColor);
		//	xDec += i % 2 == 0 ? 0 : 1;
		//}
			
	
		// Draw border
		x = 0;
		y = r;
		d = 1 - r;
		circlePoints(x, y, borderColor);
		while (y > x)
		{
			if (d < 0)
				d += (x << 1) + 3;
			else
			{
				d += ((x - y) << 1) + 5;
				y--;
			}
			x++;
			circlePoints(x, y, borderColor);
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
				iPointSelected = i;
				return true;
			}
		}

		return center.distance(x,y) <= r + 3;
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

			center.x += dx; center.y += dy;
			moveBoundingBox(dx, dy);
		}
	}
};