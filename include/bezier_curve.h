#pragma once
#include <vector>
#include "shape.h"


class CBezier : public CShape
{
private:
	vector<Point> points;
	bool addPoint = false;
	bool closed = false;

public:
	CBezier(int x, int y, float r1, float g1, float b1, float r2, float g2, float b2)
		: CShape(x, y, r1, g1, b1, r2, g2, b2, true)
	{
		points.push_back({ x, y });
		points.push_back({ x, y });
	}

	~CBezier() { cout << "Se destruyo una curva de bezier" << endl; }

	// Add new point to bezier curve
	void newPoint(int x, int y) override
{
		points.push_back({ x, y });
	}

	// Update last point value
	void update(int x, int y)
	{
		points.back() = { x, y };
	}

	void forceFinish(int x, int y) override
	{ 
		points.push_back({ x, y });
		closed = true;
	}

	// Render each control point
	void renderCtrlPoints()
	{
		// Render each vertex
		int n = points.size();
		for (int i = 0; i < n; i++)
			points[i].renderCtrlPoint();
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
		int n = points.size();
		// Draw Border
		for (int i = 1; i < n; i++)
			drawLine(points[i - 1].x, points[i - 1].y, points[i].x, points[i].y, borderColor);

		if (closed)
		{	
			Point oldPoint = points[0], newPoint;
			for (float t = 0; t < 1; t += 0.01)
			{
				newPoint = nextCurvePoint(points, t);
				drawLine(oldPoint.x, oldPoint.y, newPoint.x, newPoint.y, fillColor);
				oldPoint = newPoint;
			}
		}
	}

	bool onClick(int x, int y)
	{
		return false;
	}

	void clickedCtrlPoint(int x, int y)
	{

	}

	void onMove(int x1, int y1)
	{
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