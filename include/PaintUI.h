#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "tinyfiledialogs.h"

#include <string>

//bin\$(Platform)$(Configuration)\

#ifdef _MSC_VER
#pragma warning(disable:4996) // Silences warnings about fopen
#endif

using namespace std;

// Pointer to current selected shape on main app
shared_ptr<CShape> hoveredShape;
shared_ptr<CShape> selectedShape;
extern list<shared_ptr<CShape>> shapes;

bool isHigherLevel(shared_ptr<CShape> fig, shared_ptr<CShape> figure)
{
	return figure->getLayerLevel() > fig->getLayerLevel();
}

void deleteFigure()
{
	if (selectedShape)
		shapes.remove(selectedShape);
	selectedShape = nullptr;
}

void deleteAllFigures()
{
	shapes.clear();
	selectedShape = nullptr;
}

class PaintUI
{
private:
	// Const char strings to display
	const char* renderingModes[2] = { "Software", "Hardware" };
	const char* shapeTypes[6] = { "Line", "Circle", "Ellipse", "Rectangle", "Triangle", "Bezier Curve" };

	// Color Palette
	ImVec4 saved_palette[26];

	// UI Widgets flags
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar;
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen;
	ImGuiButtonFlags colorButtonFlags = ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight;
public:
	// Colors
	float bgColor[3] = { 0.386f, 0.513f, 0.559f };
	float fillColor[3] = { 1.0f, 1.0f, 1.0f };
	float borderColor[3] = { 0.0f, 0.0f, 0.0f };
	
	// Current State variables
	int shapeSelected; 
	bool currentMode, allowFill;
	bool openBGPicker, openFillPicker, openBorderPicker, openHelp;

