#pragma once
#include "line.h"
#include <algorithm>
#include <math.h>

class CTriangle : public CShape
{
private:	
	CtrlPoint points[3];
	CtrlPoint divide;
	int currentIndex = 1;

	float leftInc1 = 0, leftInc2 = 0, rightInc1 = 0, rightInc2 = 0;
	float ixLeft, ixRight, ixLeft2, ixRight2;

public:
	CTriangle(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2)
		: CShape(x0, y0, r1, g1, b1, r2, g2, b2)
	{
		points[0] = CtrlPoint(x0, y0);
		points[1] = CtrlPoint(x1, y1);
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

	void setPointsOrder()
	{
		sort(begin(points), end(points));
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

			// Draw Current Border
			glColor3f(borderColor.r, borderColor.g, borderColor.b);
			glBegin(GL_LINES);
				for (int i = 0; i < currentIndex; i++)
				{
					glVertex2i(points[i % 3].x, points[i % 3].y);
					glVertex2i(points[(i+1) % 3].x, points[(i+1) % 3].y);
				}
			glEnd();
		}
		else
		{	
			// Draw Content
			if (currentIndex == 3)
			{
				int ymin = points[0].y, ymid = points[1].y, ymax = points[2].y;

				ixLeft = (float)points[0].x, ixRight = (float)points[0].x;
				// Draw lower semi-triangle filler
				for (int y = ymin; y <= ymid; y++) 
				{
					hLine(ceil(ixLeft), floor(ixRight), y, fillColor);
					ixLeft += leftInc1;
					ixRight += rightInc1;
				}

				if(divide.x < points[1].x)
					ixLeft = divide.x, ixRight = points[1].x;
				else 
					ixLeft = points[1].x, ixRight = divide.x;

				// Draw upper semi-triangle filler
				for (int y = ymid + 1; y <= ymax; y++) 
				{
					ixLeft += leftInc2;
					ixRight += rightInc2;
					hLine(ceil(ixLeft), floor(ixRight), y, fillColor);
				}
			}

			// Draw Border
			for (int i = 0; i < currentIndex; i++)
				drawLine(points[i % 3].x, points[i % 3].y, points[(i + 1) % 3].x, points[(i + 1) % 3].y, borderColor);
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
		currentIndex ++;
		if (currentIndex != 3)
		{
			points[currentIndex].x = points[currentIndex-1].x;
			points[currentIndex].y = points[currentIndex-1].y;
			return false;
		}
		else
		{
			setPointsOrder();

			int xmin = points[0].x, ymin = points[0].y;
			int xmid = points[1].x, ymid = points[1].y;
			int xmax = points[2].x, ymax = points[2].y;

			divide.x = (int)(((float)(xmax - xmin) / (float)(ymax - ymin)*(ymid - ymax))+xmax);
			divide.y = points[1].y;

			if (divide.x > points[1].x)
			{
				leftInc1 = (float)(xmid - xmin) / (float)(ymid - ymin);
				leftInc2 = (float)(xmax - xmid) / (float)(ymax - ymid);
				rightInc1 = (float)(xmax - xmin) / (float)(ymax - ymin);
				rightInc2 = rightInc1;
			}
			else
			{
				rightInc1 = (float)(xmid - xmin) / (float)(ymid - ymin);
				rightInc2 = (float)(xmax - xmid) / (float)(ymax - ymid);
				leftInc1 = (float)(xmax - xmin) / (float)(ymax - ymin);
				leftInc2 = leftInc1;
			}
		
			return true;
		}
	}
};