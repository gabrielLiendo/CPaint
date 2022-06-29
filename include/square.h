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

	void update(int x1Ctrl, int y1Ctrl)
	{
		ctrlPoints[1].set(x1Ctrl, y1Ctrl);

		int x0Ctrl = ctrlPoints[0].getX(), y0Ctrl = ctrlPoints[0].getY();
		
		points[0].set(x0Ctrl, y0Ctrl);
		points[1].set(x0Ctrl, y1Ctrl);
		points[2].set(x1Ctrl, y1Ctrl);
		points[3].set(x1Ctrl, y0Ctrl);
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