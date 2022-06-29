#pragma once
#include "point2D.h"
#include "shape.h"

class CTriangle : public CShape
{
private:	
	Point2D points[3];

public:
	CTriangle(float r, float g, float b) : CShape(r, g, b){}

	~CTriangle()
	{
		cout << "Se destruyo un triangulo" << endl;
	}

	void update(int x1Ctrl, int y1Ctrl)
	{	
		ctrlPoints[1].set(x1Ctrl, y1Ctrl);

		int x0Ctrl = ctrlPoints[0].getX(), y0Ctrl = ctrlPoints[0].getY();
		int xMiddle = x0Ctrl + (x1Ctrl - x0Ctrl) / 2;

		if (y1Ctrl > y0Ctrl)
			swap(y1Ctrl, y0Ctrl);

		points[0].set(xMiddle, y0Ctrl);
		points[1].set(x0Ctrl, y1Ctrl);
		points[2].set(x1Ctrl, y1Ctrl);
	}

	void render()
	{	
		setColor(fillColor[0], fillColor[1], fillColor[2]);

		glBegin(GL_TRIANGLES); 
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