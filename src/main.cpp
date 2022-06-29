#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

#include <GL/freeglut.h>
#include <iostream>
#include <list>
#include <memory>

#include "line.h"
#include "triangle.h"
#include "square.h"
#include "circle.h"


using namespace std;

typedef enum { HARDWARE, SOFTWARE } renderingMode;
typedef enum { LINE, TRIANGLE, RECTANGLE, CIRCLE } figureShape;

renderingMode mode = HARDWARE;
figureShape shape = LINE;

int width = 840, height = 480;

// Colors
float bgColor[] = { 0.1f, 0.2f, 0.4f };
float borderColor[] = {0.0f, 0.0f, 0.0f};
float fillColor[] = {1.0f, 1.0f, 1.0f};

// Displayed Figures 
list<shared_ptr<CShape>> shapes;
shared_ptr<CShape> drawingShape;


void drawGUI()
{
	ImGui::Begin("Main Configuration");                   

	ImGui::ColorEdit3("Background Color", bgColor); 

	ImGui::ColorEdit3("Fill Color", fillColor);

	if (ImGui::Button("Line"))
		shape = LINE;
	if (ImGui::Button("Triangle"))
		shape = TRIANGLE;
	if (ImGui::Button("Rectangle"))
		shape = RECTANGLE;
	if (ImGui::Button("Circle"))
		shape = CIRCLE;
	
	ImGui::End();
}

void renderScene(void) 
{	
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	//Draw the ImGui frame
	drawGUI();

	cout << "." << endl;

	// Render GUI
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	glClearColor(bgColor[0], bgColor[1], bgColor[2], 1);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	// Render shape currently being drawn
	if(drawingShape)
		drawingShape->render();

	// Render every shape already in canvas
	for (auto const& s : shapes) 
		s->render();
	
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

void onClickCanvas(int button, int state, int x, int y)
{	
	// Manage clicked button (left, rigth, middle)
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		// Manage state of left-click (pressed or released)
		switch (state)
		{
		case GLUT_DOWN:
		{	
			y = height - y - 1;
			if (shape == LINE)
			{	
				shared_ptr<CLine> l = make_shared<CLine>(fillColor[0], fillColor[1], fillColor[2]);
				l->set(x, y);
				drawingShape = l;
			}
			else if (shape == TRIANGLE)
			{
				shared_ptr<CTriangle> t = make_shared<CTriangle>(fillColor[0], fillColor[1], fillColor[2]);
				t->set(x, y);
				drawingShape = t;
			}
			else if (shape == RECTANGLE)
			{	
				shared_ptr<CSquare> s = make_shared<CSquare>(fillColor[0], fillColor[1], fillColor[2]);
				s->set(x, y);
				drawingShape = s;
			}
			else if (shape == CIRCLE)
			{
				shared_ptr<C_Circle> c = make_shared<C_Circle>(fillColor[0], fillColor[1], fillColor[2]);
				c->set(x, y);
				drawingShape = c;
			}
			
			break;
		}
		case GLUT_UP:
			shapes.push_back(drawingShape);
			drawingShape = NULL;
			cout << "DONE" << endl;
		default:
			break;
		}
	default:
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

void onMotion(int x, int y)
{	
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse)
		ImGui_ImplGLUT_MotionFunc(x, y);
	else
	{
		if (drawingShape)
			drawingShape->update(x, height - y - 1);
	}
}

void onNormalKey(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'B':

	case 27:
		exit(0);
		break;
	default:
		break;
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
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGLUT_Init();
	ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();

	// Set GLUT custom event callbacks
	// NOTE: This will overwrite some of bindings set by ImGui_ImplGLUT_InstallFuncs() 
	glutMouseFunc(onClick);
	glutMotionFunc(onMotion);
	glutKeyboardFunc(onNormalKey);

	glutMainLoop();

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}