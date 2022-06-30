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

	void update(int x1, int y1)
	{	
		ctrlPoints[1].set(x1, y1);

		int x0 = ctrlPoints[0].getX(), y0 = ctrlPoints[0].getY();
		int xMiddle = x0 + ((x1 - x0) >> 1);

		if (y1 > y0)
			swap(y1, y0);

		points[0].set(xMiddle, y0);
		points[1].set(x0, y1);
		points[2].set(x1, y1);
	}

	void render(const char* mode)
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