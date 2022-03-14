#include "pch.h"
#include "BaseGUI.h"

void BaseGUI::ToggleWindow(std::vector<std::string> params)
{
	isWindowOpen_ = !isWindowOpen_;
}

/*
*  PushDisabled()
*  Disabled below inputs.
*  At the end, you must write PopDisabled().
*/
void BaseGUI::PushDisabled()
{
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
}

/*
* PopDisabled()
* Enabled inputs between this and latest called PushDisabled().
* You must write PushDisabled() before calling this.
*/
void BaseGUI::PopDisabled()
{
    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
}

void BaseGUI::RenderRotatedImage(ImTextureID id, ImVec2 center, ImVec2 size, float angle, ImColor color, ImDrawList* draw_list)
{
    //ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float cos_a = cosf(angle);
    float sin_a = sinf(angle);
    ImVec2 pos[4] =
    {
        center + ImRotate(ImVec2(-size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
        center + ImRotate(ImVec2(+size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
        center + ImRotate(ImVec2(+size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a),
        center + ImRotate(ImVec2(-size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a)
    };
    ImVec2 uvs[4] =
    {
        ImVec2(0.0f, 0.0f),
        ImVec2(1.0f, 0.0f),
        ImVec2(1.0f, 1.0f),
        ImVec2(0.0f, 1.0f)
    };

    draw_list->AddImageQuad(id, pos[0], pos[1], pos[2], pos[3], uvs[0], uvs[1], uvs[2], uvs[3], color);
}
