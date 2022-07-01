#pragma once
#include "imgui_UI.h"

class Paint : public UI
{

public:
	void onClickShape(int x, int y)
	{
		for (auto const& s : shapes)
		{
			if (s->onClick(x, y))
			{
				selectedShape = s;
				return;
			}
		}
		selectedShape = nullptr;
	}
	void onClickCanvas(int button, int state, int x, int y)
	{
		// Manage clicked button (left, rigth, middle)
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN)
				// Left-click was pressed
			{
				y = height - y - 1;
				onClickShape(x, y);

				// A shape was selected
				if (selectedShape) {
					cout << "CUADRADO SELECCIONADO" << endl;
					selectedShape->setRefPoint(x, y);
				}
				else // We can draw
				{

					cout << "VACIO" << endl;
					cout << x << " " << y << endl;
					drawing = true;
					firstX0 = x; firstY0 = y;
				}
			}
			else
			{
				if (drawingShape)
				{
					shapes.push_back(drawingShape);
					drawingShape = nullptr;
					drawing = false;
					cout << "DONE" << endl;
				}
				if (selectedShape)
					selectedShape = nullptr;
			}
			break;
		default:
			drawing = false;
			break;
		}
	}


	void onClick(int button, int state, int x, int y)
	{
		ImGuiIO& io = ImGui::GetIO();

		if (io.WantCaptureMouse) {
			ImGui_ImplGLUT_MouseFunc(button, state, x, y);
		}
		else {
			onClickCanvas(button, state, x, y);
		}
	}

	void createShape(int x1, int y1)
	{
		int x0 = firstX0, y0 = firstY0;
		if (shapeSelected == "Line")
		{
			shared_ptr<CLine> l = make_shared<CLine>(fillColor[0], fillColor[1], fillColor[2]);
			l->set(x0, y0, x1, y1);
			drawingShape = l;
		}
		else if (shapeSelected == "Triangle")
		{
			shared_ptr<CTriangle> t = make_shared<CTriangle>(fillColor[0], fillColor[1], fillColor[2]);
			t->set(x0, y0, x1, y1);
			drawingShape = t;
		}
		else if (shapeSelected == "Rectangle")
		{
			shared_ptr<CRectangle> s = make_shared<CRectangle>(fillColor[0], fillColor[1], fillColor[2]);
			s->set(x0, y0, x1, y1);
			drawingShape = s;
		}
		else if (shapeSelected == "Circle")
		{
			shared_ptr<C_Circle> c = make_shared<C_Circle>(fillColor[0], fillColor[1], fillColor[2]);
			c->set(x0, y0, x1, y1);
			drawingShape = c;
		}
	}

	void onMotion(int x1, int y1)
	{
		ImGuiIO& io = ImGui::GetIO();

		if (io.WantCaptureMouse)
			ImGui_ImplGLUT_MotionFunc(x1, y1);
		else
		{
			y1 = height - y1 - 1;
			if (drawing)
			{
				if (!drawingShape)
					createShape(x1, y1);
				else
					drawingShape->set(firstX0, firstY0, x1, y1);
			}
			else if (selectedShape)
				selectedShape->onMove(x1, y1);
		}
	}
};