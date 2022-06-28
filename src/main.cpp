#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

#include <GL/freeglut.h>

#include <list>
#include <iostream>
#include <memory>

#include "line.h"
#include "triangle.h"
#include "square.h"

using namespace std;

typedef enum { HARDWARE, SOFTWARE } renderingMode;
typedef enum { LINE, TRIANGLE, SQUARE } figureShape;

renderingMode mode = HARDWARE;
figureShape shape = LINE;

int width = 840, height = 480;

// Colors
float bgColor[] = { 0.1f, 0.2f, 0.4f };
float borderColor[] = {0.0f, 0.0f, 0.0f};
float fillColor[] = {1.0f, 1.0f, 1.0f};

// Displayed Figures 
list<shared_ptr<CShape>> shapes;
shared_ptr<CShape> current_shape;


static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void my_display_code()
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}
void renderScene(void) 
{	
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	my_display_code();

	//Rendering
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();

	glClearColor(bgColor[0], bgColor[1], bgColor[2], 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//cout << "." << endl;

	if(current_shape)
		current_shape->render();

	// Render every shape
	for (auto const& s : shapes) 
		s->render();

	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	// Present frame buffer
	glutSwapBuffers();
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
				l->set(x, y, x, y);
				current_shape = l;
			}
			else if (shape == TRIANGLE)
			{
				shared_ptr<CTriangle> t = make_shared<CTriangle>(fillColor[0], fillColor[1], fillColor[2]);
				t->set(x, y, x, y, x, y);
				current_shape = t;
			}
			else if (shape == SQUARE)
			{	
				shared_ptr<CSquare> s = make_shared<CSquare>(fillColor[0], fillColor[1], fillColor[2]);
				s->set(x, y, x, y, x, y, x, y);
				current_shape = s;
			}
			
			break;
		}
		case GLUT_UP:
			shapes.push_back(current_shape);
			current_shape = NULL;
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

	if (io.WantCaptureMouse) 
		ImGui_ImplGLUT_MouseFunc(button, state, x, y); // Redirect event to ImGui
	else 
		onClickCanvas(button, state, x, y); // Manage click on canvas	
}

void onMotion(int x, int y)
{	
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse)
		ImGui_ImplGLUT_MotionFunc(x, y);
	else
	{
		if (current_shape)
		{
			current_shape->update(x, height - y - 1);
			glutPostRedisplay();
		}
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
	// NOTE: This will overwrite some of the functions set by ImGui_ImplGLUT_InstallFuncs() 
	glutMouseFunc(onClick);
	glutMotionFunc(onMotion);

	glutMainLoop();

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}