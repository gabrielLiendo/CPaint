#pragma once
#include "shape.h"

class C_Circle : public CShape
{
public:
	C_Circle(float r, float g, float b) : CShape(r, g, b) {}

	~C_Circle()
	{
		cout << "Se destruyó un circulo" << endl;
	}

	void update(int x1, int y1)
	{
		// Update bounding box corner
		this->x1 = x1; this->y1 = y1;
	}
		
	void circlePoints(int x, int y, int cx, int cy)
	{	
		for(int i=0; i<2; i++)
		{
			putPixel(x+cx, y+cy);
			putPixel(-x+cx, y+cy);
			putPixel(-y+cx, x+cy);
			putPixel(y+cx, x+cy);
			x = -x; y = -y;
		}
	}

	void render(const char* mode)
	{
		setColor(fillColor[0], fillColor[1], fillColor[2]);

		if (y1 > y0)
			swap(y1, y0);

		int cx = x0 + ((x1 - x0) >> 1);
		int cy = y1 + ((y0 - y1) >> 1);
		int x, y, d, r;

		x = 0;
		r = y0 - cy;
		y = r;
		d = 1 - r;
		circlePoints(x, y, cx, cy);
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
			circlePoints(x, y, cx, cy);
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