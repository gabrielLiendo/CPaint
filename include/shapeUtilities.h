#pragma once
#include <list>
#include <memory>

#include "line.h"
#include "circle.h"
#include "ellipse.h"
#include "rectangle.h"
#include "triangle.h"
#include "bezierCurve.h"

list<shared_ptr<CShape>> shapes;
shared_ptr<CShape> hoveredShape = nullptr;
shared_ptr<CShape> drawingShape = nullptr;
shared_ptr<CShape> selectedShape = nullptr;

bool isHigherLevel(shared_ptr<CShape> fig, shared_ptr<CShape> figure)
{
	return figure->getLayerLevel() > fig->getLayerLevel();
}

void createFigure(int shapeSelected, int x0, int y0, int x1, int y1, float* fillColor, float* borderColor, bool filled)
{
	if (shapeSelected == 0)
	{
		shared_ptr<CLine> l = make_shared<CLine>(x0, y0, x1, y1, borderColor[0], borderColor[1], borderColor[2]);
		drawingShape = l;
	}
	else if (shapeSelected == 1)
	{
		shared_ptr<CCircle> c = make_shared<CCircle>(x0, y0, x1, y1,
			fillColor[0], fillColor[1], fillColor[2], borderColor[0], borderColor[1], borderColor[2], filled, false);
		drawingShape = c;
	}
	else if (shapeSelected == 2)
	{
		shared_ptr<CEllipse> e = make_shared<CEllipse>(x0, y0, x1, y1,
			fillColor[0], fillColor[1], fillColor[2], borderColor[0], borderColor[1], borderColor[2], filled);
		drawingShape = e;
	}
	else if (shapeSelected == 3)
	{
		shared_ptr<CRectangle> s = make_shared<CRectangle>(x0, y0, x1, y1,
			fillColor[0], fillColor[1], fillColor[2], borderColor[0], borderColor[1], borderColor[2], filled);
		drawingShape = s;
	}
	else if (shapeSelected == 4)
	{
		shared_ptr<CTriangle> s = make_shared<CTriangle>(x1, y1,
			fillColor[0], fillColor[1], fillColor[2], borderColor[0], borderColor[1], borderColor[2], filled);
		drawingShape = s;
	}
	else if (shapeSelected == 5)
	{
		shared_ptr<CBezier> b = make_shared<CBezier>(x1, y1,
			fillColor[0], fillColor[1], fillColor[2], borderColor[0], borderColor[1], borderColor[2]);
		drawingShape = b;
	}
}

void unselectFigure()
{
	if (selectedShape)
		selectedShape->release();
	selectedShape = nullptr;
}

void saveFigure()
{
	// We finisish the figure, and it's marked as 'selected'
	selectedShape = drawingShape;
	if (shapes.size() > 1)
		selectedShape->setLayerLevel(shapes.back()->getLayerLevel());
	shapes.push_back(drawingShape);
	drawingShape = nullptr;
}

void deleteFigure()
{
	if (selectedShape)
		shapes.remove(selectedShape);
	selectedShape = nullptr;
}

void deleteAllFigures()
{
	shapes.clear();
	selectedShape = nullptr;
}