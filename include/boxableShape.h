#pragma once

class BoxableShape
{
protected:
	CtrlPoint boxPoints[4];
	int iPointSelected = -1;

public:
	BoxableShape(int x0, int y0, int x1, int y1)
	{
		boxPoints[0] = CtrlPoint(x0, y1);
		boxPoints[1] = CtrlPoint(x0, y0);
		boxPoints[2] = CtrlPoint(x1, y0);
		boxPoints[3] = CtrlPoint(x1, y1);
	}

	// Set bounding box corner points
	void setBoundingBox(int x0, int y0, int x1, int y1)
	{
		if (x1 < x0)
			swap(x0, x1);
		if (y1 < y0)
			swap(y1, y0);

		boxPoints[0].x = x0; boxPoints[0].y = y1;
		boxPoints[1].x = x0; boxPoints[1].y = y0;
		boxPoints[2].x = x1; boxPoints[2].y = y0;
		boxPoints[3].x = x1; boxPoints[3].y = y1;
	}

	// Move the 4 points of the box by 'dx' and 'dy' respectably
	void moveBoundingBox(int dx, int dy)
	{
		for (int i = 0; i < 4; i++)
		{
			boxPoints[i].x += dx;
			boxPoints[i].y += dy;
		}
	}

	void resizeBoundingBox(int x, int y)
	{
		int i = iPointSelected;

		boxPoints[i].x = x; boxPoints[i].y = y;

		if (iPointSelected == 0) {
			boxPoints[3].y = y;
			boxPoints[1].x = x;
		}

		if (iPointSelected == 1) {
			boxPoints[2].y = y;
			boxPoints[0].x = x;
		}

		if (iPointSelected == 2) {
			boxPoints[1].y = y;
			boxPoints[3].x = x;
		}

		if (iPointSelected == 3) {
			boxPoints[0].y = y;
			boxPoints[2].x = x;
		}
			
	}

	void renderBox()
	{
		// Set color and style of the line
		glColor3f(1.0f, 1.0f, 1.0f);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
			for(int i=0; i < 4; i++)
				glVertex2i(boxPoints[i].x, boxPoints[i].y);
		glEnd();
		glDisable(GL_LINE_STIPPLE);

		// Render vertex points
		for (int i = 0; i < 4; i++)
			boxPoints[i].renderCtrlPoint();
	}
};