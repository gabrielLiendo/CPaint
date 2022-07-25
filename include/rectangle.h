#pragma once
#include "shape.h"

class CRectangle : public CShape
{
public:
	CRectangle(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2, bool filled)
		: CShape(r1, g1, b1, r2, g2, b2, filled, "RECTANGLE ") 
	{
		anchorPoint.x = x0; anchorPoint.y = y0;
		setBoundingBox(x0, y0, x1, y1);
	}

	~CRectangle(){ cout << "Se destruyo un cuadrado" << endl;}

	void update(int x, int y)
	{
		setBoundingBox(anchorPoint.x, anchorPoint.y, x, y);
	}
	
	// Render the bounding box
	void drawCtrlStructure()
	{
		renderBox();

		// Render vertex points
		for (int i = 0; i < 4; i++)
			boxPoints[i].renderCtrlPoint();
	}

	void render(const bool modeHardware)
	{
		if(modeHardware) // Hardware
		{
			if (filled)
			{	// Draw Filled
				glColor3f(fillColor.r, fillColor.g, fillColor.b);
				glBegin(GL_QUADS);
				for (int i = 0; i < 4; i++)
					glVertex2i(boxPoints[i].x, boxPoints[i].y);
				glEnd();
			}
			
			// Draw Border
			glColor3f(borderColor.r, borderColor.g, borderColor.b);
			glBegin(GL_LINE_LOOP);
				for (int i = 0; i < 4; i++)
					glVertex2i(boxPoints[i].x, boxPoints[i].y);
			glEnd();
		}
		else // Software
		{
			int xmin = boxPoints[0].x, ymin = boxPoints[0].y;
            int xmax = boxPoints[2].x, ymax = boxPoints[2].y;
			
			// Draw top and bottom borders
			horizontalLine(xmin, xmax, ymin, borderColor);
			horizontalLine(xmin, xmax, ymax, borderColor);

			// Draw middle lines, filled or empty
			if (filled)
			{	// Draw border and filler in the same iteration
				for (int y = ymin + 1; y < ymax; y++)
				{
					putPixel(xmin, y, borderColor);
					horizontalLine(xmin + 1, xmax - 1, y, fillColor);
					putPixel(xmax, y, borderColor);
				}
			}
			else
			{	// Draw border rest of the border
				for (int y = ymin + 1; y < ymax; y++)
				{
					putPixel(xmin, y, borderColor);
					putPixel(xmax, y, borderColor);
				}
			}
		}
	}

	// We check if the click fell inside the rectangle, threshold: 3 pixels
	bool onClick(int x, int y)
	{	
		return  (x > boxPoints[0].x - 3 && x < boxPoints[2].x + 3 && y > boxPoints[0].y - 3 && y < boxPoints[2].y + 3);
	}

	// We check if the click fell on a vertex
	bool hoveredCtrlPoint(int x, int y)
	{
		int dx, dy;
		for (int i = 0; i < 4; i++)
		{
			dx = (x - boxPoints[i].x);
			dy = (y - boxPoints[i].y);
			// Check squared distance between vertex i and the click, threshold: 4 pixels
			if ((dx * dx + dy * dy) <= 16)
			{
				pointHovered = &boxPoints[i];
				indexSelected = i;
				return true;
			}
		}

		return false;
	}

	void onMove(int x, int y)
	{	
		if (pointSelected)
		{
			int i = indexSelected;
			int op = (i + 2) % 4;

			xResize(i, op, x);
			yResize(i, op, y);
		}
		else
		{	// We move the whole rectangle
			int dx = x - anchorPoint.x;
			int dy = y - anchorPoint.y;

			anchorPoint.x = x; anchorPoint.y = y;
			moveBoundingBox(dx, dy);
		}
	}
};