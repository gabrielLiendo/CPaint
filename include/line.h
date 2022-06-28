#pragma once
#include "point2D.h"
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

	void set(int x0, int y0, int x1, int y1)
	{
		points[0].set(x0, y0);
		points[1].set(x1, y1);
	}

	void update(int x1, int y1)
	{
		points[1].set(x1, y1);
	}

	void render()
	{
		// despliegas la línea con el algoritmo de bresenham
		setColor(fillColor[0], fillColor[1], fillColor[2]);

		// user putpixel de aquí en adelante... con Bresenham
		glBegin(GL_LINES);
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
