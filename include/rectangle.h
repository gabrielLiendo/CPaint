#pragma once
#include "shape.h"

class CRectangle : public CShape
{
public:
	CRectangle(float r, float g, float b) : CShape(r, g, b) {}

	~CRectangle()
	{
		cout << "Se destruyo un cuadrado" << endl;
	}

	void render(const char* mode)
	{
		glColor3f(fillColor[0], fillColor[1], fillColor[2]);

		glBegin(GL_QUADS);
			for (auto p : boxPoints)
				glVertex2i(p.x, p.y);
		glEnd();
	}

	bool onClick(int x, int y)
	{	
		
		cout << "CHECK RECTANGLE" << endl;
		cout << boxPoints[0].x << " " << boxPoints[0].y << endl;
		cout << boxPoints[1].x << " " << boxPoints[1].y << endl;
		cout << boxPoints[2].x << " " << boxPoints[2].y << endl;
		cout << boxPoints[3].x << " " << boxPoints[3].y << endl;

		cout << "PIXEL:" << x << " " << y << endl;
		if (boxPoints[0].x < x && x < boxPoints[2].x && boxPoints[2].y < y && y < boxPoints[0].y)
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
			boxPoints[i].x += dx;
			boxPoints[i].y += dy;
		}

		refPoint.x = x1; refPoint.y = y1;
	}
};