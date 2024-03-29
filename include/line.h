#pragma once
#include "shape.h"

class CLine : public CShape
{
private:
	Point2D points[2];
	
public:
	CLine(int x0, int y0, int x1, int y1, float r, float g, float b): CShape(r, g, b,"LINE ")
	{
		points[0] = Point2D(x0, y0);
		points[1] = Point2D(x1, y1);
	}

	~CLine(){ cout << "Se destruyo una linea" << endl; }

	// Update end point when drawing
	void update(int x1, int y1)
	{
		points[1].x = x1; 
		points[1].y = y1;
	}

	// Render each vertex
	void drawCtrlStructure()
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
		int x0 = points[0].x, x1 = points[1].x;
		int y0 = points[0].y, y1 = points[1].y;
		if (x0 > x1)
		{
			x0 = points[1].x;
			x1 = points[0].x;
		}
		if (y0 > y1)
		{
			y0 = points[1].y;
			y1 = points[0].y;
		}

		if (x < x0 - 3 || x > x1 + 3 || y < y0 - 3 || y > y1 + 3) 
			return false;
		

		int a = points[0].y - points[1].y;
		int b = points[1].x - points[0].x;
		int c = points[0].x * points[1].y - points[1].x * points[0].y;

		return (int)abs(a * x + b * y + c) / sqrt(a * a + b * b) <= 4;

		
		/* Other method I tried for this was squaring the distance like so:

			double num = (a * x + b * y + c);
			int distance = static_cast<int>(num / (double)(a * a + b * b) * num);
			return distance > 0 && distance <= 16;

		  Getting rid of the calls to the functions abs() and sqrt(), and it worked,
		  buuut sometimes I got negative distances and the values got really big, 
		  really fast. So I went with the boring and slower, but safer route. */
		
	}

	// We check if the click fell on a vertex
	bool hoveredCtrlPoint(int x, int y)
	{
		int dx, dy;
		for (int i = 0; i < 2; i++)
		{
			dx = (x - points[i].x);
			dy = (y - points[i].y);
			// Check squared distance between vertex i and the click, threshold: 4 pixels
			if ((dx * dx + dy * dy) <= 16)
			{
				pointHovered = &points[i];
				return true;
			}
		}

		return false;
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
		std::string info = name;

		return info + to_string(points[0].x) + " " + to_string(points[0].y) + " " + to_string(points[1].x) + " " + to_string(points[1].y) + " "
			+ to_string(borderColor.r) + " " + to_string(borderColor.g) + " " + to_string(borderColor.b) + "\n";
	}
};