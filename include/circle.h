#pragma once
#include "shape.h"
#include "boxableShape.h"

class CCircle : public CShape, public BoxableShape
{
private:
	int r; // Radius

public:
	CCircle(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2)
		: CShape(r1, g1, b1, r2, g2, b2), BoxableShape(x0, y0, x1, y1) 
	{
		// Set the center of the circle
		anchorPoint.x = x0;
		anchorPoint.y = y0;
	}

	~CCircle()
	{
		cout << "Se destruyo un circulo" << endl;
	}

	void update(int x1, int y1)
	{
		r = anchorPoint.distance(x1, y1);
		cout << r << endl;
		//setBoundingBox(x1 ,y1);
	}
	
	void setAnchorPoint(int x, int y)
	{
		BoxableShape::setAnchorPoint(x, y);
	}

	void circlePoints(int x, int y, Color color)
	{	
		int cx = anchorPoint.x, cy = anchorPoint.y;

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
				y -= 1;
			}
			x += 1;
			circlePoints(x, y, borderColor);
		}
	}

	bool onClick(int x, int y)
	{
		return false;
	}

	void onMove(int x1, int y1)
	{
	}
};