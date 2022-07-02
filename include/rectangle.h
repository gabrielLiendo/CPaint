#pragma once
#include "shape.h"

class CRectangle : public CShape, public BoxableShape
{
public:
	CRectangle(int x0, int y0, int x1, int y1, float r, float g, float b)
		: CShape(r, g, b), BoxableShape(x0, y0, x1, y1) {}

	~CRectangle()
	{
		cout << "Se destruyo un cuadrado" << endl;
	}

	void update(int x1, int y1)
	{
		setBoundingBox(x1, y1);
	}

	void setRefPoint(int x, int y)
	{
		BoxableShape::setRefPoint(x, y);
	}

	void render(const char* mode)
	{
		glColor3f(fillColor[0], fillColor[1], fillColor[2]);

		glBegin(GL_QUADS);
			for (auto p : boxPoints)
				glVertex2i(p.x, p.y);
		glEnd();

		if (selected)
			renderBox();
	}

	bool onClick(int x, int y)
	{	
		return (x > boxPoints[0].x && x < boxPoints[2].x && y > boxPoints[2].y && y < boxPoints[0].y);
	}

	void onMove(int x1, int y1)
	{	
		moveBoundingBox(x1, y1);
	}
};