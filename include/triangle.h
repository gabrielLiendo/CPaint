#pragma once
#include "line.h"
#include <algorithm>
#include <math.h>

class CTriangle : public CShape
{
private:	
	CtrlPoint points[4];
	int currentIndex = 1;
	float leftInc1 = 0, leftInc2 = 0, rightInc1 = 0, rightInc2 = 0;

public:
	CTriangle(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2, bool filled)
		: CShape(x0, y0, r1, g1, b1, r2, g2, b2, filled)
	{
		points[0] = CtrlPoint(x0, y0);
		points[1] = CtrlPoint(x1, y1);
	}

	CTriangle(int x0, int y0, int x1, int y1, int x2, int y2, float r1, float g1, float b1, float r2, float g2, float b2, bool filled)
		: CShape(x0, y0, r1, g1, b1, r2, g2, b2, filled)
	{
		points[0] = CtrlPoint(x0, y0);
		points[1] = CtrlPoint(x1, y1);
		points[2] = CtrlPoint(x2, y2);

		currentIndex = 3;
		setRenderValues();
	}

	~CTriangle(){ cout << "Se destruyo un triangulo" << endl;}

	void update(int x1, int y1)
	{	
		// Update current vertex positions
		points[currentIndex].x = x1; 
		points[currentIndex].y = y1;
	}

	void renderCtrlPoints()
	{
		// Render each vertex
		for (int i = 0; i < 3; i++)
			points[i].renderCtrlPoint();
	}


	void setRenderValues()
	{
		// Order points by y-axis, p[0] is the lowest, p[2] is the highest
		if (points[1].y < points[0].y) { swap(points[1], points[0]); }
		if (points[2].y < points[0].y) { swap(points[2], points[0]); }
		if (points[2].y < points[1].y) { swap(points[2], points[1]); }

		int xmin = points[0].x, ymin = points[0].y;
		int xmid = points[1].x, ymid = points[1].y;
		int xmax = points[2].x, ymax = points[2].y;

		// Set values of the point that separates one side into two lines 
		points[3].x = (int)(((float)(xmax - xmin) / (float)(ymax - ymin) * (ymid - ymax)) + xmax);
		points[3].y = points[1].y;

		if (points[3].x > points[1].x)
		{	// The left side of the triangle has two slopes, the right side has one
			leftInc1 = (float)(xmid - xmin) / (float)(ymid - ymin);
			leftInc2 = (float)(xmax - xmid) / (float)(ymax - ymid);
			rightInc1 = (float)(xmax - xmin) / (float)(ymax - ymin);
			rightInc2 = rightInc1;
		}
		else
		{	// The right side of the triangle has two slopes, the left side has one
			rightInc1 = (float)(xmid - xmin) / (float)(ymid - ymin);
			rightInc2 = (float)(xmax - xmid) / (float)(ymax - ymid);
			leftInc1 = (float)(xmax - xmin) / (float)(ymax - ymin);
			leftInc2 = leftInc1;
		}
	}

	void render(const bool modeHardware)
	{	
		if (modeHardware)
		{
			if (currentIndex == 3 && filled)
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
			if (currentIndex == 3 && filled)
			{
				float ixLeft = (float)points[0].x, ixRight = (float)points[0].x;

				// Draw lower semi-triangle filler
				for (int y = points[0].y +1; y <= points[1].y; y++)
				{
					ixLeft += leftInc1;
					ixRight += rightInc1;
					horizontalLine(ceil(ixLeft), floor(ixRight), y, fillColor);
				}

				// Draw upper semi-triangle filler
				for (int y = points[1].y + 1; y < points[2].y; y++)
				{
					ixLeft += leftInc2;
					ixRight += rightInc2;
					horizontalLine(ceil(ixLeft), floor(ixRight), y, fillColor);
				}
			}
			// Draw Border
			for (int i = 0; i < currentIndex; i++)
				drawLine(points[i % 3].x, points[i % 3].y, points[(i + 1) % 3].x, points[(i + 1) % 3].y, borderColor);
		}
	}

	bool onClick(int x, int y)
	{
		int y1, y2, y3;
		int xmin = points[0].x, ymin = points[0].y;
		int xmid = points[1].x, ymid = points[1].y;
		int xmax = points[2].x, ymax = points[2].y;

		y1 = (int)(((float)(xmid - xmin) / (float)(ymid - ymin) * (y - ymid)) + (xmid - x));
		y2 = (int)(((float)(xmax - xmid) / (float)(ymax - ymid) * (y - ymax)) + (xmax - x));
		y3 = (int)(((float)(xmin - xmax) / (float)(ymin - ymax) * (y - ymin)) + (xmin - x));

		return (y1 > 0 && y2 > 0 && y3 < 0) || (y1 < 0 && y2 < 0 && y3 > 0);
	}

	void clickedCtrlPoint(int x, int y)
	{
		// We check if the click fell on a vertex
		int dx, dy;
		for (int i = 0; i < 3; i++)
		{
			dx = (x - points[i].x);
			dy = (y - points[i].y);
			// Check squared distance between vertex i and the click, threshold: 5 pixels
			if ((dx * dx + dy * dy) <= 25)
			{
				pointSelected = &points[i];
				return;
			}
		}
	}

	void onMove(int x1, int y1)
	{

		if (pointSelected)
		{	// Only move the vertex selected
			pointSelected->x = x1;
			pointSelected->y = y1;
		}
		else
		{
			// We move the whole triangle
			int dx = x1 - anchorPoint.x;
			int dy = y1 - anchorPoint.y;

			anchorPoint.x = x1;
			anchorPoint.y = y1;

			for (int i = 0; i < 4; i++)
			{
				points[i].x += dx;
				points[i].y += dy;
			}
		}
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
			setRenderValues();
			return true;
		}
	}

	std::string getInfo()
	{
		string info = "TRIANGLE ";

		// Add position
		info += to_string(points[0].x) + " " + to_string(points[0].y) + " "
			 + to_string(points[1].x) + " " + to_string(points[1].y) + " "
			 + to_string(points[2].x) + " " + to_string(points[2].y) + " ";

		// Add border info
		info += to_string(borderColor.r) + " " + to_string(borderColor.g) + " "
			+ to_string(borderColor.b);

		// Add filler info
		if (filled)
		{
			info = "FILLED_" + info + " " + to_string(fillColor.r) + " "
				+ to_string(fillColor.g) + " " + to_string(fillColor.b);
		}

		info += "\n";

		return info;
	}
};