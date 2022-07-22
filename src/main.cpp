#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

#include <GL/freeglut.h>
#include <iostream>
#include <memory>
#include <list>
#include <algorithm>

#include "line.h"
#include "circle.h"
#include "ellipse.h"
#include "rectangle.h"
#include "triangle.h"
#include "bezier_curve.h"
#include "paintUI.h"

PaintUI ui;
int width = 1600, height = 800;
int firstX0, firstY0;
bool drawing = true;
bool clickedCtrlPoint = false;

// Displayed Figures 
list<shared_ptr<CShape>> shapes;
shared_ptr<CShape> drawingShape = nullptr;

bool isHigherLevel(shared_ptr<CShape> fig, shared_ptr<CShape> figure);

void deleteFigure();

void deleteAllFigures();

void createFigure(int x1, int y1)
{
	const int shapeSelected = ui.shapeSelected;
	const bool filled = ui.allowFill;
	const float* fillColor = ui.fillColor, * borderColor = ui.borderColor;
	int x0 = firstX0, y0 = firstY0;

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

	shapes.insert(std::upper_bound(shapes.begin(), shapes.end(),
		drawingShape, isHigherLevel), drawingShape);

	drawingShape = nullptr;
	drawing = false;
}

void setViewport()
{
	// Set the viewport to be the entire new window
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	glOrtho(-0.5, width - 0.5, height - 0.5, -0.5, -1, 1);
}

void renderScene(void) 
{	
	float *bgColor = ui.bgColor;
	bool  currentMode = ui.currentMode;

	//Clear Frame
	glClearColor(bgColor[0], bgColor[1], bgColor[2], 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// Render every shape in canvas
	if (drawingShape)
		drawingShape->render(currentMode);

	for (auto const& s : shapes)
		s->render(currentMode);

	if (selectedShape)
		selectedShape->renderCtrlPoints();

	// ImGui window rendering
	ui.renderWindow();
	
	// Present frame buffer
	glutSwapBuffers();
	glutPostRedisplay();
}

void onResize(int w, int h)
{	
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)w, (float)h);

	if (h == 0)
		return;

	width = w;
	height = h;
	
	setViewport();
}

// Deberiamos revisar esto al revez, para revisar primero la lista que esta en el nivel 
// superior
void onHoverShape(int x, int y)
{
	for (auto it = shapes.crbegin(); it != shapes.crend(); it++)
	{
		if ((* it)->onClick(x, y))
		{
			hoveredShape = *it;
			return;
		}
	}
	hoveredShape = nullptr;
}
	

void onClickCanvas(int button, int state, int x, int y)
{	
	// Manage clicked button (left, rigth, middle)
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if(state == GLUT_DOWN)
		{	// Left-click was pressed
			
			// Check if click fell on a ctrl point of the selected figure
			if (selectedShape && selectedShape->clickedCtrlPoint(x, y))
				break;

			// Check if click fell on figure
			unselectFigure();
			
			if (hoveredShape) 
			{	// The click fell on top of a figure
				selectedShape = hoveredShape;
				selectedShape->setAnchorPoint(x, y);
				selectedShape->clickedCtrlPoint(x, y);
				drawing = false;
			}
			else if (ui.shapeSelected > 3)
			{
				if (!drawingShape)
					createFigure(x, y);
				else 
					drawingShape->newPoint(x, y);
			}
			else 
			{	// We can draw
				drawing = true;
				firstX0 = x; firstY0 = y;
			}
		}
		else 
		{	// Left-click was lifted
			if (drawingShape && drawingShape->finished())
				saveFigure();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_UP && ui.shapeSelected == 5 && drawingShape)
		{
			drawingShape->forceFinish(x, y);
			saveFigure();
		}
		else if (ui.shapeSelected == 4)
		{
			unselectFigure();
			drawingShape = nullptr;
			drawing = false;
		}
		else
			unselectFigure();
		break;
	default:
		unselectFigure();
		break;
	}
}

void onClick(int button, int state, int x, int y)
{	
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse)
		ImGui_ImplGLUT_MouseFunc(button, state, x, y);
	else
		onClickCanvas(button, state, x, y);
}

void onMotion(int x, int y)
{	
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse)
		ImGui_ImplGLUT_MotionFunc(x, y);
	else
	{
		if (drawing)
		{
			if (!drawingShape)
				createFigure(x, y);
			else
				drawingShape->update(x, y);
		}
		else if (selectedShape) 
		{	// Drag shape position
			if (clickedCtrlPoint)
				glutSetCursor(GLUT_CURSOR_CROSSHAIR);
			else
				glutSetCursor(GLUT_CURSOR_CYCLE);
			selectedShape->onMove(x, y);
		}
	}
}

void onPassiveMotion(int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddMousePosEvent((float)x, (float)y);

	if (drawingShape)
	{	// We are drawing a Triangle or a Bezier curve
		drawingShape->update(x, y);
	}
	else
	{	// We are freely hovering over the canvas
		onHoverShape(x, y);
		if (hoveredShape)
			glutSetCursor(GLUT_CURSOR_CYCLE);
		else
			glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	}
}

void onKeyboardEntry(unsigned char c, int x, int y)
{
	switch (c)
	{
		case '1':	ui.shapeSelected = 0;						break;  // 1-6: Change shape to draw
		case '2':	ui.shapeSelected = 1;						break;
		case '3':	ui.shapeSelected = 2;						break;
		case '4':	ui.shapeSelected = 3;						break;
		case '5':	ui.shapeSelected = 4;						break;
		case '6':	ui.shapeSelected = 5;						break;
		case 'h':	ui.currentMode = !ui.currentMode;			break;  // h: Change rendering mode
		case 'B':   ui.openBGPicker = !ui.openBGPicker;			break;  // B,C,F: Open color windows 
		case 'C':   ui.openBorderPicker = !ui.openBorderPicker; break;
		case 'F':   ui.openFillPicker = !ui.openFillPicker;		break;
		case 'u':	unselectFigure();							break;	// u: Unselect current figure
		case 'b':   ui.toggleLevel(-2);							break;	// f, b, -, + : Toggle current figure layer level
		case '-':   ui.toggleLevel(-1);							break;
		case '+':   ui.toggleLevel(1);							break;
		case 'f':   ui.toggleLevel(2);							break;  
		case 'x':	deleteAllFigures();							break;  // x: Delete all figures
		case   8:   deleteFigure();								break;  // backspace: Delete current figure
		default:    ImGui_ImplGLUT_KeyboardFunc(c, x, y);		break;  // Give control to ImGui
	}
}

int main(int argc, char** argv)
{	
	// Initialize GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Proyecto 1 - Gabriel Carrizo");
	setViewport();
	
	// Initialize ImGui
	ui.init();

	// Set GLUT custom event callbacks
	// NOTE: This will overwrite some of bindings set by ImGui_ImplGLUT_InstallFuncs() 
	glutDisplayFunc(renderScene);
	glutMouseFunc(onClick);
	glutMotionFunc(onMotion);
	glutPassiveMotionFunc(onPassiveMotion);
	glutKeyboardFunc(onKeyboardEntry);
	glutReshapeFunc(onResize);

	glutMainLoop();

	// Cleanup ImGui
	ui.close();

	return 0;
}