#pragma once
#include "shape.h"
#include "boxableShape.h"

class CRectangle : public CShape, public BoxableShape
{
public:
	CRectangle(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2)
		: CShape(r1, g1, b1, r2, g2, b2), BoxableShape(x0, y0, x1, y1) {}

	~CRectangle()
	{
		cout << "Se destruyo un cuadrado" << endl;
	}

	void update(int x1, int y1)
	{
		cout  << "UPDATING" << endl;
		setBoundingBox(x1, y1);
	}

	void release() override
	{
		selected = false;
		pointSelected = nullptr;
		iPointSelected = -1;
	}

	void setAnchorPoint(int x, int y)
	{
		BoxableShape::setAnchorPoint(x, y);
	}

	void render(const bool mode)
	{
		if(mode) // Hardware
		{
			// Draw Content
			glColor3f(fillColor.r, fillColor.g, fillColor.b);
			glBegin(GL_QUADS);
				for (int i=0; i < 4; i++)
					glVertex2i(boxPoints[i].x, boxPoints[i].y);
			glEnd();

			// Draw Border
			glColor3f(borderColor.r, borderColor.g, borderColor.b);
			glBegin(GL_LINE_LOOP);
				for (int i = 0; i < 4; i++)
					glVertex2i(boxPoints[i].x, boxPoints[i].y);
			glEnd();
		}
		else
		{
			int xmin = boxPoints[1].x, ymin = boxPoints[1].y;
            int xmax = boxPoints[3].x, ymax = boxPoints[3].y;
			 
			for(int x = xmin; x <= xmax; x++)
			{
				putPixel(x, ymin, borderColor);
				putPixel(x, ymax, borderColor);
			}

			for (int y = ymin+1; y < ymax; y++)
			{
				putPixel(xmin, y, borderColor);
				// Draw filler
				for (int x = xmin + 1; x < xmax; x++)
					putPixel(x, y, fillColor);
				putPixel(xmax, y, borderColor);
			}
		}
	
		if (selected)
			renderBox();
	}

	bool onClick(int x, int y)
	{	
		// We check if the click fell on a vertex
		int x0 = boxPoints[0].x, y0 = boxPoints[0].y;
		int x1 = boxPoints[1].x, y1 = boxPoints[1].y;

		for (int i = 0; i < 4; i++)
		{
			if (boxPoints[i].distance(x, y) <= 3)
			{
				pointSelected = &boxPoints[i];
				iPointSelected = i;
				return true;
			}
		}

		// We check if the click fell inside the rectangle
		return (x > boxPoints[0].x && x < boxPoints[2].x && y > boxPoints[2].y && y < boxPoints[0].y);
	}

	void onMove(int x1, int y1)
	{	
		if (pointSelected)
			resizeBoundingBox(x1, y1);
		else
			moveBoundingBox(x1, y1);
	}
};