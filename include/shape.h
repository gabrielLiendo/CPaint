#pragma once
#include <iostream>

using namespace std;

class CShape
{
protected:
	//float borderColor[3];
	float fillColor[3];

public:
	CShape(float r, float g, float b)
	{
		fillColor[0] = r;
		fillColor[1] = g;
		fillColor[2] = b;
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


	virtual void render() = 0;
	
	virtual void update(int x, int y) = 0;

	// recibe el click del mouse y retorna true si efectivamente
	// el objetos fue seleccionado
	virtual bool onClick(int x, int y) = 0;
	
	// este método es invocado solo hacia el objeto "current"
	virtual void onMove(int x, int y) = 0;

	// podríamos responder a los eventos del mouse
	// . todos responden al click, pero solo uno puede
	// . retornar "yo fui seleccionado"
};
