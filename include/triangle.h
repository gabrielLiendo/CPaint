#pragma once
#include "shape.h"

class CTriangle : public CShape
{
private:	
	Point2D points[3];

public:
	CTriangle(float r, float g, float b) : CShape(r, g, b){}

	~CTriangle()
	{
		cout << "Se destruyo un triangulo" << endl;
	}

	void update(int x1, int y1)
	{	
		// Update bounding box corner
		this->x1 = x1; this->y1 = y1;
		int xMiddle = x0 + ((x1 - x0) >> 1);


		// Update vertex positions
		points[0].x = xMiddle; points[0].y = y0;
		points[1].x = x0; points[1].y = y1;
		points[2].x = x1; points[2].y = y1;
	}

	void render(const char* mode)
	{	
		setColor(fillColor[0], fillColor[1], fillColor[2]);

		glBegin(GL_TRIANGLES); 
			for (auto p : points)
				glVertex2i(p.x, p.y);
		glEnd();
	}

	bool onClick(int x, int y)
	{
		return false;
	}

	void onMove(int x1, int y1)
	{
	}
};