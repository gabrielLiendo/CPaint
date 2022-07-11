#pragma once
#include "shape.h"
#include "boxableShape.h"

class CEllipse : public CShape, public BoxableShape
{
private:
	int a; // Radius in x-axis
	int b; // Radius in y-axis
	int cx, cy; // Center of ellipse

public:
	CEllipse(int x0, int y0, int x1, int y1, float r1, float g1, float b1, float r2, float g2, float b2, bool filled)
		: CShape(x0, y0, r1, g1, b1, r2, g2, b2, filled), BoxableShape(x0, y0, x1, y1) 
	{
		update(x1, y1);
	}

	~CEllipse(){ cout << "Se destruyo un elipse" << endl; }

	void update(int x1, int y1)
	{
		int x0 = anchorPoint.x;
		int y0 = anchorPoint.y;

		if (y1 >= y0)
			swap(y0, y1);

		// Update radii values
		a = ((x1 - x0) >> 1);
		b = ((y0 - y1) >> 1);

		// Update the center of the ellipse
		cy = y1 + b;
		cx = x0 + a;

		// Update position of the bounding box
		setBoundingBox(cx + a, cy - b, cx - a, cy + b);
	}

	// Render the bounding box
	void renderCtrlPoints()
	{
		renderBox();
	}


	// Draw the 4 symmetrical points of the point (x,y)
	void ellipsePoints(int x, int y)
	{
		putPixel(cx + x, cy + y, borderColor);
		putPixel(cx - x, cy + y, borderColor);
		putPixel(cx + x, cy - y, borderColor);
		putPixel(cx - x, cy - y, borderColor);
	}

	void fillEllipse(int x, int y)
	{
		horizontalLine(cx - x, cx + x, cy + y, fillColor);
		horizontalLine(cx - x, cx + x, cy - y, fillColor);
	}

	// Draw border and filler in the same iteration
	void render(const bool mode)
	{
		int x, y, d;

		x = 0;
		y = b;

		int ap2, bp2;
		int  incE, incS, incSE, varE, varSE,  varS;
		ap2 = a * a; bp2 = b * b;
		

		// Initial values
		d = b * ((b - ap2) << 2) + ap2;
		incE = 12 * bp2;
		incSE = (3 * bp2 - (ap2 * (b - 1)) << 1) << 2;

		varE = bp2  << 3;
		varSE = (ap2 + bp2) << 3;
		varS = ap2 << 3;

		if (filled)
		{
			// Draw initial 4 points
			ellipsePoints(x, y);

			// Mode 1: Draw and fill while the tangent line to the point has slope [-1,0]
			while (((bp2 * (x + 1)) << 1) < ap2 * ((y << 1) - 1))
			{
				if (d < 0)
				{
					d += incE;	
					incSE += varE;
				}
				else
				{
					d += incSE;
					incSE += varSE;
					y -= 1;
					fillEllipse(x, y);
				}
				x += 1;
				incE += varE;
				ellipsePoints(x, y);
			}

			// Mode 2: Draw and fill while the tangent line to the point has slope (-inf,-1]
			incS = (ap2 * (3 - (y << 1))) << 2;
			incSE = ((bp2*(x+1)) << 3) + ((ap2*(3 - (y << 1))) << 2);
			d = bp2 * (((x * x + x) << 2) + 1) + ap2 * ((y * y - (y << 1) + 1 - bp2) << 2);
			while (y > 0)
			{
				if (d < 0)
				{
					d += incSE;
					incSE += varSE;
					x += 1;
				}
				else
				{
					d += incS;
					incSE += varS;
				}
				incS += varS;
				y -= 1;
				fillEllipse(x, y);
				ellipsePoints(x, y);
			}
		}
		else
		{
			// Draw initial 4 points
			ellipsePoints(x, y);

			// Mode 1: Draw and fill while the tangent line to the point has slope [-1,0]
			while (((bp2 * (x + 1)) << 1) < ap2 * ((y << 1) - 1))
			{
				if (d < 0)
				{
					d += incE;
					incSE += varE;
				}
				else
				{
					d += incSE;
					incSE += varSE;
					y -= 1;
				}
				incE += varE;
				x += 1;
				ellipsePoints(x, y);
			}

			// Mode 2: Draw and fill while the tangent line to the point has slope (-inf,-1]
			incS = (ap2 * (3 - (y << 1))) << 2;
			incSE = ((bp2 * (x + 1)) << 3) + ((ap2 * (3 - (y << 1))) << 2);
			d = bp2 * (((x * x + x) << 2) + 1) + ap2 * ((y * y - (y << 1) + 1 - bp2) << 2);

			while (y > 0)
			{
				if (d < 0)
				{
					d += incSE;
					incSE += varSE;
					x += 1;
				}
				else
				{
					d += incS;
					incSE += varS;
				}
				incS += varS;
				y -= 1;
				ellipsePoints(x, y);
			}
		}
	}

	bool onClick(int x, int y)
	{
		int dx = x - cx;
		int dy = y - cy;

		return (((float)(dx * dx)/(a * a)) + ((float)(dy * dy) / (b * b))) <= 1.2;
	}

	void clickedCtrlPoint(int x, int y)
	{
	}

	void onMove(int x1, int y1)
	{
		if (pointSelected)
		{
			//int dx = x1 - anchorPoint.x;
			//anchorPoint.x = x1;
			//anchorPoint.y = y1;
			//r = r + dx;

			//setBoundingBox(boxPoints[0].x - r, boxPoints[0].y + r, boxPoints[2].y + r, boxPoints[2].y - r);
		}
		else
		{	// Move the whole ellipse
			int dx = x1 - anchorPoint.x;
			int dy = y1 - anchorPoint.y;

			anchorPoint.x = x1;
			anchorPoint.y = y1;

			cx += dx; cy += dy;
			moveBoundingBox(dx, dy);
		}
	}

	std::string getInfo()
	{
		string info = "ELLIPSE ";

		// Add position
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