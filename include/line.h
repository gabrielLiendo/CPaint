#pragma once
#include "shape.h"

class CLine : public CShape
{
private:
	Point points[2];
	
public:
	CLine(int x0, int y0, int x1, int y1, float r, float g, float b): CShape(r, g, b,"LINE ")
	{
		points[0] = Point(x0, y0);
		points[1] = Point(x1, y1);
	}

	~CLine(){ cout << "Se destruyo una linea" << endl; }

	// Update end point when drawing
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
			glColor3f(borderColor.r, borderColor.g, borderColor.b);
			glBegin(GL_LINES);
				glVertex2i(points[0].x, points[0].y);
				glVertex2i(points[1].x, points[1].y);
			glEnd();
		}
		else 
			drawLine(points[0].x, points[0].y, points[1].x, points[1].y, borderColor);
	}

	// We check if the click fell close to the line, threshold: 4 pixels
	bool onClick(int x, int y) 
	{
		int x0 = points[0].x, y0 = points[0].y;
		int x1 = points[1].x, y1 = points[1].y;

		if ((x1 > x0 && (x > x1 + 3 || x < x0 - 3)) || (x1 <= x0 && (x > x0 + 3 || x < x1 - 3)))
			return false;
			
		int a = y0 - y1, b = x1 - x0,  c = x0 * y1 - x1 * y0;
		int distance = static_cast<int>(abs(a * x + b * y + c) / sqrt(a * a + b * b));

		if (distance <= 4)
		{
			clickedCtrlPoint(x, y);
			return true;
		}
		return false;
	}

	// We check if the click fell on a vertex
	void clickedCtrlPoint(int x, int y)
	{
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

	void onMove(int x, int y)
	{
		if (pointSelected)
		{	// Only move the vertex selected
			pointSelected->x = x;
			pointSelected->y = y;
		}
		else
		{	// Move the whole line
			int dx = x - anchorPoint.x;
			int dy = y - anchorPoint.y;

			points[0].x += dx; points[0].y += dy;
			points[1].x += dx; points[1].y += dy;

			anchorPoint.x = x; anchorPoint.y = y;
		}
	}

	// Form string with position and border color
	std::string getInfo() override
	{
		return info + to_string(points[0].x) + " " + to_string(points[0].y) + " " + to_string(points[1].x) + " " + to_string(points[1].y) + " "
			+ to_string(borderColor.r) + " " + to_string(borderColor.g) + " " + to_string(borderColor.b) + "\n";
	}
};