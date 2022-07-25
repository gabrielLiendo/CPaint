#pragma once
#include "shape.h"

class CBezier : public CShape
{
private:
	vector<Point> ctrlPoints;
	vector<Point> segmentsPoints;
	int n; // number of ctrlPoints
	int m = 0; // number of points for segments
	bool closed = false;

public:
	CBezier(int x, int y, float r1, float g1, float b1, float r2, float g2, float b2)
		: CShape(r1, g1, b1, r2, g2, b2, true, "BEZIER")
	{
		ctrlPoints.push_back({ x, y });
		ctrlPoints.push_back({ x, y });
		n = 2;
	}

	CBezier(int *points, int n, float r, float g, float b)
		: CShape(r, g, b, r, g, b, true, "BEZIER")
	{
		for (int i=0; i < n; i++)
			ctrlPoints.push_back({ points[i*2], points[(i*2)+1]});

		closed = true;
		this->n = n;
		setRenderValues();
	}

	~CBezier() { 
		ctrlPoints.clear();
		ctrlPoints.shrink_to_fit();
		segmentsPoints.clear();
		segmentsPoints.shrink_to_fit();
		cout << "Se destruyo una curva de bezier" << endl; 
	}

	// Add new point to bezier curve
	void newPoint(int x, int y) override
	{
		ctrlPoints.push_back({ x, y });
		n++;
	}

	// Update last point value
	void update(int x, int y)
	{
		ctrlPoints.back() = { x, y };
		segmentsPoints.clear();
		setRenderValues();
	}

	void forceFinish(int x, int y) override
	{ 
		ctrlPoints.back() = { x, y };
		closed = true;
		segmentsPoints.clear();
		setRenderValues();
	}

	// Render control structure
	void drawCtrlStructure()
	{
		drawCtrlPolygon();
		renderBox();

		// Render each control points
		for (int i = 0; i < n; i++)
			ctrlPoints[i].renderCtrlPoint();
	}

	void setRenderValues()
	{
		// set bounding box values
		int minX = ctrlPoints[0].x, minY = ctrlPoints[0].y;
		int maxX = ctrlPoints[0].x, maxY = ctrlPoints[0].y;

		for (int i = 1; i < n; i++)
		{
			if (ctrlPoints[i].x < minX)
				minX = ctrlPoints[i].x;
			else if(ctrlPoints[i].x > maxX)
				maxX = ctrlPoints[i].x;

			if (ctrlPoints[i].y < minY)
				minY = ctrlPoints[i].y;
			else if (ctrlPoints[i].y > maxY)
				maxY = ctrlPoints[i].y;
		}

		boxPoints[0].x = minX; boxPoints[0].y = minY;
		boxPoints[1].x = minX; boxPoints[1].y = maxY;
		boxPoints[2].x = maxX; boxPoints[2].y = maxY;
		boxPoints[3].x = maxX; boxPoints[3].y = minY;

		// set segment points values
		int area = (maxX - minX) * (maxY - minY);
		/*  This step 'formula' was created by me, and it's not very good, sorry.
			I set up a minimun amount of 16 segments and tried to give a 'weight'
			to the number of control points and the dimension of the bounding box
			to contribute to the number of segments created
		*/
		double step = (double)1.0 / (double)(10 + 3*n);
		cout << area << " " << step << " " << area/5000.0 << " " << 1 / step << endl;
		for (double t = 0; t <= 1; t += step)
			segmentsPoints.push_back(bezierPoint(ctrlPoints, n, t));
			
		m = segmentsPoints.size();
	}

	Point bezierPoint(vector<Point> ctrlPoints, int n, double t)
	{
		while (n > 1)
		{
			n--;
			for (int i = 0; i < n; i++)
			{
				ctrlPoints[i].x = ctrlPoints[i].x + t * ctrlPoints[i + 1].x - t*ctrlPoints[i].x;
				ctrlPoints[i].y = ctrlPoints[i].y + t * ctrlPoints[i + 1].y - t*ctrlPoints[i].y;
			}
		}
		return ctrlPoints[0];
	}

