#pragma once
#include "shape.h"

class CSquare : public CShape
{
private:
	Point2D points[4];

public:
	CSquare(float r, float g, float b) : CShape(r, g, b) {}

	~CSquare()
	{
		cout << "Se destruyo un cuadrado" << endl;
	}

	void update(int x1, int y1)
	{
		// Update bounding box corner
		this->x1 = x1; this->y1 = y1;
		
		// Update vertex positions
		points[0].x = x0; points[0].y = y0;
		points[1].x = x0; points[1].y = y1;
		points[2].x = x1; points[2].y = y1;
		points[3].x = x1; points[3].y = y0;
	}

	void render(const char* mode)
	{
		setColor(fillColor[0], fillColor[1], fillColor[2]);

		glBegin(GL_QUADS);
			for (auto p : points)
				glVertex2i(p.x, p.y);
		glEnd();
	}

	bool onClick(int x, int y)
	{	
		cout << "CHECK RECTANGLE" << endl;
		cout << x0 << " " << x1 << " " << y0 << " " << y1 << endl;
		cout << x << " " << y << endl;
		if (x > x0 && x < x1 && y > y1 && y < y0)
		{
			cout << "clicked" << endl;
			return true;
		}
		return false;
	}

	void onMove(int x1, int y1)
	{	
		int dx = x1 - refPoint.x;
		int dy = y1 - refPoint.y;

		for (int i = 0; i < 4; i++)
		{
			points[i].x += dx;
			points[i].y += dy;
		}

		refPoint.x = x1; refPoint.y = y1;
	}
};