	PaintUI() 
	{
		// Initialize State variables
		shapeSelected = -1;
		currentMode = false;
		allowFill = true;
		openBGPicker = false;
		openFillPicker = false;
		openBorderPicker = false;
		openHelp = false;

		// Initialize Color Palette
		saved_palette[0] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);		saved_palette[13] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		saved_palette[1] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);		saved_palette[14] = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
		saved_palette[2] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);		saved_palette[15] = ImVec4(1.0f, 0.7f, 0.4f, 1.0f);
		saved_palette[3] = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);		saved_palette[16] = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
		saved_palette[4] = ImVec4(0.5f, 1.0f, 0.0f, 1.0f);		saved_palette[17] = ImVec4(0.7f, 1.0f, 0.4f, 1.0f);
		saved_palette[5] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);		saved_palette[18] = ImVec4(0.4f, 1.0f, 0.4f, 1.0f);
		saved_palette[6] = ImVec4(0.0f, 1.0f, 0.5f, 1.0f);		saved_palette[19] = ImVec4(0.4f, 1.0f, 0.7f, 1.0f);
		saved_palette[7] = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);		saved_palette[20] = ImVec4(0.4f, 1.0f, 1.0f, 1.0f);
		saved_palette[8] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);		saved_palette[21] = ImVec4(0.4f, 0.7f, 1.0f, 1.0f);
		saved_palette[9] = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);		saved_palette[22] = ImVec4(0.4f, 0.4f, 1.0f, 1.0f);
		saved_palette[10] = ImVec4(0.5f, 0.0f, 1.0f, 1.0f);		saved_palette[23] = ImVec4(0.7f, 0.4f, 1.0f, 1.0f);
		saved_palette[11] = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);		saved_palette[24] = ImVec4(1.0f, 0.4f, 1.0f, 1.0f);
		saved_palette[12] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);		saved_palette[25] = ImVec4(0.75f, 0.75f, 0.75f, 1.0f);
	}

	static void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void init()
	{
		// Setup Dear ImGui context and style of window
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Setup Style
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
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
	}

	void renderWindow()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplGLUT_NewFrame();

		drawUI();

		// Render GUI
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	}

	void toggleLevel(int option)
	{
		if (selectedShape)
		{
			int level = selectedShape->getLayerLevel();
			switch (option)
			{
			case -2:
				level = (shapes.front()->getLayerLevel()) - 1;
				break;
			case -1: 
				level -= 1;
				break;
			case 1:
				level += 1;
				break;
			case 2:
				level = (shapes.back()->getLayerLevel()) + 1;
				break;
			default:
				break;
			}
			selectedShape->setLayerLevel(level);
			shapes.sort([](const shared_ptr<CShape>& a, const shared_ptr<CShape>& b)
				{ return a->getLayerLevel() < b->getLayerLevel(); });
		}
	}

	void drawPalette()
	{
		ImGui::BeginGroup();
		for (int n = 0; n < 26; n++)
		{
			if ((n % 13) != 0)
				ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);

			ImGui::PushID(n);
			ImGui::PushStyleColor(ImGuiCol_Button, saved_palette[n]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, saved_palette[n]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, saved_palette[n]);
			if (ImGui::ButtonEx("##colorButton", ImVec2(20, 20), colorButtonFlags))
			{
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					fillColor[0] = saved_palette[n].x; fillColor[1] = saved_palette[n].y; fillColor[2] = saved_palette[n].z;
					if (selectedShape)
						selectedShape->setFillColor(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
				}
				else
				{
					borderColor[0] = saved_palette[n].x; borderColor[1] = saved_palette[n].y; borderColor[2] = saved_palette[n].z;
					if (selectedShape)
						selectedShape->setBorderColor(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
				}
			}
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}
		ImGui::EndGroup();
	}

	int readColorValues(char **token, float *color)
	{
		int count = 0;
		float c = 0.0f;

		for (int i = 0; i < 3; i++)
		{
			if (!(*token))
				return false;

			c = atof(*token);

			if (c < 0.0f || c > 1.0f)
				return 4;
			else
				count += 1;

			color[i] = c;
			*token = strtok(NULL, " \t");
		}
		return count;
	}

	bool readFigure(char **token, int n, int *pointValues, float *borderColor, float *fillColor, bool filled)
	{
		int count = 0, value;
		*token = strtok(NULL, " \t");

		// Read n integer values for points positions 
		for (int i = 0; i < n; i++)
		{
			if (!(*token))
				return false;

			value = atoi(*token);
			if (value < 0)
				return false;
			else
				count += 1;
			pointValues[i] = value;
			*token = strtok(NULL, " \t");
		}

		if (count != n || readColorValues(&(*token), borderColor) != 3)
			return false;

		if (filled && readColorValues(&(*token), fillColor) != 3)
			return false;

		return (*token == NULL);
	}

	void loadScene()
	{
		// File Variables
		char const *lTheOpenFileName, *lFilterPatterns[2] = { "*.txt", "*.text" };
		FILE* lIn;
		char lBuffer[1024];

		// Read Figures Variables 
		char* token;
		int  filePValues[60];
		float fileBGColor[3], fileBColor[3], fileFColor[3];
		bool filled = false;

		lTheOpenFileName = tinyfd_openFileDialog("Open", "", 2, lFilterPatterns, NULL, 0);

		if (!lTheOpenFileName)
			return;

		deleteAllFigures();

	#ifdef _WIN32
		if (tinyfd_winUtf8)
			lIn = _wfopen(tinyfd_utf8to16(lTheOpenFileName), L"r"); // the UTF-8 filename is converted to UTF-16
		else
	#endif
			lIn = fopen(lTheOpenFileName, "r");

		if (!lIn)
		{
			tinyfd_messageBox("Error", "Can not open this file in read mode", "Ok", "error", 1);
			return;
		}

		// Get Background Color
		fgets(lBuffer, sizeof(lBuffer), lIn);
		token = strtok(lBuffer, " \t");
		if (strcmp(token, "BACKGROUND") == 0)
		{
			int count = 0;
			token = strtok(NULL, " \t");
			count += readColorValues(&token, fileBGColor);

			if (count != 3)
				cout << "Invalid Background Color" << endl;
			else
			{
				bgColor[0] = fileBGColor[0]; 
				bgColor[1] = fileBGColor[1]; 
				bgColor[2] = fileBGColor[2];
			}
		}

		// Get Figures
		while (fgets(lBuffer, sizeof(lBuffer), lIn) != NULL)
		{
			filled = false;
			token = strtok(lBuffer, " \t");
			if (strncmp(token, "FILLED_", 7) == 0) {
				filled = true;
				token += 7;
			}

			if (strcmp(token, "LINE") == 0)
			{
				if (readFigure(&token, 4, filePValues, fileBColor, fileFColor, filled))
				{
					shared_ptr<CLine> l = make_shared<CLine>(filePValues[0], filePValues[1], filePValues[2], filePValues[3],
						fileBColor[0], fileBColor[1], fileBColor[2]);

					shapes.push_back(l);
				}
				else
					cout << "Invalid Line" << endl;	
			}
			else if (strcmp(token, "TRIANGLE") == 0)
			{
				if (readFigure(&token, 6, filePValues, fileBColor, fileFColor, filled))
				{
					shared_ptr<CTriangle> t = make_shared<CTriangle>(filePValues[0], filePValues[1], filePValues[2], filePValues[3],
						filePValues[4], filePValues[5], fileFColor[0], fileFColor[1], fileFColor[2], fileBColor[0], fileBColor[1],
						fileBColor[2], filled);

					shapes.push_back(t);
				}	
				else
					cout << "Invalid Triangle" << endl;
			}
			else if (strcmp(token, "RECTANGLE") == 0)
			{
				if (readFigure(&token, 4, filePValues, fileBColor, fileFColor, filled))
				{
					shared_ptr<CRectangle> r = make_shared<CRectangle>(filePValues[0], filePValues[1], filePValues[2], filePValues[3],
						fileFColor[0], fileFColor[1], fileFColor[2], fileBColor[0], fileBColor[1], fileBColor[2], filled);
					shapes.push_back(r);
				}
				else
					cout << "Invalid Rectangle" << endl;
			}
			else if (strcmp(token, "ELLIPSE") == 0)
			{
				if (readFigure(&token, 4, filePValues, fileBColor, fileFColor, filled))
				{
					shared_ptr<CEllipse> e = make_shared<CEllipse>(filePValues[0], filePValues[1], filePValues[2], filePValues[3],
						fileFColor[0], fileFColor[1], fileFColor[2], fileBColor[0], fileBColor[1], fileBColor[2], filled);
					shapes.push_back(e);
				}
				else
					cout << "Invalid Ellipse" << endl;
			}
			else if (strcmp(token, "CIRCLE") == 0)
			{
				if (readFigure(&token, 4, filePValues, fileBColor, fileFColor, filled))
				{
					shared_ptr<CCircle> c = make_shared<CCircle>(filePValues[0], filePValues[1], filePValues[2], filePValues[3],
						fileFColor[0], fileFColor[1], fileFColor[2], fileBColor[0], fileBColor[1], fileBColor[2], filled, true);
					shapes.push_back(c);
				}
				else
					cout << "Invalid Circle" << endl;
			}
			else if (strncmp(token, "BEZIER", 5) == 0)
			{	
				int n = atoi(token + 6);
				if (readFigure(&token, n*2, filePValues, fileBColor, fileFColor, filled))
				{
					shared_ptr<CBezier> b = make_shared<CBezier>(filePValues, n, fileBColor[0], fileBColor[1], fileBColor[2]);
					shapes.push_back(b);
				}
				else
					cout << "Invalid Bezier Curve" << endl;
			}
			else 
				cout << "Non existing Figure" << endl;
		}

		fclose(lIn);
	}

	void saveScene()
	{
		char const* lTheSaveFileName;
		char const* lFilterPatterns[2] = { "*.txt", "*.text" };
		FILE* lIn;

	#ifdef _WIN32
		tinyfd_winUtf8 = 1;
	#endif

		lTheSaveFileName = tinyfd_saveFileDialog("Save As", "Scene.txt", 2, lFilterPatterns, nullptr);

		if (!lTheSaveFileName)
			return;

	#ifdef _WIN32
		if (tinyfd_winUtf8)
			lIn = _wfopen(tinyfd_utf8to16(lTheSaveFileName), L"w");
		else
	#endif
			lIn = fopen(lTheSaveFileName, "w");

		if (!lIn)
		{
			tinyfd_messageBox("Error", "Can not open this file in write mode", "Ok", "Error", 1);
			return;
		}

		std::string background = "BACKGROUND " + std::to_string(bgColor[0]) + " " 
			+ std::to_string(bgColor[1]) + " " + std::to_string(bgColor[2]) + "\n";

		fputs(background.c_str(), lIn);

		for (auto const& s : shapes)
		{
			string info = s->getInfo();
			fputs(info.c_str(), lIn);
		}
			
		fclose(lIn);
	}

	void drawUI()
	{	
		ImGui::Begin("Configuration", 0, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load Scene", "L"))
					loadScene();
				if (ImGui::MenuItem("Save Scene", "S"))
					saveScene();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Shortcuts", " "))
					openHelp = true;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGuiStyle& style = ImGui::GetStyle();

			float w = ImGui::CalcItemWidth();
			float spacing = style.ItemInnerSpacing.x;
			float button_sz = ImGui::GetFrameHeight();

			ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
			if (ImGui::BeginCombo("##rendering mode", renderingModes[currentMode], ImGuiComboFlags_NoArrowButton))
			{
				for (int n = 0; n < 2; n++)
				{
					bool is_selected = (currentMode == n);
					if (ImGui::Selectable(renderingModes[n], is_selected))
						currentMode = renderingModes[n];
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			ImGui::SameLine(0, spacing);
			if (ImGui::ArrowButton("##l", ImGuiDir_Left))
				currentMode = !currentMode;

			ImGui::SameLine(0, spacing);
			if (ImGui::ArrowButton("##r", ImGuiDir_Right))
				currentMode = !currentMode;

			ImGui::SameLine(0, style.ItemInnerSpacing.x);
			ImGui::Text("Rendering Mode");

			ImGui::ColorEdit3("Background Color", bgColor);
		}

		if (ImGui::CollapsingHeader("Draw Figure", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::TreeNodeEx("Choose Shape", nodeFlags))
			{

				ImGui::TreePush();
				float w = ImGui::CalcItemWidth();
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < 3; j++)
					{
						if (j > 0)
							ImGui::SameLine();

						if (ImGui::Selectable(shapeTypes[(3 * i) + j], shapeSelected == ((3 * i) + j), 0, ImVec2(w/ (float)2.3, 40.0f)))
							shapeSelected = (3 * i) + j;
					}
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("Edit Colors", nodeFlags))
			{
				ImGui::SameLine();
				HelpMarker("Left-click on palette \nto change the fill color. \n\nRight-click on palette \nto change the border color.");

				ImGui::TreePush();

				if (selectedShape)
					allowFill = selectedShape->getFillBool();

				if (ImGui::Checkbox("Draw Filler", &allowFill) && selectedShape)
				{
					selectedShape->setFilled(allowFill);
					selectedShape->setFillColor(fillColor[0], fillColor[1], fillColor[2]);
				}

				if (ImGui::ColorEdit3("Fill Color", fillColor) && selectedShape)
					selectedShape->setFillColor(fillColor[0], fillColor[1], fillColor[2]);

				if (ImGui::ColorEdit3("Border Color", borderColor) && selectedShape)
					selectedShape->setBorderColor(borderColor[0], borderColor[1], borderColor[2]);
				
				/*if (selectedShape)
				{
					float* bColor = selectedShape->getBorderColor();
					float* fColor = selectedShape->getFillColor();
					borderColor[0] = bColor[0]; borderColor[1] = bColor[1]; borderColor[2] = bColor[2];
					fillColor[0] = fColor[0]; fillColor[1] = fColor[1]; fillColor[2] = fColor[2];
				}*/

				ImGui::Separator();
				ImGui::Text("Default Palette:");
				drawPalette();

				ImGui::TreePop();
			}
		}

		if (ImGui::CollapsingHeader("Tools", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::TreeNodeEx("Control Layer", nodeFlags))
			{
				ImGui::TreePush();

				ImGui::Text("Back");
				ImGui::SameLine();
				if (ImGui::Button("<<", ImVec2(20, 20)))
					toggleLevel(-2);
				ImGui::SameLine();

				ImGui::Text("Down");
				ImGui::SameLine();
				if (ImGui::Button("-", ImVec2(20, 20)))
					toggleLevel(-1);
				ImGui::SameLine();

				ImGui::Text("Up");
				ImGui::SameLine();
				if (ImGui::Button("+", ImVec2(20, 20)))
					toggleLevel(1);
				ImGui::SameLine();

				ImGui::Text("Front");
				ImGui::SameLine();
				if (ImGui::Button(">>", ImVec2(20, 20)))
					toggleLevel(2);

				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("Delete", nodeFlags))
			{
				ImGui::TreePush();
				if (ImGui::Button("Selected Figure", ImVec2(100, 20)))
					deleteFigure();
				ImGui::SameLine();

				if (ImGui::Button("All Figures", ImVec2(100, 20)))
					deleteAllFigures();
				ImGui::TreePop();
			}
		}

		// Popup Windows
		if (openBGPicker && ImGui::Begin("Background Color", &openBGPicker, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
		{
			float w = ImGui::CalcItemWidth();
			ImGui::PushItemWidth((float)1.35 * w);
			ImGui::ColorPicker3("##picker", (float*)&bgColor, ImGuiColorEditFlags_NoSidePreview);
			ImGui::PopItemWidth();
			ImGui::End();
		}


		if (openFillPicker && ImGui::Begin("Fill Color", &openFillPicker, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
		{	
			float w = ImGui::CalcItemWidth();
			ImGui::PushItemWidth((float)1.35 *w);
			if (ImGui::ColorPicker3("##picker", (float*)&fillColor, ImGuiColorEditFlags_NoSidePreview))
			{
				if (selectedShape)
					selectedShape->setFillColor(fillColor[0], fillColor[1], fillColor[2]);
			}
			ImGui::PopItemWidth();
			ImGui::End();
		}


		if (openBorderPicker && ImGui::Begin("Border Color", &openBorderPicker, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
		{
			float w = ImGui::CalcItemWidth();
			ImGui::PushItemWidth((float)1.35 * w);
			if (ImGui::ColorPicker3("##picker", (float*)&borderColor, ImGuiColorEditFlags_NoSidePreview))
			{
				if (selectedShape)
					selectedShape->setBorderColor(borderColor[0], borderColor[1], borderColor[2]);
			}
			ImGui::PopItemWidth();
			ImGui::End();
		}

		if (openHelp && ImGui::Begin("Help", &openHelp, ImGuiWindowFlags_None))
		{
			ImGui::Text("Keyboard shortcuts list.");
			ImGui::Separator();
			ImGui::Text("Scene");
			ImGui::BulletText("h : change rendering mode.");
			ImGui::BulletText("u : unselect current figure");
			ImGui::BulletText("Backspace: delete current figure.");
			ImGui::BulletText("x : delete all figures.");
			ImGui::Text("Files");
			ImGui::BulletText("L : load scene from text file.");
			ImGui::BulletText("S : save scene to text file.");
			ImGui::Text("Shapes");
			ImGui::BulletText("1: select line.");
			ImGui::BulletText("2: select circle.");
			ImGui::BulletText("3: select ellipse.");
			ImGui::BulletText("4: select rectangle.");
			ImGui::BulletText("5: select triangle.");
			ImGui::BulletText("6 : select bezier curve");
			ImGui::Text("Colors");
			ImGui::BulletText("B : change background color.");
			ImGui::BulletText("F : change filler color.");
			ImGui::BulletText("C : change border color.");
			ImGui::Text("Control layer");
			ImGui::BulletText("- : push figure one layer below.");
			ImGui::BulletText("+ : bring figure one layer forward.");
			ImGui::BulletText("b : push figure to the lowest layer.");
			ImGui::BulletText("f : bring figure to the highest layer.");
			ImGui::End();
		}

		ImGui::End();
		ImGui::ShowDemoWindow();
	}

	void close()
	{
		ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplGLUT_Shutdown();
		ImGui::DestroyContext();
	}
};