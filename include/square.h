#pragma once
#include "point2D.h"
#include "shape.h"

class CSquare : public CShape
{
private:
	Point2D points[4];

public:
	CSquare(float r, float g, float b) : CShape(r, g, b) {}

	~CSquare()
	{
		cout << "Se destruyo un cuadrado" << endl;
	}

	void set(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
	{
		points[0].set(x0, y0);
		points[1].set(x1, y1);
		points[2].set(x2, y2);
		points[3].set(x3, y3);
	}

	void update(int x3, int y3)
	{	
		int x0 = points[0].getX();
		int y0 = points[0].getY();

		points[1].set(x0, y3);
		points[2].set(x3, y3);
		points[3].set(x3, y0);
	}

	void render()
	{
		setColor(fillColor[0], fillColor[1], fillColor[2]);

		glBegin(GL_QUADS);
			for (auto p : points)
				glVertex2i(p.getX(), p.getY());
		glEnd();
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