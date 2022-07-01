#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

#include <GL/freeglut.h>
#include <iostream>
#include <list>
#include <memory>

#include "imgui_UI.h"
#include "line.h"
#include "triangle.h"
#include "rectangle.h"
#include "circle.h"


int width = 1600, height = 800;

// Displayed Figures 
list<shared_ptr<CShape>> shapes;
shared_ptr<CShape> drawingShape = nullptr;
shared_ptr<CShape> selectedShape = nullptr;

int firstX0, firstY0;
bool drawing = true;

//Meter la Drawing Shape en la lista shapes
//Una sola paleta, derecho para borde e izquierdo para borde
//Click dragueas y sueltas, click dragueas y sueltas y listo
//GL Loop
//Al pasar el mouse encima de una figura 
void renderScene(void) 
{	
	//Clear Frame
	glClearColor(bgColor[0], bgColor[1], bgColor[2], 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	//Draw the ImGui frame
	drawUI();

	// Render GUI
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	// Render every shape already in canvas
	if (drawingShape)
		drawingShape->render(currentMode);

	if (selectedShape) {
		selectedShape->setColor(fillColor[0], fillColor[1], fillColor[2]);
	}
		

	for (auto const& s : shapes) 
		s->render(currentMode);
	
	// Present frame buffer
	glutSwapBuffers();
	glutPostRedisplay();
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

	if (io.WantCaptureMouse){
		ImGui_ImplGLUT_MouseFunc(button, state, x, y);
	}
	else {
		onClickCanvas(button, state, x, y);
	}
}

void createShape(int x1, int y1)
{	
	int x0 = firstX0, y0 = firstY0;
	if (shapeToDraw == "Line")
	{
		shared_ptr<CLine> l = make_shared<CLine>(fillColor[0], fillColor[1], fillColor[2]);
		l->set(x0, y0, x1, y1);
		drawingShape = l;
	}
	else if (shapeToDraw == "Triangle")
	{
		shared_ptr<CTriangle> t = make_shared<CTriangle>(fillColor[0], fillColor[1], fillColor[2]);
		t->set(x0, y0, x1, y1);
		drawingShape = t;
	}
	else if (shapeToDraw == "Rectangle")
	{
		shared_ptr<CRectangle> s = make_shared<CRectangle>(fillColor[0], fillColor[1], fillColor[2]);
		s->set(x0, y0, x1, y1);
		drawingShape = s;
	}
	else if (shapeToDraw == "Circle")
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


int main(int argc, char** argv)
{	
	// Initialize GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Proyecto 1 - Gabriel Carrizo");

	glViewport(0, 0, width, height);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	
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

	glutMainLoop();

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}