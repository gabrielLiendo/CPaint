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

	void update(int x1, int y1)
	{
		ctrlPoints[1].set(x1, y1);

		int x0 = ctrlPoints[0].getX(), y0 = ctrlPoints[0].getY();
		
		points[0].set(x0, y0);
		points[1].set(x0, y1);
		points[2].set(x1, y1);
		points[3].set(x1, y0);
	}

	void render(const char* mode)
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