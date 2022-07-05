#pragma once
#include "shape.h"
#include "boxableShape.h"

class CRectangle : public CShape, public BoxableShape
{
public:
	CRectangle(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2)
		: CShape(x0, y0, r1, g1, b1, r2, g2, b2), BoxableShape(x0, y0, x1, y1) {}

	~CRectangle(){ cout << "Se destruyo un cuadrado" << endl;}

	void update(int x1, int y1)
	{
		setBoundingBox(anchorPoint.x, anchorPoint.y, x1, y1);
	}
	
	// Render the bounding box
	void renderCtrlPoints()
	{
		renderBox();
	}

	void render(const bool modeHardware)
	{
		if(modeHardware) // Hardware
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
		else // Software
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
	}

	bool onClick(int x, int y)
	{	
		// We check if the click fell inside the rectangle, threshold: 3 pixels
		return (x > boxPoints[0].x - 3 && x < boxPoints[2].x + 3 && y > boxPoints[2].y -3 && y < boxPoints[0].y + 3);
	}

	void clickedCtrlPoint(int x, int y)
	{
		// We check if the click fell on a vertex
		int dx, dy;
		for (int i = 0; i < 4; i++)
		{
			dx = (x - boxPoints[i].x);
			dy = (y - boxPoints[i].y);
			// Check squared distance between vertex i and the click, threshold: 4 pixels
			if ((dx * dx + dy * dy) <= 16)
			{
				pointSelected = &boxPoints[i];
				iPointSelected = i;
				return;
			}
		}
	}

	void onMove(int x1, int y1)
	{	
		if (pointSelected)
			resizeBoundingBox(x1, y1);
		else {
			// We move the whole rectangle
			int dx = x1 - anchorPoint.x;
			int dy = y1 - anchorPoint.y;

			anchorPoint.x = x1;
			anchorPoint.y = y1;

			moveBoundingBox(dx, dy);
		}
	}
};