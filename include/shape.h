#pragma once
#include <iostream>

using namespace std;

struct Point2D
{
	int x; 
	int y;
};

class CShape
{
protected:
	//float borderColor[3];
	int x0, x1, y0, y1;
	float fillColor[3];
	Point2D boxPoints[4];
	Point2D refPoint;

public:
	CShape(float r, float g, float b)
	{	
		fillColor[0] = r; fillColor[1] = g; fillColor[2] = b;
	}

	// Set bounding box corner points
	void set(int x0, int y0, int x1, int y1)
	{
		if (x1 < x0)
			swap(x0, x1);
		if (y1 < y0)
			swap(y1, y0);

		boxPoints[0].x = x0; boxPoints[0].y = y1;
		boxPoints[1].x = x0; boxPoints[1].y = y0;
		boxPoints[2].x = x1; boxPoints[2].y = y0;
		boxPoints[3].x = x1; boxPoints[3].y = y1;
	}

	void setRefPoint(int x, int y)
	{
		refPoint.x = x;
		refPoint.y = y;
	}

	virtual ~CShape()
	{
		cout << "Se destruyo un shape" << endl;
	}

	void putPixel(int x, int y)
	{	
		glBegin(GL_POINTS);
			glVertex2i(x, y);
		glEnd();
	}

	void setColor(float r, float g, float b)
	{
		fillColor[0] = r; fillColor[1] = g; fillColor[2] = b;
	}

	virtual void render(const char* mode) = 0;

	// recibe el click del mouse y retorna true si efectivamente
	// el objetos fue seleccionado
	virtual bool onClick(int x, int y) = 0;
	
	// este método es invocado solo hacia el objeto "current"
	virtual void onMove(int x1, int y1) = 0;

	// podríamos responder a los eventos del mouse
	// . todos responden al click, pero solo uno puede
	// . retornar "yo fui seleccionado"
};
