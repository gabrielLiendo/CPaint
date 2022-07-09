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
#include "paintUI.h"

PaintUI ui;
int width = 1600, height = 800;
int firstX0, firstY0;
bool drawing = true;

// Displayed Figures 
list<shared_ptr<CShape>> shapes;
shared_ptr<CShape> drawingShape = nullptr;


//Click dragueas y sueltas, click dragueas y sueltas y listo
bool isHigherLevel(shared_ptr<CShape> fig, shared_ptr<CShape> figure)
{
	return figure->getLayerLevel() > fig->getLayerLevel();
}

void createShape(int x1, int y1)
{
	const int shapeSelected = ui.shapeSelected;
	const float* fillColor = ui.fillColor, * borderColor = ui.borderColor;
	int x0 = firstX0, y0 = firstY0;

	if (shapeSelected == 0)
	{
		shared_ptr<CLine> l = make_shared<CLine>(x0, y0, x1, y1,
			fillColor[0], fillColor[1], fillColor[2], borderColor[0], borderColor[1], borderColor[2]);
		drawingShape = l;
	}
	else if (shapeSelected == 1)
	{
		shared_ptr<CCircle> c = make_shared<CCircle>(x0, y0, x1, y1,
			fillColor[0], fillColor[1], fillColor[2], borderColor[0], borderColor[1], borderColor[2]);
		drawingShape = c;
	}
	else if (shapeSelected == 2)
	{
		shared_ptr<CEllipse> e = make_shared<CEllipse>(x0, y0, x1, y1,
			fillColor[0], fillColor[1], fillColor[2], borderColor[0], borderColor[1], borderColor[2]);
		drawingShape = e;
	}
	else if (shapeSelected == 3)
	{
		shared_ptr<CRectangle> s = make_shared<CRectangle>(x0, y0, x1, y1,
			fillColor[0], fillColor[1], fillColor[2], borderColor[0], borderColor[1], borderColor[2]);
		drawingShape = s;
	}
	else if (shapeSelected == 4)
	{
		shared_ptr<CTriangle> s = make_shared<CTriangle>(x0, y0, x1, y1,
			fillColor[0], fillColor[1], fillColor[2], borderColor[0], borderColor[1], borderColor[2]);
		drawingShape = s;
	}
}

//Al pasar el mouse encima de una figura 
void renderScene(void) 
{	
	float *bgColor = ui.bgColor;
	bool  currentMode = ui.currentMode;

	//Clear Frame
	glClearColor(bgColor[0], bgColor[1], bgColor[2], 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	//Draw the ImGui frame
	ui.drawUI();

	// Render GUI
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	// Render every shape already in canvas
	if (drawingShape)
		drawingShape->render(currentMode);
	
	for (auto const& s : shapes) 
		s->render(currentMode);

	if (selectedShape)
		selectedShape->renderCtrlPoints();
	
	// Present frame buffer
	glutSwapBuffers();
	glutPostRedisplay();
}

void unselectFigure()
{
	if (selectedShape)
		selectedShape->release();
	selectedShape = nullptr;
}


void onResize(int w, int h)
{	
	width = w;
	height = h;

	// Set the viewport to be the entire new window
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.5, width - 0.5, height - 0.5, -0.5, -1, 1);
	glViewport(0, 0, width, height);
}

void onClickShape(int x, int y)
{
	for (auto const& s : shapes)
	{
		if (s->onClick(x, y))
		{
			s->clickedCtrlPoint(x, y);
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
		if(state== GLUT_DOWN)
		// Left-click was pressed
		{	
			unselectFigure();
			onClickShape(x, y);
			
			// A shape was selected
			if (selectedShape) {
				cout << "FIGURA SELECCIONADA" << endl;
				selectedShape->setAnchorPoint(x, y);
				drawing = false;
			}
			else // We can draw
			{	
				cout << "ESPACIO EN BLANCO" << endl;
				drawing = true;
				firstX0 = x; firstY0 = y;
			}
		}
		else // GLUT_UP
		{
			if (drawing && drawingShape && drawingShape->finished())
			{
				// We finisish the figure, and it's marked as 'selected'
				selectedShape = drawingShape;

				shapes.insert(std::upper_bound(shapes.begin(), shapes.end(),
					drawingShape, isHigherLevel), drawingShape);

				drawingShape = nullptr;
				drawing = false;
				cout << "FIGURA DIBUJADA" << endl;
			}
			glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
		}
		break;
	default:
		unselectFigure();
		drawing = false;
		break;
	}
}
	

void onClick(int button, int state, int x, int y)
{	
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse){
		ImGui_ImplGLUT_MouseFunc(button, state, x, y);
	}
	else {
		onClickCanvas(button, state, x, y);
	}
}



void onMotion(int x1, int y1)
{	
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse)
		ImGui_ImplGLUT_MotionFunc(x1, y1);
	else
	{
		if (drawing)
		{
			if (!drawingShape)
				createShape(x1, y1);
			else
				drawingShape->update(x1, y1);
				
		}
		else if (selectedShape) {
			// Draggin shape position
			selectedShape->onMove(x1, y1);
			glutSetCursor(GLUT_CURSOR_CYCLE);
		}
			
	}
}



void onKeyboardEntry(unsigned char c, int x, int y)
{
	// 1-6: Change shape to draw
	if (c >= 49 && c <= 54)
		ui.shapeSelected = c - 49;
	// Change rendering mode
	if (c == 'h')
		ui.currentMode = !ui.currentMode;
	// Open color windows 
	else if (c == 'B')
		ui.openBGPicker = !ui.openBGPicker;
	else if (c == 'c')
		ui.openBorderPicker = !ui.openBorderPicker;
	else if (c == 'f')
		ui.openFillPicker = !ui.openFillPicker;
	// Unselect current figure
	else if (c == 'u')
		unselectFigure();
	// Toggle current figure layer level
	else if (c == 'b')
		ui.toggleLevel(-2);
	else if (c == '-')
		ui.toggleLevel(-1);
	else if (c == '+')
		ui.toggleLevel(1);
	else if (c == 'f')
		ui.toggleLevel(2);
	// Redirect input to ImGui
	else
		ImGui_ImplGLUT_KeyboardFunc(c, x, y);
}



int main(int argc, char** argv)
{	
	//ui = PaintUI(selectedShape);

	// Initialize GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Proyecto 1 - Gabriel Carrizo");

	glViewport(0, 0, width, height);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.5, width - 0.5, height - 0.5, -0.5, -1, 1);
	
	// Setup GLUT display function
	glutDisplayFunc(renderScene);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.Fonts->AddFontFromFileTTF("misc/fonts/DroidSans.ttf", 15.5f);
	io.Fonts->AddFontFromFileTTF("misc/fonts/Roboto-Medium.ttf", 15.0f);

	ImGui::StyleColorsDark();
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 5));
	ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));

	// Setup Platform/Renderer backends
	ImGui_ImplGLUT_Init();
	ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();

	// Set GLUT custom event callbacks
	// NOTE: This will overwrite some of bindings set by ImGui_ImplGLUT_InstallFuncs() 
	glutMouseFunc(onClick);
	glutMotionFunc(onMotion);
	glutKeyboardFunc(onKeyboardEntry);
	
	//glutSpecialFunc(onSpecialEntry);

	glutMainLoop();

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}