#pragma once
#include <string>
#include "imgui.h"

// Pointer to current selected shape on main app
shared_ptr<CShape> selectedShape;

class PaintUI
{
private:
	const char* renderingModes[2] = { "Software", "Hardware" };
	const char* shapeTypes[6] = { "Line", "Circle", "Ellipse", "Rectangle", "Triangle", "Bezier\n Curve" };

public:
	// Colors
	float bgColor[3] = { 0.887f, 0.322f, 0.322f };
	float fillColor[3] = { 0.0f, 0.0f, 0.0f };
	float borderColor[3] = { 0.0f, 0.0f, 0.0f };
	
	// Current State
	int selected = -1;                            
	bool currentMode = 0; // 0->Software, 1->Hardware
	bool rightClick = false;

	PaintUI() { ; }

	void switchMode() {
		currentMode |= 0;
	}

	void drawPalette(float* bindedColor, const char *id)
	{
		static ImVec4 saved_palette[28] = {};
		static bool saved_palette_init = true;

		if (saved_palette_init)
		{
			saved_palette[0] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			saved_palette[1] = ImVec4(0.75f, 0.75f, 0.75f, 0.75f);

			for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
			{
				ImGui::ColorConvertHSVtoRGB(n / 27.0f, 0.8f, 0.8f, saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
			}
			saved_palette_init = false;
		}

		ImGui::BeginGroup();

		ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;

		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			ImVec2 cursorPos = ImGui::GetCursorPos();

			ImGui::PushID(n);

			if ((n % 14) != 0)
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

			if (ImGui::ColorButton(id, saved_palette[n], palette_button_flags, ImVec2(20, 20)))
			{
				bindedColor[0] = saved_palette[n].x;
				bindedColor[1] = saved_palette[n].y;
				bindedColor[2] = saved_palette[n].z;

				if (selectedShape)
					selectedShape->setFillColor(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
			}

			// Allow user to drop colors into each palette entry. 
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
					memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
					memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
				ImGui::EndDragDropTarget();
			}

			ImGui::PopID();
		}
		ImGui::EndGroup();
	}

	void drawUI()
	{	
		//Set UI flags
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar;
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen;

		ImGui::Begin("Configuration");

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

		if (ImGui::CollapsingHeader("Draw", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::TreeNodeEx("Shape", node_flags))
			{
				ImGui::TreePush();

				
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < 3; j++)
					{
						if (j > 0)
							ImGui::SameLine();

						if (ImGui::Selectable(shapeTypes[(3 * i) + j], selected == ((3 * i) + j), 0, ImVec2(70.0f, 30.0f)))
						{
							selected = (3 * i) + j;
						}
					}
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("Fill", node_flags))
			{
				ImGui::TreePush();
				ImGui::Text("Current Color:");
				ImGui::SameLine();
				ImGui::ColorEdit3("##1", fillColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker);

				if (ImGui::TreeNodeEx("Default Palette:##1", node_flags)) {
					drawPalette(fillColor, "palette##1");
				}

				if (ImGui::TreeNodeEx("Custom Color:##1", ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
					ImGui::ColorEdit3("##fillColor", fillColor);

				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("Border", node_flags))
			{
				ImGui::TreePush();
				ImGui::Text("Current Color:");
				ImGui::SameLine();
				ImGui::ColorEdit3("##2", borderColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker);

				if (ImGui::TreeNodeEx("Default Palette:##2", ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
					drawPalette(borderColor, "palette##2");
				}

				if (ImGui::TreeNodeEx("Custom Color:##2", ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
					ImGui::ColorEdit3("##borderColor", borderColor);
				}
				ImGui::TreePop();

			}
		}

		ImGui::End();

		ImGui::ShowDemoWindow();
	}
};