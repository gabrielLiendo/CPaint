#pragma once
#include "imgui.h"
#include "imgui_internal.h"

#include <string>


// Pointer to current selected shape on main app
shared_ptr<CShape> selectedShape;
extern list<shared_ptr<CShape>> shapes;

class PaintUI
{
private:
	const char* renderingModes[2] = { "Software", "Hardware" };
	const char* shapeTypes[6] = { "Line", "Circle", "Ellipse", "Rectangle", "Triangle", "Bezier Curve" };

	// Color Palette
	bool saved_palette_init = true;
	ImVec4 saved_palette[28];

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
	bool rightClick = false;

	bool openBGPicker = false;
	bool openFillPicker = false;
	bool openBorderPicker = false;

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

	void drawPalette()
	{
		if (saved_palette_init)
		{
			saved_palette[0] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			saved_palette[1] = ImVec4(0.75f, 0.75f, 0.75f, 0.75f);

			for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
			{
				ImGui::ColorConvertHSVtoRGB(n / 27.0f, 0.8f, 0.8f, saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
				saved_palette[n].w = 1.0f; 
			}
			saved_palette_init = false;
		}

		ImGui::BeginGroup();
		for (int n = 0; n < 28; n++)
		{
			if ((n % 14) != 0)
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

	void drawUI()
	{	
		ImGui::Begin("Configuration", 0, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Do stuff */ }
				if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// Rendering Mode Toggler
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
			if (ImGui::Button("<<", ImVec2(20, 20)))
				toggleLevel(-2);
			ImGui::SameLine();
			if (ImGui::Button("-", ImVec2(20, 20)))
				toggleLevel(-1);
			ImGui::SameLine();
			if (ImGui::Button("+", ImVec2(20, 20)))
				toggleLevel(1);
			ImGui::SameLine();
			if (ImGui::Button(">>", ImVec2(20, 20)))
				toggleLevel(2);
		}

		// Popup Windows
		ImGui::SetNextWindowSize(ImVec2(200, 270));
		if (openBGPicker && ImGui::Begin("Background Color", &openBGPicker, ImGuiWindowFlags_NoResize))
		{
			float w = ImGui::CalcItemWidth();
			ImGui::PushItemWidth(1.35 * w);
			ImGui::ColorPicker3("##picker", (float*)&bgColor, ImGuiColorEditFlags_NoSidePreview);
			ImGui::End();
		}

		ImGui::SetNextWindowSize(ImVec2(200, 270));
		if (openFillPicker && ImGui::Begin("Fill Color", &openFillPicker, ImGuiWindowFlags_NoResize))
		{	
			float w = ImGui::CalcItemWidth();
			ImGui::PushItemWidth(1.35*w);
			if (ImGui::ColorPicker3("##picker", (float*)&fillColor, ImGuiColorEditFlags_NoSidePreview))
			{
				if (selectedShape)
					selectedShape->setFillColor(fillColor[0], fillColor[1], fillColor[2]);
			}
			ImGui::End();
		}

		ImGui::SetNextWindowSize(ImVec2(200, 270));
		if (openBorderPicker && ImGui::Begin("Border Color", &openBorderPicker, ImGuiWindowFlags_NoResize))
		{
			float w = ImGui::CalcItemWidth();
			ImGui::PushItemWidth(1.35 * w);
			if (ImGui::ColorPicker3("##picker", (float*)&borderColor, ImGuiColorEditFlags_NoSidePreview))
			{
				if (selectedShape)
					selectedShape->setBorderColor(borderColor[0], borderColor[1], borderColor[2]);
			}
			ImGui::End();
		}

		ImGui::End();
		//ImGui::ShowDemoWindow();
	}
};