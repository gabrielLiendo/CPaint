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
#include "square.h"
#include "circle.h"


int width = 1600, height = 800;

// Displayed Figures 
list<shared_ptr<CShape>> shapes;
shared_ptr<CShape> drawingShape;


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

	// Render shape currently being drawn
	if(drawingShape)
		drawingShape->render(currentMode);

	// Render every shape already in canvas
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
			if (shapeSelected == "Line")
			{
				shared_ptr<CLine> l = make_shared<CLine>(fillColor[0], fillColor[1], fillColor[2]);
				l->set(x, y);
				drawingShape = l;
			}
			else if (shapeSelected == "Triangle")
			{
				shared_ptr<CTriangle> t = make_shared<CTriangle>(fillColor[0], fillColor[1], fillColor[2]);
				t->set(x, y);
				drawingShape = t;
			}
			else if (shapeSelected == "Rectangle")
			{
				shared_ptr<CSquare> s = make_shared<CSquare>(fillColor[0], fillColor[1], fillColor[2]);
				s->set(x, y);
				drawingShape = s;
			}
			else if (shapeSelected == "Circle")
			{
				shared_ptr<C_Circle> c = make_shared<C_Circle>(fillColor[0], fillColor[1], fillColor[2]);
				c->set(x, y);
				drawingShape = c;
			}
		}
		else 
		{
			// Left-click released
			shapes.push_back(drawingShape);
			drawingShape = nullptr;
			cout << "DONE" << endl;
		}

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