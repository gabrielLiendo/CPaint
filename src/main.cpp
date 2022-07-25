#include <GL/freeglut.h>
#include "CPaintUI.h"

PaintUI ui;
int width = 1600, height = 800;
int oldx0, oldy0;
bool drawing = false;
bool overCtrlPoint = false;

void setViewport()
{
	// Set the viewport to be the entire new window
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	glOrtho(-0.5, width - 0.5,  - 0.5, height - 0.5, -1, 1);
}

void renderScene(void) 
{	
	float *bgColor = ui.bgColor;
	bool  currentMode = ui.currentMode;

	//Clear Frame
	glClearColor(bgColor[0], bgColor[1], bgColor[2], 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// Render every shape in canvas
	for (auto const& s : shapes)
		s->render(currentMode);

	if (selectedShape)
		selectedShape->drawCtrlStructure();

	if (drawingShape)
		drawingShape->render(currentMode);

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
	y = height - y - 1;
	// Manage clicked button (left, rigth, middle)
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if(state == GLUT_DOWN)
		{	// Left-click was pressed
			
			if (selectedShape && overCtrlPoint)
				selectedShape->clickedCtrlPoint(x, y);
			else if (hoveredShape) 
			{	// The click fell on top of a figure
				selectedShape = hoveredShape;
				selectedShape->setAnchorPoint(x, y);
				drawing = false;
			}
			else if (ui.shapeSelected > 3)
			{
				unselectFigure();
				drawing = true;
				if (!drawingShape)
					createFigure(ui.shapeSelected, oldx0, oldy0, x, y, ui.fillColor, ui.borderColor, ui.allowFill);
				else 
					drawingShape->newPoint(x, y);
			}
			else 
			{	// We clicked on free space
				unselectFigure();
				drawing = true;
				oldx0 = x; oldy0 = y;
			}
		}
		else 
		{	// Left-click was lifted
			if (drawingShape && drawingShape->finished())
			{
				saveFigure();
				drawing = false;
			}
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_UP && ui.shapeSelected == 5 && drawingShape)
		{
			drawingShape->forceFinish(x, y);
			saveFigure();
			drawing = false;
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
		y = height - y - 1;
		if (drawing)
		{
			if (!drawingShape)
				createFigure(ui.shapeSelected, oldx0, oldy0, x, y, ui.fillColor, ui.borderColor, ui.allowFill);
			else
				drawingShape->update(x, y);
		}
		else if (selectedShape) 
		{	// Drag shape position
			if (overCtrlPoint)
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

	y = height - y - 1;
	if (drawingShape)
	{	// We are drawing a Triangle or a Bezier curve
		drawingShape->update(x, y);
	}
	else
	{	// We are freely hovering over the canvas
		onHoverShape(x, y);
		if (selectedShape && selectedShape->hoveredCtrlPoint(x, y))
		{
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
			overCtrlPoint = true;
		}
		else
		{	
			if (selectedShape)
				selectedShape->release();
			overCtrlPoint = false;
			if (hoveredShape)
				glutSetCursor(GLUT_CURSOR_CYCLE);
			else
				glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
		}
	}
}

void onKeyboardEntry(unsigned char c, int x, int y)
{
	switch (c)
	{
		case '1':	if (!drawing) ui.shapeSelected = 0;						break;  // 1-6: Change shape to draw
		case '2':	if (!drawing) ui.shapeSelected = 1;						break;
		case '3':	if (!drawing) ui.shapeSelected = 2;						break;
		case '4':	if (!drawing) ui.shapeSelected = 3;						break;
		case '5':	if (!drawing) ui.shapeSelected = 4;						break;
		case '6':	if (!drawing) ui.shapeSelected = 5;						break;
		case 'h':	ui.currentMode = !ui.currentMode;			break;  // h: Change rendering mode
		case 'B':   ui.openBGPicker = !ui.openBGPicker;			break;  // B,C,F: Open color windows 
		case 'C':   ui.openBorderPicker = !ui.openBorderPicker; break;
		case 'F':   ui.openFillPicker = !ui.openFillPicker;		break;
		case 'L':   ui.loadScene();								break;
		case 'S':   ui.saveScene();								break;
		case 'H':	ui.openHelp = !ui.openHelp;					break;  // H: Open window with keyboard shortcuts
		case 'I':	ui.openMainWindow = !ui.openMainWindow;		break;	// I: Collapse/Open main interface window
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