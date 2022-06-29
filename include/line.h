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

	void render()
	{
		// despliegas la línea con el algoritmo de bresenham
		setColor(fillColor[0], fillColor[1], fillColor[2]);

		
		// user putpixel de aquí en adelante... con Bresenham
		//glEnable(GL_LINE_STIPPLE);
		//glLineStipple(3, 3);
		//glDisable(GL_LINE_STIPPLE);

		glBegin(GL_LINES);
			for (auto p : ctrlPoints)
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