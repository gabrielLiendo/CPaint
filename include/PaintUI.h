#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "tinyfiledialogs.h"

#include <string>

#ifdef _MSC_VER
#pragma warning(disable:4996) // Silences warnings about strcpy strcat fopen
#endif

using namespace std;

// Pointer to current selected shape on main app
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
	const char* renderingModes[2] = { "Software", "Hardware" };
	const char* shapeTypes[6] = { "Line", "Circle", "Ellipse", "Rectangle", "Triangle", "Bezier Curve" };

	// Color Palette
	bool saved_palette_init = true;
	ImVec4 saved_palette[26];

	// Color test
	ImVec4 color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 backup_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

	// UI Widgets flags
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar;
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen;
	ImGuiButtonFlags colorButtonFlags = ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight;
public:
	// Colors
	float bgColor[3] = { 0.386f, 0.513f, 0.559f };
	float fillColor[3] = { 1.0f, 1.0f, 1.0f };
	float borderColor[3] = { 0.0f, 0.0f, 0.0f };
	
	// Current State
	int shapeSelected = -1; 
	bool currentMode = 0; // 0->Software, 1->Hardware
	bool allowFill = 1;

	bool openBGPicker = false;
	bool openFillPicker = false;
	bool openBorderPicker = false;
	bool openHelp = false;
	bool openDeleteModal = false;

	PaintUI() { ; }

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

	void initPalette()
	{
		saved_palette[0] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); 
		saved_palette[1] = ImVec4(0.75f, 0.75f, 0.75f, 1.0f);
		saved_palette[2] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		saved_palette[3] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
		saved_palette[4] = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
		saved_palette[5] = ImVec4(0.5f, 1.0f, 0.0f, 1.0f);
		saved_palette[6] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
		saved_palette[7] = ImVec4(0.0f, 1.0f, 0.5f, 1.0f);
		saved_palette[8] = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
		saved_palette[9] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);
	}

	void drawPalette()
	{
		if (saved_palette_init)
		{
			
			saved_palette[0] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			saved_palette[1] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
			saved_palette[2] = ImVec4(0.8f, 0.0f, 0.0f, 1.0f);
			saved_palette[3] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
			saved_palette[4] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);

			saved_palette[6] = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);

			saved_palette[5] = ImVec4(1.0f, 0.7f, 0.4f, 1.0f);
			saved_palette[7] = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

			
			
			saved_palette[8] = ImVec4(0.5f, 1.0f, 0.0f, 1.0f);
			saved_palette[9] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);

			saved_palette[10] = ImVec4(0.0f, 0.4f, 0.0f, 1.0f);
			saved_palette[11] = ImVec4(0.0f, 1.0f, 0.5f, 1.0f);
			
			saved_palette[13] = ImVec4(0.5f, 1.0f, 1.0f, 1.0f);

			saved_palette[12] = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
			saved_palette[14] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);
			saved_palette[15] = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
			saved_palette[16] = ImVec4(0.5f, 0.0f, 1.0f, 1.0f);
			saved_palette[17] = ImVec4(0.7f, 0.4f, 1.0f, 1.0f);
			saved_palette[18] = ImVec4(1.0f, 0.4f, 0.7f, 1.0f);
			saved_palette[19] = ImVec4(1.0f, 0.0f, 0.5f, 1.0f);
			
			saved_palette_init = false;
		}

		ImGui::BeginGroup();
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			if ((n % 10) != 0)
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

	int readPointsValues(char **token, int n, int *pValues)
	{
		int count = 0, value = 0;

		
		return count;
	}

	int readColorValues(char **token, float *color)
	{
		int count = 0;
		float c = 0.0f;

		for (int i = 0; i < 3; i++)
		{
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
			value = atoi(*token);
			if (value < 0)
				return false;
			else
				count += 1;
			pointValues[i] = value;
			*token = strtok(NULL, " \t");
		}

		count += readColorValues(&(*token), borderColor);
		if(filled)
			count += readColorValues(&(*token), fillColor);

		if (count != n + 3 * (1 + filled))
			return false;

		return true;
	}

	void loadScene()
	{
		deleteAllFigures();

		// File Variables
		char const* lTheOpenFileName;
		char const* lFilterPatterns[2] = { "*.txt", "*.text" };
		FILE* lIn;
		char lBuffer[1024];
		// Read Figures Variables 
		char* token;
		int  filePValues[6];
		float color, fileBGColor[3], fileBColor[3], fileFColor[3];

		lTheOpenFileName = tinyfd_openFileDialog("Open", "", 2, lFilterPatterns, NULL, 0);

		if (!lTheOpenFileName)
			return;

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
				bgColor[0] = fileBGColor[0]; bgColor[1] = fileBGColor[1]; bgColor[2] = fileBGColor[2];
			}
		}

		// Get Figures
		while (fgets(lBuffer, sizeof(lBuffer), lIn) != NULL) {
			token = strtok(lBuffer, " \t");
			if (strcmp(token, "LINE") == 0)
			{
				if (readFigure(&token, 4, filePValues, fileBColor, fileFColor, false))
				{
					shared_ptr<CLine> l = make_shared<CLine>(filePValues[0], filePValues[1], filePValues[2], filePValues[3],
						fileBColor[0], fileBColor[1], fileBColor[2], fileBColor[0], fileBColor[1], fileBColor[2], false);

					shapes.insert(std::upper_bound(shapes.begin(), shapes.end(), l, isHigherLevel), l);
				}
				else
					cout << "Invalid Line" << endl;	
			}
			else if (strcmp(token, "TRIANGLE") == 0)
			{
				if (readFigure(&token, 6, filePValues, fileBColor, fileFColor, false))
				{
					shared_ptr<CTriangle> t = make_shared<CTriangle>(filePValues[0], filePValues[1], filePValues[2], filePValues[3],
						filePValues[4], filePValues[5], fileBColor[0], fileBColor[1], fileBColor[2], fileBColor[0], fileBColor[1],
						fileBColor[2], false);

					shapes.insert(std::upper_bound(shapes.begin(), shapes.end(), t, isHigherLevel), t);
				}	
				else
					cout << "Invalid Triangle" << endl;
			}
			else if (strcmp(token, "FILLED_TRIANGLE") == 0)
			{
				if (readFigure(&token, 6, filePValues, fileBColor, fileFColor, true))
				{
					shared_ptr<CTriangle> t = make_shared<CTriangle>(filePValues[0], filePValues[1], filePValues[2], filePValues[3],
						filePValues[4], filePValues[5], fileFColor[0], fileFColor[1], fileFColor[2], fileBColor[0], fileBColor[1],
						fileBColor[2], true);
			
					shapes.insert(std::upper_bound(shapes.begin(), shapes.end(), t, isHigherLevel), t);
				}
				else
					cout << "Invalid Filled Triangle" << endl;
			}
			else if (strcmp(token, "RECTANGLE") == 0)
			{
				if (readFigure(&token, 4, filePValues, fileBColor, fileFColor, false))
				{
					shared_ptr<CRectangle> r = make_shared<CRectangle>(filePValues[0], filePValues[1], filePValues[2], filePValues[3],
						fileBColor[0], fileBColor[1], fileBColor[2], fileBColor[0], fileBColor[1], fileBColor[2], false);

					shapes.insert(std::upper_bound(shapes.begin(), shapes.end(), r, isHigherLevel), r);
				}
				else
					cout << "Invalid Rectangle" << endl;
			}
			else if (strcmp(token, "FILLED_RECTANGLE") == 0)
			{
				if (readFigure(&token, 4, filePValues, fileBColor, fileFColor, true))
				{
					shared_ptr<CRectangle> r = make_shared<CRectangle>(filePValues[0], filePValues[1], filePValues[2], filePValues[3],
						fileFColor[0], fileFColor[1], fileFColor[2], fileBColor[0], fileBColor[1], fileBColor[2], true);

					shapes.insert(std::upper_bound(shapes.begin(), shapes.end(), r, isHigherLevel), r);
				}			
				else
					cout << "Invalid Filled Rectangle" << endl;
			}
			else if (strcmp(token, "ELLIPSE") == 0)
			{
				if (readFigure(&token, 4, filePValues, fileBColor, fileFColor, false))
				{
					shared_ptr<CEllipse> e = make_shared<CEllipse>(filePValues[0], filePValues[1], filePValues[2], filePValues[3],
						fileBColor[0], fileBColor[1], fileBColor[2], fileBColor[0], fileBColor[1], fileBColor[2], false);

					shapes.insert(std::upper_bound(shapes.begin(), shapes.end(), e, isHigherLevel), e);
				}
				else
					cout << "Invalid Ellipse" << endl;
			}
			else if (strcmp(token, "FILLED_ELLIPSE") == 0)
			{
				if (readFigure(&token, 4, filePValues, fileBColor, fileFColor, true))
				{
					shared_ptr<CEllipse> e = make_shared<CEllipse>(filePValues[0], filePValues[1], filePValues[2], filePValues[3],
						fileFColor[0], fileFColor[1], fileFColor[2], fileBColor[0], fileBColor[1], fileBColor[2], true);

					shapes.insert(std::upper_bound(shapes.begin(), shapes.end(), e, isHigherLevel), e);
				}
				else
					cout << "Invalid Filled Ellipse" << endl;
			}
			else if (strcmp(token, "CIRCLE") == 0)
			{
				if (readFigure(&token, 4, filePValues, fileBColor, fileFColor, false))
				{
					shared_ptr<CCircle> c = make_shared<CCircle>(filePValues[0], filePValues[1], filePValues[2], filePValues[3],
						fileBColor[0], fileBColor[1], fileBColor[2], fileBColor[0], fileBColor[1], fileBColor[2], false, true);

					shapes.insert(std::upper_bound(shapes.begin(), shapes.end(), c, isHigherLevel), c);
				}
				else
					cout << "Invalid Circle" << endl;
			}
			else if (strcmp(token, "FILLED_CIRCLE") == 0)
			{
				if (readFigure(&token, 4, filePValues, fileBColor, fileFColor, true))
				{
					shared_ptr<CCircle> c = make_shared<CCircle>(filePValues[0], filePValues[1], filePValues[2], filePValues[3],
						fileFColor[0], fileFColor[1], fileFColor[2], fileBColor[0], fileBColor[1], fileBColor[2], true, true);

					shapes.insert(std::upper_bound(shapes.begin(), shapes.end(), c, isHigherLevel), c);
				}
				else
					cout << "Invalid Filled Circle" << endl;
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
		char lBuffer[1024];

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

		for (auto const& s : shapes) {
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
			// Rendering Mode Toggler
			//ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();

			//auto boldFont = io.Fonts->Fonts[0];

			float w = ImGui::CalcItemWidth();
			float spacing = style.ItemInnerSpacing.x;
			float button_sz = ImGui::GetFrameHeight();

			ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
			if (ImGui::BeginCombo("##rendering mode", renderingModes[currentMode], ImGuiComboFlags_NoArrowButton))
			{
				//ImGui::PushFont(boldFont);
				for (int n = 0; n < 2; n++)
				{
					bool is_selected = (currentMode == n);
					if (ImGui::Selectable(renderingModes[n], is_selected))
						currentMode = renderingModes[n];
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				//ImGui::PopFont();
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

						if (ImGui::Selectable(shapeTypes[(3 * i) + j], shapeSelected == ((3 * i) + j), 0, ImVec2(w/2.3, 40.0f)))
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
					
				if (ImGui::ColorEdit3("Fill Color", fillColor))
				{
					if (selectedShape)
						selectedShape->setFillColor(fillColor[0], fillColor[1], fillColor[2]);
				}
				if (ImGui::ColorEdit3("Border Color", borderColor))
				{
					if (selectedShape)
						selectedShape->setBorderColor(borderColor[0], borderColor[1], borderColor[2]);
				}
				
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
				if (ImGui::Button("Current Figure", ImVec2(100, 20)))
				deleteFigure();
				ImGui::SameLine();

				if (ImGui::Button("All Figures", ImVec2(100, 20)) || openDeleteModal)
				{
					openDeleteModal = true;
					ImGui::OpenPopup("Delete All Figures?");
				}
				ImGui::SameLine();
			}
		}

		// Popup Windows
		if (openBGPicker && ImGui::Begin("Background Color", &openBGPicker, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
		{
			float w = ImGui::CalcItemWidth();
			ImGui::PushItemWidth(1.35 * w);
			ImGui::ColorPicker3("##picker", (float*)&bgColor, ImGuiColorEditFlags_NoSidePreview);
			ImGui::PopItemWidth();
			ImGui::End();
		}


		if (openFillPicker && ImGui::Begin("Fill Color", &openFillPicker, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
		{	
			float w = ImGui::CalcItemWidth();
			ImGui::PushItemWidth(1.35*w);
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
			ImGui::PushItemWidth(1.35 * w);
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

		if (ImGui::BeginPopupModal("Delete All Figures?", &openDeleteModal, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("All the figures in the scene will be deleted \nThis operation cannot be undone!\n\n");
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0))) 
			{
				deleteAllFigures(); 
				openDeleteModal = false;
				ImGui::CloseCurrentPopup(); 
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { openDeleteModal = false;  ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}
		ImGui::End();
		ImGui::ShowDemoWindow();
	}
};