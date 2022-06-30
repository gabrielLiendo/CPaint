#pragma once
#include "point2D.h"
#include "shape.h"

class CLine : public CShape
{
public:
	CLine(float r, float g, float b) : CShape(r, g, b) {}

	~CLine()
	{
		cout << "Se destruyo una linea" << endl;
	}

	void update(int x2, int y2)
	{
		ctrlPoints[1].set(x2, y2);
	}

	void render(const char* mode)
	{	
		setColor(fillColor[0], fillColor[1], fillColor[2]);

		if(mode=="Hardware")
		{
			//glEnable(GL_LINE_STIPPLE);
			//glLineStipple(3, 3);
			glBegin(GL_LINES);
			for (auto p : ctrlPoints)
				glVertex2i(p.getX(), p.getY());
			glEnd();
		}
		else 
		{	
			int x0 = ctrlPoints[0].getX(), y0 = ctrlPoints[0].getY();
			int x1 = ctrlPoints[1].getX(), y1 = ctrlPoints[1].getY();

			// Draw using Bresenham's algorithm
			int dx, dy, x, y, d, incE, incNE;

			dx = x1 - x0;
			dy = y1 - y0;
			d = dx - (dy << 1);
			incE = dy << 1;
			incNE = (dx - dy) << 1;
			x = x0;
			y = y0;

			putPixel(x, y);

			if (dx > dy && dy >= 0) {
				while (x < x1)
				{
					if (d <= 0) {
						d += incNE;
						y += 1;
					}
					else
						d += incE;
					x += 1;
					putPixel(x, y);
				}
			}
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