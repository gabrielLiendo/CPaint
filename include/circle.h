#pragma once
#include "point2D.h"
#include "shape.h"

class C_Circle : public CShape
{
public:
	C_Circle(float r, float g, float b) : CShape(r, g, b) {}

	~C_Circle()
	{
		cout << "Se destruyó un circulo" << endl;
	}

	void set(int x0, int y0)
	{	
		ctrlPoints[0].set(x0, y0);
		ctrlPoints[1].set(x0, y0);
	}

	void update(int x1, int y1)
	{
		ctrlPoints[1].set(x1, y1);
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

	void render()
	{
		setColor(fillColor[0], fillColor[1], fillColor[2]);

		int x0Ctrl = ctrlPoints[0].getX(), y0Ctrl = ctrlPoints[0].getY();
		int x1Ctrl = ctrlPoints[1].getX(), y1Ctrl = ctrlPoints[1].getY();

		if (y1Ctrl > y0Ctrl)
			swap(y1Ctrl, y0Ctrl);

		int cx = x0Ctrl + (x1Ctrl - x0Ctrl)/2;
		int cy = y1Ctrl + (y0Ctrl - y1Ctrl)/2;
		int x, y, d, r;

		x = 0;
		r = y0Ctrl - cy;
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
		// determinar la distancia del click a la línea
		// si es mejor a un umbral (e.g. 3 píxeles) entonces
		// retornas true
		return false;
	}

	void onMove(int x, int y)
	{
	}

};