#pragma once
#include <string>
#include "imgui.h"
#include "imgui_internal.h"

// Pointer to current selected shape on main app
shared_ptr<CShape> selectedShape;

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
	int selected = -1;                            
	bool currentMode = 0; // 0->Software, 1->Hardware
	bool rightClick = false;

	bool openBGPicker = false;
	bool openPopUpFlag = false;

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

	static void CustomPicker(ImVec4 color, bool &statusPicker)
	{
		
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

						if (ImGui::Selectable(shapeTypes[(3 * i) + j], selected == ((3 * i) + j), 0, ImVec2(w/2.3, 40.0f)))
							selected = (3 * i) + j;
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
			ImGui::Text("Send to front");
		}

		openBGPicker |= ImGui::ColorButton("##3b", ImVec4(bgColor[0], bgColor[1], bgColor[2], 1.0f), colorButtonFlags);

		if (openBGPicker) {
			ImGui::OpenPopup("mypicker");
		}
		if (ImGui::BeginPopup("mypicker"))
		{
			float w = ImGui::CalcItemWidth();
			ImGui::BeginGroup();
			ImGui::Text("Background Color");
			ImGui::SameLine();
			ImGui::SetCursorPosX(w - 10);
			if (ImGui::Button("x", ImVec2(20, 20)))
			{
				openBGPicker = false;
			}
			ImGui::Separator();
			ImGui::ColorPicker3("##picker", (float*)&bgColor, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
			ImGui::EndGroup();
			ImGui::EndPopup();
			openBGPicker = false;
		}

		ImGui::End();
		ImGui::ShowDemoWindow();
	}
};