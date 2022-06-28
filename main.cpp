// 3. Bajar "Open File Dialog", "open"

#include <GL/freeglut.h>
#include <AntTweakBar.h>
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


void renderScene(void) 
{
	glClearColor(bgColor[0], bgColor[1], bgColor[2], 1);
	glClear(GL_COLOR_BUFFER_BIT);

	cout << "." << endl;

	if(current_shape)
		current_shape->render();

	// Render every shape
	for (auto const& s : shapes) 
		s->render();
	
	// Draw tweak bar
	TwDraw();

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

	// Send the new window size to AntTweakBar
	TwWindowSize(width, height);
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
			//cout << x << " " << y << endl;
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
	if (!TwEventMouseButtonGLUT(button, state, x, y))  // send event to AntTweakBar
	{ // event has not been handled by AntTweakBar
	  // your code here to handle the event
		onClickCanvas(button, state, x, y);
	}
	else {
		cout << "Interfaz" << endl;
	}
}

void onMotion(int x, int y)
{
	y = height - y - 1;

	if (current_shape)
	{	
		current_shape->update(x, y);
		glutPostRedisplay();
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
	
	// Initialize AntTweakBar
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(width, height);

	// Set GLUT event callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(onResize);
	glutMouseFunc(onClick);
	glutMotionFunc(onMotion);
	// Redirect GLUT events to AntTweakBar
	// - Directly redirect GLUT mouse "passive" motion events to AntTweakBar (same as MouseMotion)
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	// - Directly redirect GLUT key events to AntTweakBar
	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
	// - Directly redirect GLUT special key events to AntTweakBar
	glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);


	// Create tweak bar
	TwBar* bar = TwNewBar("Main Configuration");
	TwDefine(" GLOBAL help='List of commands and their shortcuts.' ");
	TwDefine(" 'Main Configuration' size='280 200' ");


	// Define a new enum types for the tweak bar
	TwEnumVal modeEV[] = { {HARDWARE, "Hardware"}, {SOFTWARE, "Software"}};
	TwType modeType = TwDefineEnum("Mode", modeEV, 2);

	TwEnumVal shapeEV[] = { {LINE, "Line"}, {TRIANGLE, "Triangle"}, {SQUARE, "Square"} };
	TwType shapeType = TwDefineEnum("Shape", shapeEV, 3);

	// Add var to the 'Scene' group
	TwAddVarRW(bar, "Background Color", TW_TYPE_COLOR3F, &bgColor,
		" help='Pick background color' group='Scene' ");

	// Add vars to the 'Draw' group
	TwAddVarRW(bar, "Rendering Mode", modeType, &mode,
		" key=h help='Toggle rendering mode.' group='Draw' ");

	TwAddVarRW(bar, "Shape", shapeType, &shape,
		" key=h help='Toggle shape of the figure to draw.' group='Draw' ");

	TwAddVarRW(bar, "borderColor", TW_TYPE_COLOR3F, &borderColor, " label='Border' group='Color' ");
	TwAddVarRW(bar, "fillColor", TW_TYPE_COLOR3F, &fillColor, " label='Fill' group='Color' ");
	TwDefine(" 'Main Configuration'/Color group=Draw ");
	
	glutMainLoop();

	// Terminate AntTweakBar
	TwTerminate();

	return 0;
}