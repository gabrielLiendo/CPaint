#pragma once
#include "shape.h"

class CTriangle : public CShape
{
private:	
	CtrlPoint points[3];
	int currentIndex = 0;

public:
	CTriangle(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2)
		: CShape(x0, y0, r1, g1, b1, r2, g2, b2)
	{
		points[0] = CtrlPoint(x0, y0);
	}

	~CTriangle(){ cout << "Se destruyo un triangulo" << endl;}

	void update(int x1, int y1)
	{	
		// Update current vertex positions
		points[currentIndex].x = x1; points[currentIndex].y = y1;
	}

	void renderCtrlPoints()
	{

	}
	
	void render(const bool modeHardware)
	{	
		// Draw Border
		glColor3f(borderColor.r, borderColor.g, borderColor.b);
		glBegin(GL_LINE_LOOP);
			for (int i = 0; i < currentIndex; i++)
				glVertex2i(points[i].x, points[i].y);
		glEnd();

		if (currentIndex == 3)
		{
			// Draw Content
			glColor3f(fillColor.r, fillColor.g, fillColor.b);
			glBegin(GL_TRIANGLES);
			for (int i = 0; i < 3; i++)
				glVertex2i(points[i].x, points[i].y);
			glEnd();
		}
	}

	

	bool onClick(int x, int y)
	{
		return false;
	}

	void clickedCtrlPoint(int x, int y)
	{

	}

	void onMove(int x1, int y1)
	{
	}

	bool finished() override
	{
		currentIndex += 1;
		return currentIndex == 3;
	}
};