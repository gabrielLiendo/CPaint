#pragma once
#include "shape.h"

class CRectangle : public CShape
{
public:
	CRectangle(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2, bool filled)
		: CShape(x0, y0, r1, g1, b1, r2, g2, b2, filled){}

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
			int xmin = boxPoints[1].x, ymin = boxPoints[1].y;
            int xmax = boxPoints[3].x, ymax = boxPoints[3].y;
			
			// Draw top and bottom borders
			horizontalLine(xmin, xmax, ymin, borderColor);
			horizontalLine(xmin, xmax, ymax, borderColor);

			// Draw middle lines, filled or empty
			if (filled)
			{
				// Draw border and filler in the same iteration
				for (int y = ymin + 1; y < ymax; y++)
				{
					putPixel(xmin, y, borderColor);
					horizontalLine(xmin + 1, xmax - 1, y, fillColor);
					putPixel(xmax, y, borderColor);
				}
			}
			else
			{
				for (int y = ymin + 1; y < ymax; y++)
				{
					putPixel(xmin, y, borderColor);
					putPixel(xmax, y, borderColor);
				}
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
			// Check squared distance between vertex i and the click, threshold: 5 pixels
			if ((dx * dx + dy * dy) <= 25)
			{
				pointSelected = &boxPoints[i];
				return;
			}
		}
	}

	void onMove(int x1, int y1)
	{	
		// We move the whole rectangle
		int dx = x1 - anchorPoint.x;
		int dy = y1 - anchorPoint.y;

		anchorPoint.x = x1;
		anchorPoint.y = y1;

		moveBoundingBox(dx, dy);
	}

	std::string getInfo()
	{
		string info = "RECTANGLE ";

		// Add position info
		info += to_string(boxPoints[0].x) + " " + to_string(boxPoints[0].y) + " "
			 + to_string(boxPoints[2].x) + " " + to_string(boxPoints[2].y) + " ";
		
		// Add border info
		info += to_string(borderColor.r) + " " + to_string(borderColor.g) + " "
			+ to_string(borderColor.b);

		// Add filler info
		if (filled)
		{
			info = "FILLED_" + info + " " + to_string(fillColor.r) + " "
				+ to_string(fillColor.g) + " " + to_string(fillColor.b);
		}

		info += "\n";

		return info;
	}
};