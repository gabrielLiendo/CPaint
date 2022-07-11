#pragma once
#include "shape.h"

class CLine : public CShape
{
private:
	CtrlPoint points[2];
	
public:
	CLine(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2, bool filled)
		: CShape(x0, y0, r1, g1, b1, r2, g2, b2, filled)
	{
		points[0] = CtrlPoint(x0, y0);
		points[1] = CtrlPoint(x1, y1);
	}

	~CLine(){ cout << "Se destruyo una linea" << endl; }

	void update(int x1, int y1)
	{
		points[1].x = x1; 
		points[1].y = y1;
	}

	// Render each vertex
	void renderCtrlPoints()
	{
		points[0].renderCtrlPoint();
		points[1].renderCtrlPoint();
	}

	void render(const bool hardwareMode)
	{	
		if(hardwareMode)
		{
			glColor3f(fillColor.r, fillColor.g, fillColor.b);
			glBegin(GL_LINES);
				glVertex2i(points[0].x, points[0].y);
				glVertex2i(points[1].x, points[1].y);
			glEnd();
		}
		else 
			drawLine(points[0].x, points[0].y, points[1].x, points[1].y, fillColor);
	}

	bool onClick(int x, int y) 
	{
		// We check if the click fell close to the line, threshold: 4 pixels
		int x0 = points[0].x, y0 = points[0].y;
		int x1 = points[1].x, y1 = points[1].y;

		int a = y0 - y1;
		int b = x1 - x0;
		int c = x0 * y1 - x1 * y0;
		int distance = (int) abs(a * x + b * y + c) / sqrt(a * a + b * b);

		return distance <= 4;
	}

	void clickedCtrlPoint(int x, int y)
	{
		// We check if the click fell on a vertex
		int dx, dy;
		for (int i = 0; i < 2; i++)
		{
			dx = (x - points[i].x);
			dy = (y - points[i].y);
			// Check squared distance between vertex i and the click
			if ((dx * dx + dy * dy) <= 16)
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
		{	// Move the whole line
			int dx = x1 - anchorPoint.x;
			int dy = y1 - anchorPoint.y;

			points[0].x += dx; points[0].y += dy;
			points[1].x += dx; points[1].y += dy;

			anchorPoint.x = x1; anchorPoint.y = y1;
		}
	}

	std::string getInfo()
	{
		string info = "LINE ";

		// Add position
		info += to_string(points[0].x) + " " + to_string(points[0].y) + " "
			+ to_string(points[1].x) + " " + to_string(points[1].y) + " ";

		info += to_string(fillColor.r) + " " + to_string(fillColor.g) + " " + to_string(fillColor.b);

		info += "\n";
	
		return info;
	}
};