	// Draw the line strip that forms with the control points
	void drawCtrlPolygon()
	{
		for (int i = 1; i < n; i++)
			drawLine(ctrlPoints[i - 1].x, ctrlPoints[i - 1].y, ctrlPoints[i].x, ctrlPoints[i].y, Color({ 0.0, 0.0, 0.0 }));
	}

	/* In this render function we only draw the segments, computing the segment points values in this function 
		would be a cold and brutal murder attempt of our poor CPU*/
	void render(const bool modeHardware)
	{
		if(!closed)
			drawCtrlPolygon();

		if (modeHardware)
		{
			glColor3f(borderColor.r, borderColor.g, borderColor.b);
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < m; i++)
				glVertex2i(segmentsPoints[i].x, segmentsPoints[i].y);
			glEnd();
		}
		else
		{	
			for (int i = 1; i < m; i++)
				drawLine(segmentsPoints[i - 1].x, segmentsPoints[i - 1].y, segmentsPoints[i].x, segmentsPoints[i].y, borderColor);
		}
	}

	bool onClick(int x, int y)
	{	
		// If the clicked fell inside the bounding box
		if (x > boxPoints[0].x - 3 && x < boxPoints[2].x + 3 && y > boxPoints[0].y - 3 && y < boxPoints[2].y + 3)
		{
			// We check if the click fell close to one of the segments, threshold: 6 pixels
			int x0, y0, x1, y1, dy, dx, c;

			for (int i = 1; i < m; i++)
			{
				x0 = segmentsPoints[i - 1].x, y0 = segmentsPoints[i - 1].y;
				x1 = segmentsPoints[i].x, y1 = segmentsPoints[i].y;

				if ((x1 > x0 && (x > x1 || x < x0)) || (x1 <= x0 && (x > x0 || x < x1)))
					continue;

				dy = y0 - y1;
				dx = x1 - x0;
				c = x0 * y1 - x1 * y0;

				if ((int)abs(dy * x + dx * y + c) / sqrt(dy * dy + dx * dx) <= 6) 	// Again this terrible formula...
					return true;
			}
		}
		return false;
	}

	bool hoveredCtrlPoint(int x, int y)
	{
		// We check if the click fell on a vertex
		int dx, dy;
		for (int i = 0; i < n; i++)
		{
			dx = (x - ctrlPoints[i].x);
			dy = (y - ctrlPoints[i].y);
			// Check squared distance between vertex i and the click
			if ((dx * dx + dy * dy) <= 25)
			{
				pointHovered = &ctrlPoints[i];
				return true;
			}
		}

		return false;
	}

	void onMove(int x1, int y1)
	{
		if (pointSelected)
		{	// Only move the vertex selected
			pointSelected->x = x1;
			pointSelected->y = y1;
			segmentsPoints.clear();
			setRenderValues();
		}
		else
		{	// We move the whole curve with it's control structure
			int dx = x1 - anchorPoint.x;
			int dy = y1 - anchorPoint.y;

			for (int i = 0; i < n; i++)
			{
				ctrlPoints[i].x += dx;
				ctrlPoints[i].y += dy;
			}

			for (int i = 0; i < m; i++)
			{
				segmentsPoints[i].x += dx;
				segmentsPoints[i].y += dy;
			}

			moveBoundingBox(dx, dy);
			anchorPoint.x = x1; anchorPoint.y = y1;
		}
		
	}

	bool finished() override
	{
		return closed;
	}

	std::string getInfo() override
	{
		std::string info = name;

		// Add number of ctrl points
		info += to_string(n) + " ";

		for (int i = 0; i < n; i++)
			info += to_string(ctrlPoints[i].x) + " " + to_string(ctrlPoints[i].y) + " ";

		// Add border info
		info += to_string(borderColor.r) + " " + to_string(borderColor.g) + " "
			+ to_string(borderColor.b);

		return info + "\n";
	}
};