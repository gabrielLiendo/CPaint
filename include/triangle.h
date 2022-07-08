#pragma once
#include "line.h"

class CTriangle : public CShape
{
private:	
	CtrlPoint points[3];
	int currentIndex = 0;

public:
	CTriangle(int x0, int y0, float r1, float g1, float b1, float r2, float g2, float b2)
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
		// Render each vertex
		for (int i = 0; i < 3; i++)
			points[i].renderCtrlPoint();
	}

	void render(const bool modeHardware)
	{	
		if (modeHardware)
		{
			if (currentIndex == 3)
			{
				// Draw Content
				glColor3f(fillColor.r, fillColor.g, fillColor.b);
				glBegin(GL_TRIANGLES);
				for (int i = 0; i < 3; i++)
					glVertex2i(points[i].x, points[i].y);
				glEnd();
			}

			// Draw Border
			glColor3f(borderColor.r, borderColor.g, borderColor.b);
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < currentIndex; i++)
				glVertex2i(points[i].x, points[i].y);
			glEnd();
		}
		else
		{
			// Draw Border
			if (currentIndex == 2)
			{
				drawLine(points[0].x, points[0].y, points[1].x, points[1].y, borderColor);
				
			}
			else if (currentIndex == 3)
			{
				drawLine(points[0].x, points[0].y, points[1].x, points[1].y, borderColor);
				drawLine(points[1].x, points[1].y, points[2].x, points[2].y, borderColor);
				drawLine(points[2].x, points[2].y, points[0].x, points[0].y, borderColor);
			}
				
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