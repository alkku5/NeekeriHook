#pragma once
#include "SDK.h"

struct settings_t
{
	struct
	{
		bool Bhop;
		int AimbotMode;
		bool LegitBacktrack;
		int LegitBacktrackTicks;
	}Menu;

}; extern settings_t g_Settings;

class CMenu
{
public:
	void ThinkKeyboard()
	{
		static bool PressedButtons = false;

		if (!PressedButtons && GetAsyncKeyState(VK_INSERT))
		{
			PressedButtons = true;
		}
		else if (PressedButtons && !GetAsyncKeyState(VK_INSERT))
		{
			PressedButtons = false;
			m_opened = !m_opened;
		}
	}

	void SelectedTab()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Button] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
	}

	void NormalTab()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	}

	void InitializeTheme()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.FramePadding = ImVec2(4, 2);
		style.ItemSpacing = ImVec2(6, 2);
		style.ItemInnerSpacing = ImVec2(6, 4);
		style.Alpha = 0.0f;
		style.WindowRounding = 4.0f;
		style.WindowTitleAlign = ImVec2(0.5, 0.5);
		style.FrameRounding = 2.0f;
		style.IndentSpacing = 6.0f;
		style.ItemInnerSpacing = ImVec2(2, 4);
		style.ColumnsMinSpacing = 50.0f;
		style.GrabMinSize = 14.0f;
		style.GrabRounding = 16.0f;
		style.ScrollbarSize = 12.0f;
		style.ScrollbarRounding = 16.0f;

		style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.58f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.60f, 0.78f, 0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.15f, 0.60f, 0.78f, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.60f, 0.78f, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.15f, 0.60f, 0.78f, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.15f, 0.60f, 0.78f, 0.78f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.60f, 0.78f, 1.0f); // 
		style.Colors[ImGuiCol_Column] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
		style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.15f, 0.60f, 0.78f, 0.78f);
		style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.15f, 0.60f, 0.78f, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_CloseButton] = ImVec4(0.86f, 0.93f, 0.89f, 0.16f);
		style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.86f, 0.93f, 0.89f, 0.39f);
		style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.15f, 0.60f, 0.78f, 0.43f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);
	}

	void drawAimbot()
	{
		ImGui::Spacing();

		ImGui::BeginChild(1, ImVec2(335, 190), true);
		{                 
			int item_current = 0;
			const char* items[] = { "Legit", "Rage"};
			ImGui::Combo("Mode", &g_Settings.Menu.AimbotMode, items, 2);
			if (g_Settings.Menu.AimbotMode == 0) {
				//Show legit options
				ImGui::Checkbox("Backtrack", &g_Settings.Menu.LegitBacktrack);
				ImGui::SliderInt("Ticks", &g_Settings.Menu.LegitBacktrackTicks, 0, 12);
			}
			else if (g_Settings.Menu.AimbotMode == 1) {
				//show rage options
			}
			
		}
		ImGui::EndChild();
	}
	void drawEsp()
	{
		ImGui::Spacing();
		ImGui::BeginChild(2, ImVec2(335, 190), true);
		{
 
		}
		ImGui::EndChild();

	}
	void drawMisc()
	{
		ImGui::Spacing();
		ImGui::BeginChild(3, ImVec2(335, 190), true);
		{
			ImGui::Checkbox("Bhop", &g_Settings.Menu.Bhop);

		}
		ImGui::EndChild();
	}
	void drawSkin()
	{
		ImGui::Spacing();
		ImGui::BeginChild(4, ImVec2(335, 190), true);
		{

		}
		ImGui::EndChild();
	}

	int active_tab = 0;

	void Think()
	{
		static bool last = m_opened;

		if (m_opened != last)
		{
			last = m_opened;

			std::string msg = ("cl_mouseenable ") + std::to_string(!m_opened);
			g_pEngine->ExecuteClientCmd(msg.c_str());
		}

		ImGui::GetIO().MouseDrawCursor = m_opened;

		/* your dope imgui menu here */
		ImGui::Begin(CHEAT_NAME, &m_opened, ImVec2(0, 0), 1.0f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		{
			ImGui::SetWindowSize(ImVec2(350, 250));

			if (ImGui::Button("AIMBOT", ImVec2(79, 20))) active_tab = 1;
			ImGui::SameLine();
			if (ImGui::Button("VISUAL", ImVec2(79, 20))) active_tab = 2;
			ImGui::SameLine();
			if (ImGui::Button("MISC", ImVec2(79, 20))) active_tab = 3;
			ImGui::SameLine();
			if (ImGui::Button("SKINS", ImVec2(79, 20))) active_tab = 4;

			ImGui::Separator();

			switch (active_tab)
			{
			case 1:
				drawAimbot();
				break;
			case 2:
				drawEsp();
				break;
			case 3:
				drawMisc();
				break;
			case 4:
				drawSkin();
				break;
			}

		}
		ImGui::End();
	}


	bool m_opened = false;
}; extern CMenu* g_pMenu;
