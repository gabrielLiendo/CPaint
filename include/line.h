#pragma once
#include "shape.h"

class CLine : public CShape
{
private:
	Point2D points[2];

public:
	CLine(float r, float g, float b) : CShape(r, g, b) {}

	~CLine()
	{
		cout << "Se destruyo una linea" << endl;
	}

	void update(int x1, int y1)
	{
		// Update bounding box corner
		this->x1 = x1; this->y1 = y1;
	}

	void render(const char* mode)
	{	
		setColor(fillColor[0], fillColor[1], fillColor[2]);

		if(mode=="Hardware")
		{
			//glEnable(GL_LINE_STIPPLE);
			//glLineStipple(3, 3);
			glBegin(GL_LINES);
				glVertex2i(x0, y0);
				glVertex2i(x1, y1);
			glEnd();
		}
		else 
		{	
			// Draw using Bresenham's algorithm
			int dx, dy, x, y, d, incE, incNE;

			dx = x1 - x0;
			dy = y1 - y0;
			d = (dy << 1) - dx;
			incE = dy << 1;
			incNE = (dy - dx) << 1;
			x = x0;
			y = y0;

			putPixel(x, y);

			while (x < x1)
			{
				if (d >= 0) {
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

	bool onClick(int x, int y) 
	{
		// determinar la distancia del click a la línea
		// si es mejor a un umbral (e.g. 3 píxeles) entonces
		// retornas true
		return false;
	}

	void onMove(int x1, int y1)
	{
	}

};