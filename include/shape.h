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
	Point2D refPoint;

public:
	CShape(float r, float g, float b)
	{	
		fillColor[0] = r; fillColor[1] = g; fillColor[2] = b;
	}

	void set(int x0, int y0, int x1, int y1)
	{
		// Set bounding box corner points
		this->x0 = x0; this->x1 = x1;
		this->y0 = y0; this->y1 = y1;
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
		glColor3f(r, g, b);
	}


	virtual void update(int x, int y) = 0;

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
