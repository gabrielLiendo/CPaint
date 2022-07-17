#pragma once
#include "shape.h"

class CBezier : public CShape
{
private:
	vector<Point> ctrlPoints;
	vector<Point> segmentsPoints;
	
	bool closed = false;

public:
	CBezier(int x, int y, float r1, float g1, float b1, float r2, float g2, float b2)
		: CShape(x, y, r1, g1, b1, r2, g2, b2, true)
	{
		ctrlPoints.push_back({ x, y });
		ctrlPoints.push_back({ x, y });
	}

	~CBezier() { cout << "Se destruyo una curva de bezier" << endl; }

	// Add new point to bezier curve
	void newPoint(int x, int y) override
	{
		ctrlPoints.push_back({ x, y });
	}

	// Update last point value
	void update(int x, int y)
	{
		ctrlPoints.back() = { x, y };
	}

	void forceFinish(int x, int y) override
	{ 
		ctrlPoints.back() = { x, y };
		closed = true;
		setRenderValues();
	}

	// Render control structure
	void renderCtrlPoints()
	{
		drawCtrlPolygon();
		renderBox();

		// Render each control points
		int n = ctrlPoints.size();
		for (int i = 0; i < n; i++)
			ctrlPoints[i].renderCtrlPoint();
	}

	void setRenderValues()
	{
		// set bounding box values
		int minX = ctrlPoints[0].x, minY = ctrlPoints[0].y;
		int maxX = ctrlPoints[0].x, maxY = ctrlPoints[0].y;
		int n = ctrlPoints.size();

		for (int i = 1; i < n; i++)
		{
			if (ctrlPoints[i].x < minX)
				minX = ctrlPoints[i].x;
			else if(ctrlPoints[i].x > maxX)
				maxX = ctrlPoints[i].x;

			if (ctrlPoints[i].y > minY)
				minY = ctrlPoints[i].y;
			else if (ctrlPoints[i].y < maxY)
				maxY = ctrlPoints[i].y;
		}

		boxPoints[0].x = minX; boxPoints[0].y = minY;
		boxPoints[1].x = minX; boxPoints[1].y = maxY;
		boxPoints[2].x = maxX; boxPoints[2].y = maxY;
		boxPoints[3].x = maxX; boxPoints[3].y = minY;

		// set segment points values
		// candidatas = (float)25 / ((float)maxD * 1.5);
		// candidatas = (float)1 / ((float) n * 5.0);
		Point newPoint;
		int dx = maxX - minX, dy = maxY - maxY, maxD = max(dx, dy);
		float step = (float)1 / ((float) n * 5.0);
		cout << dx << " " << dy << " " <<  step << endl;
		

		segmentsPoints.push_back({ ctrlPoints[0].x, ctrlPoints[0].y });
		for (float t = 0; t <= 1; t += step)
		{
			newPoint = nextCurvePoint(ctrlPoints, t);
			segmentsPoints.push_back({ newPoint.x, newPoint.y });
		}
		segmentsPoints.push_back({ ctrlPoints.back().x, ctrlPoints.back().y, });
	}

	void drawCtrlPolygon()
	{
		int n = ctrlPoints.size();
		// Draw Border
		for (int i = 1; i < n; i++)
			drawLine(ctrlPoints[i - 1].x, ctrlPoints[i - 1].y, ctrlPoints[i].x, ctrlPoints[i].y, borderColor);
	}

	Point nextCurvePoint(vector<Point> points, float t)
	{	
		if (points.size() == 1)
			return points[0];
		else
		{	
			int x, y;
			vector<Point> newPoints;
			for (int i = 0; i < points.size() - 1; i++)
			{
				x = (1 - t) * points[i].x + t * points[i + 1].x;
				y = (1 - t) * points[i].y + t * points[i + 1].y;
				newPoints.push_back({ x, y });
			}
			return nextCurvePoint(newPoints, t);
		}
	}

	void render(const bool modeHardware)
	{
		if (closed)
		{	
			int n = segmentsPoints.size();
			for (int i = 1; i < n; i++)
				drawLine(segmentsPoints[i-1].x, segmentsPoints[i-1].y,	segmentsPoints[i].x, segmentsPoints[i].y, fillColor);
		}
		else 
			drawCtrlPolygon();
	}

	bool onClick(int x, int y)
	{	// We check if the click fell close to one of the segments, threshold: 6 pixels
		int x0, y0, x1, y1, dy, dx, c, distance;
		int n = segmentsPoints.size();

		for (int i = 1; i < n; i++)
		{
			x0 = segmentsPoints[i-1].x, y0 = segmentsPoints[i-1].y;
			x1 = segmentsPoints[i].x, y1 = segmentsPoints[i].y;

			if ((x1 > x0 && (x > x1 || x < x0)) || (x1 <= x0 && (x > x0 || x < x1)))
				continue;

			dy = y0 - y1;
			dx = x1 - x0;
			c = x0 * y1 - x1 * y0;

			if ((int)abs(dy * x + dx * y + c) / sqrt(dy * dy + dx * dx) <= 6)
				return true;
		}

		return false;
	}

	void clickedCtrlPoint(int x, int y)
	{
		// We check if the click fell on a vertex
		int dx, dy;
		int n = ctrlPoints.size();
		for (int i = 0; i < n; i++)
		{
			dx = (x - ctrlPoints[i].x);
			dy = (y - ctrlPoints[i].y);
			// Check squared distance between vertex i and the click
			if ((dx * dx + dy * dy) <= 25)
			{
				pointSelected = &ctrlPoints[i];
				return;
			}
		}
	}

	void onMove(int x1, int y1)
	{

		if (pointSelected)
		{	// Only move the vertex selected
			pointSelected->x = x1;
			pointSelected->y = y1;
		}
		else
		{
			// We move the whole curve
			int dx = x1 - anchorPoint.x;
			int dy = y1 - anchorPoint.y;

			anchorPoint.x = x1;
			anchorPoint.y = y1;

			int n = ctrlPoints.size();
			for (int i = 0; i < n; i++)
			{
				ctrlPoints[i].x += dx;
				ctrlPoints[i].y += dy;
			}

			int m = segmentsPoints.size();
			for (int i = 0; i < m; i++)
			{
				segmentsPoints[i].x += dx;
				segmentsPoints[i].y += dy;
			}

			moveBoundingBox(dx, dy);
		}
		
	}

	bool finished() override
	{
		return closed;
	}

	std::string getInfo()
	{
		return "BEZIER";
	}
};