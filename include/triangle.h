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
		cout << "Se destruyo una linea" << endl;
	}

	void set(int x0, int y0, int x1, int y1, int x2, int y2)
	{
		points[0].set(x0, y0);
		points[1].set(x1, y1);
		points[2].set(x2, y2);
	}

	void update(int x2, int y2)
	{
		int x0 = points[0].getX();
		int y0 = points[0].getY();

		if (x2 >= x0)
		{
			points[1].set(x0 - (x2 - x0), y2);
			points[2].set(x2, y2);
		}
		else 
		{	
			points[1].set(x2, y2);
			points[2].set(x0 - (x2 - x0), y2);
		}	
	}

	void render()
	{
		setColor(fillColor[0], fillColor[1], fillColor[2]);

		glBegin(GL_TRIANGLES); {
			for (auto p : points)
				glVertex2i(p.getX(), p.getY());
		}
			
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