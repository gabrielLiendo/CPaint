#pragma once
#include <vector>
#include "line.h"


class CBezier : public CShape
{
private:
	std::vector<Point> points;

public:
	CBezier(int x0, int y0, float r1, float g1, float b1, float r2, float g2, float b2)
		: CShape(x0, y0, r1, g1, b1, r2, g2, b2, false)
	{
		cout << "paso" << endl;
		points.push_back({ x0, y0 });
		cout << "paso" << endl;
	}

	~CBezier() { cout << "Se destruyo una curva de bezier" << endl; }

	// Add new point to bezier curve
	void newPoint(int x, int y) override
	{
		points.push_back(Point(x, y));
	}

	// Update last point value
	void update(int x, int y)
	{
		points.back() = { x, y };
	}

	// Render each control point
	void renderCtrlPoints()
	{
		for (int i = 0; i < 3; i++)
			points[i].renderCtrlPoint();
	}

	void render(const bool modeHardware)
	{
		cout << "paso" << endl;
		int n = points.size();
		cout << "paso" << endl;
		if (n >= 2)
		{
			cout << "paso" << endl;
			// Draw Border
			for (int i = 0; i < n; i++)
				drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, borderColor);
			cout << "paso" << endl;
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
		return true;
	}

	std::string getInfo()
	{
		return "BEZIER";
	}
};