#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

#include <GL/freeglut.h>
#include <iostream>
#include <memory>
#include <list>

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
bool isHigherLevel(shared_ptr<CShape> fig, shared_ptr<CShape> figure);

void deleteFigure();

void deleteAllFigures();

void createShape(int x1, int y1)
{
	const int shapeSelected = ui.shapeSelected;
	const bool filled = ui.allowFill;
	const float* fillColor = ui.fillColor, * borderColor = ui.borderColor;
	int x0 = firstX0, y0 = firstY0;

	if (shapeSelected == 0)
	{
		shared_ptr<CLine> l = make_shared<CLine>(x0, y0, x1, y1,
			fillColor[0], fillColor[1], fillColor[2], borderColor[0], borderColor[1], borderColor[2], filled);
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
		shared_ptr<CTriangle> s = make_shared<CTriangle>(x0, y0, x1, y1,
			fillColor[0], fillColor[1], fillColor[2], borderColor[0], borderColor[1], borderColor[2], filled);
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


	// Render every shape already in canvas
	if (drawingShape)
		drawingShape->render(currentMode);

	for (auto const& s : shapes)
		s->render(currentMode);

	if (selectedShape)
		selectedShape->renderCtrlPoints();

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	//Draw the ImGui frame
	ui.drawUI();

	// Render GUI
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

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
			cout << x << " " << y << endl;
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
				cout << "ESPACIO EN BLANCO " << x << " " << y << endl;
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
		case 'x':   ui.openDeleteModal = true;					break;  // x: Delete all figures
		case   8:   deleteFigure();								break;  // backspace: Delete current figure
		default:    ImGui_ImplGLUT_KeyboardFunc(c, x, y);		break;  // Give control to ImGui
	}
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
	//io.Fonts->AddFontFromFileTTF("misc/fonts/Roboto-Medium.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("misc/fonts/opensans/OpenSans-Bold.ttf", 18.0f);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("misc/fonts/Roboto-Regular.ttf", 16.0f);

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

	glutMainLoop();

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}