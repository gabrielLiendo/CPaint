#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

#include <GL/freeglut.h>
#include <iostream>
#include <list>
#include <memory>

#include "line.h"
#include "circle.h"
#include "ellipse.h"
#include "rectangle.h"
//#include "triangle.h"
#include "paintUI.h"

PaintUI ui;

int width = 1600, height = 800;

// Displayed Figures 
list<shared_ptr<CShape>> shapes;
shared_ptr<CShape> drawingShape = nullptr;

int firstX0, firstY0;
bool drawing = true;

//Una sola paleta, derecho para borde e izquierdo para borde
//Click dragueas y sueltas, click dragueas y sueltas y listo
//GL Loop
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
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	// Render every shape already in canvas
	if (drawingShape)
		drawingShape->render(currentMode);
		
	for (auto const& s : shapes) 
		s->render(currentMode);
	
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
	if (h == 0)
		return;

	width = w;
	height = h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	glOrtho(0, width, 0, height, -1, 1);

	// Set the viewport to be the entire window
	glViewport(0, 0, width, height);
}

void onClickShape(int x, int y)
{
	for (auto const& s : shapes)
	{
		if (s->onClick(x, y))
		{
			selectedShape = s; 
			selectedShape->setSelected(true);
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
			if (selectedShape)
			{
				selectedShape->release();
				selectedShape = nullptr;
			}

			y = height - y - 1;
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
		else 
		{
			if (drawing && drawingShape)
			{
				// We finisish the figure, and it's marked as 'selected'
				drawingShape->setSelected(true);
				selectedShape = drawingShape;
				shapes.push_back(drawingShape);
				drawingShape = nullptr;
				drawing = false;
				cout << "DONE" << endl;
			}
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

void createShape(int x1, int y1)
{	
	const int selected = ui.selected;
	float *fillColor = ui.fillColor, *borderColor = ui.borderColor;

	int x0 = firstX0, y0 = firstY0;

	if (selected == 0)
	{
		shared_ptr<CLine> l = make_shared<CLine>(x0, y0, x1, y1, 
			fillColor[0], fillColor[1], fillColor[2], borderColor[0], borderColor[1], borderColor[2]);
		drawingShape = l;
	}
	else if (selected == 1)
	{
		shared_ptr<CCircle> c = make_shared<CCircle>(x0, y0, x1, y1,
			fillColor[0], fillColor[1], fillColor[2], borderColor[0], borderColor[1], borderColor[2]);
		drawingShape = c;
	}
	else if (selected == 2)
	{
		shared_ptr<CEllipse> e = make_shared<CEllipse>(x0, y0, x1, y1,
			fillColor[0], fillColor[1], fillColor[2], borderColor[0], borderColor[1], borderColor[2]);
		drawingShape = e;
	}
	else if (selected == 3)
	{
		shared_ptr<CRectangle> s = make_shared<CRectangle>(x0, y0, x1, y1, 
			fillColor[0], fillColor[1], fillColor[2],borderColor[0], borderColor[1], borderColor[2]);
		drawingShape = s;
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
				drawingShape->update(x1, y1);
		}
		else if (selectedShape)
			selectedShape->onMove(x1, y1);
	}
}



void onKeyboardEntry(unsigned char c, int x, int y)
{
	// 1-6: Change shape to draw
	if (c >= 49 && c <= 54)
		ui.selected = c - 49;
	// Change rendering mode
	else if (c == 'h')
		ui.currentMode = !ui.currentMode;
	else if (c == 'B')
		ui.openBGPicker = !ui.openBGPicker;
	else if (c == 'u')
		unselectFigure();
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
	glOrtho(-0.5, width - 0.5, -0.5, height - 0.5, -1, 1);
	
	// Setup GLUT display function
	glutDisplayFunc(renderScene);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();

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