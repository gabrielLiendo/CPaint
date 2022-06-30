#pragma once
#include "imgui.h"

// Colors
float bgColor[] = { 1.0f, 1.0f, 1.0f };
float borderColor[] = { 0.0f, 0.0f, 0.0f };
float fillColor[] = { 0.0f, 0.0f, 0.0f };

const char* renderingModes[] = { "Hardware", "Software" };
const char* currentMode = renderingModes[0];

const char* shapeTypes[] = { "Line", "Circle", "Ellipse", "Rectangle", "Triangle", "Bezier\n Curve"};
const char* shapeSelected = shapeTypes[0];

static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

void switchMode() {
	if (currentMode == "Hardware")
		currentMode = renderingModes[1];
	else
		currentMode = renderingModes[0];
}

void drawPalette(float* bindedColor, const char* desc_id)
{	
	static ImVec4 saved_palette[28] = {};
	static bool saved_palette_init = true;
	if (saved_palette_init)
	{
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
				saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
		}
		saved_palette_init = false;
	}

	ImGui::BeginGroup(); // Lock X position
	for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
	{
		ImGui::PushID(n);
		if ((n % 14) != 0)
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

		ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
		if (ImGui::ColorButton(desc_id, saved_palette[n], palette_button_flags, ImVec2(20, 20))) 
		{
			bindedColor[0] = saved_palette[n].x;
			bindedColor[1] = saved_palette[n].y;
			bindedColor[2] = saved_palette[n].z;
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
	ImGui::EndGroup(); // Lock X position
}

void drawUI()
{
	//Set UI flags
	ImGuiWindowFlags window_flags =  ImGuiWindowFlags_NoTitleBar;
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
		if (ImGui::BeginCombo("##rendering mode", currentMode, ImGuiComboFlags_NoArrowButton))
		{
			for (int n = 0; n < IM_ARRAYSIZE(renderingModes); n++)
			{
				bool is_selected = (currentMode == renderingModes[n]);
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
			switchMode();

		ImGui::SameLine(0, spacing);
		if (ImGui::ArrowButton("##r", ImGuiDir_Right))
			switchMode();

		ImGui::SameLine(0, style.ItemInnerSpacing.x);
		ImGui::Text("Rendering Mode");

		ImGui::ColorEdit3("Background Color", bgColor);
	}

	if (ImGui::CollapsingHeader("Draw", ImGuiTreeNodeFlags_DefaultOpen))
	{	
		if (ImGui::TreeNodeEx("Shape", node_flags))
		{	
			ImGui::TreePush();

			static int selected = -1;
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 3; j++)
				{
					if (j > 0)
						ImGui::SameLine();

					if (ImGui::Selectable(shapeTypes[(3 * i) + j], selected == ((3 * i) + j), 0, ImVec2(70.0f, 50.0f)))
					{
						selected = (3 * i) + j;
						shapeSelected = shapeTypes[selected];
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