#pragma once

class Point2D
{
private:
	int x, y;

public:
	Point2D() { x = 0; y = 0; };

	Point2D(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	void set(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	int getX() { return x; }

	int getY() { return y; }
};