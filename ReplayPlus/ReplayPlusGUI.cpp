#include "pch.h"
#include "ReplayPlus.h"
#include "bakkesmod/wrappers/GuiManagerWrapper.h"

// Plugin Settings Window code here
std::string ReplayPlus::GetPluginName() {
	return "ReplayPlus";
}

// Render the plugin settings here
// This will show up in bakkesmod when the plugin is loaded at
//  f2 -> plugins -> ReplayPlus
void ReplayPlus::RenderSettings() {
    bool isTabOpen = ImGui::BeginTabBar("SettingsTab", ImGuiTabBarFlags_None);
    if (isTabOpen) {
        // Map settings tab
        bool isMapSettingsTabOpen = ImGui::BeginTabItem("Map");
        if (isMapSettingsTabOpen) {
            // Enable map
            CVarWrapper isEnabledMapCvar = cvarManager->getCvar("map_enabled");
            if (!isEnabledMapCvar) return;
            bool isEnabledMap = isEnabledMapCvar.getBoolValue();
            if (ImGui::Checkbox("Enabled map", &isEnabledMap)) isEnabledMapCvar.setValue(isEnabledMap);

            // If map is not enabled, disable input below.
            if (!isEnabledMap) BaseGUI::PushDisabled();

            // Ball size setting
            CVarWrapper ballSizeCvar = cvarManager->getCvar("map_ballsize");
            if (!ballSizeCvar) return;
            float ballSize = ballSizeCvar.getFloatValue();
            if (ImGui::SliderFloat("Ball size", &ballSize, 0.01f, 10.0f)) ballSizeCvar.setValue(ballSize);
            ImGui::SameLine();
            if (ImGui::Button("Reset###ballsizereset")) { ballSizeCvar.ResetToDefault(); }

            // Boostpad size setting
            CVarWrapper boostpadSizeCvar = cvarManager->getCvar("map_boostpadsize");
            if (!boostpadSizeCvar) return;
            float boostpadSize = boostpadSizeCvar.getFloatValue();
            if (ImGui::SliderFloat("Boostpad size", &boostpadSize, 0.01f, 10.0f)) boostpadSizeCvar.setValue(boostpadSize);
            ImGui::SameLine();
            if (ImGui::Button("Reset###boostpadsizereset")) { boostpadSizeCvar.ResetToDefault(); }

            // Car size setting
            CVarWrapper carSizeCvar = cvarManager->getCvar("map_carsize");
            if (!carSizeCvar) return;
            float carSize = carSizeCvar.getFloatValue();
            if (ImGui::SliderFloat("Car size", &carSize, 0.01f, 10.0f)) carSizeCvar.setValue(carSize);
            ImGui::SameLine();
            if (ImGui::Button("Reset###carsizereset")) { carSizeCvar.ResetToDefault(); }

            ImGui::Separator();

            // Name settings
            if (ImGui::TreeNode("Name settings")) {
                // Name visible setting
                CVarWrapper isVisibleNameCvar = cvarManager->getCvar("map_isnamevisible");
                if (!isVisibleNameCvar) { ImGui::TreePop(); return; }
                bool isVisibleName = isVisibleNameCvar.getBoolValue();
                if (ImGui::Checkbox("Name visibled", &isVisibleName)) isVisibleNameCvar.setValue(isVisibleName);

                // If name is not visibled, disable input name settings.
                if (!isVisibleName) BaseGUI::PushDisabled();

                // Enable to rescale name size by window size.
                CVarWrapper isNameScaledByWindowSizeCvar = cvarManager->getCvar("map_isnamescaledbywindowsize");
                if (!isNameScaledByWindowSizeCvar) { BaseGUI::PopDisabled(); ImGui::TreePop(); return; }
                bool isNameScaledByWindowSize = isNameScaledByWindowSizeCvar.getBoolValue();
                if (ImGui::Checkbox("Enable to rescale name size by window size", &isNameScaledByWindowSize)) isNameScaledByWindowSizeCvar.setValue(isNameScaledByWindowSize);

                // Name size setting
                CVarWrapper nameSizeCvar = cvarManager->getCvar("map_namesize");
                if (!nameSizeCvar) { BaseGUI::PopDisabled(); ImGui::TreePop(); return; }
                float nameSize = nameSizeCvar.getFloatValue();
                if (ImGui::SliderFloat("Name size", &nameSize, 0.01f, 10.0f)) nameSizeCvar.setValue(nameSize);
                ImGui::SameLine();
                if (ImGui::Button("Reset###namesizereset")) { nameSizeCvar.ResetToDefault(); }

                // End of disabled by name visibled.
                if (!isVisibleName) BaseGUI::PopDisabled();

                // End of name settings.
                ImGui::TreePop();
            }
            ImGui::Separator();

            // Start boost amount settings.
            if (ImGui::TreeNode("Boost amount settings")) {
                // Boost amount visible setting
                CVarWrapper isVisibleBoostAmountCvar = cvarManager->getCvar("map_isboostamountvisible");
                if (!isVisibleBoostAmountCvar) { ImGui::TreePop(); return; }
                bool isVisibleBoostAmount = isVisibleBoostAmountCvar.getBoolValue();
                if (ImGui::Checkbox("Boost amount visibled", &isVisibleBoostAmount)) isVisibleBoostAmountCvar.setValue(isVisibleBoostAmount);

                // If boost amount is not visibled, disable input boost amount settings.
                if (!isVisibleBoostAmount) BaseGUI::PushDisabled();

                // Enable to rescale boost amount size by window size.
                CVarWrapper isBoostAmountSizeRescaledByWindowSizeCvar = cvarManager->getCvar("map_isboostamountsizerescaledbywindowsize");
                if (!isBoostAmountSizeRescaledByWindowSizeCvar) { BaseGUI::PopDisabled(); ImGui::TreePop(); return; }
                bool isBoostAmountSizeRescaledByWindowSize = isBoostAmountSizeRescaledByWindowSizeCvar.getBoolValue();
                if (ImGui::Checkbox("Enable to rescale boost amount size by window size", &isBoostAmountSizeRescaledByWindowSize)) isBoostAmountSizeRescaledByWindowSizeCvar.setValue(isBoostAmountSizeRescaledByWindowSize);

                // Boost amount size setting
                CVarWrapper boostAmountSizeCvar = cvarManager->getCvar("map_boostamountsize");
                if (!boostAmountSizeCvar) { BaseGUI::PopDisabled(); ImGui::TreePop(); return; }
                float boostAmountSize = boostAmountSizeCvar.getFloatValue();
                if (ImGui::SliderFloat("Boost amount size", &boostAmountSize, 0.01f, 10.0f)) boostAmountSizeCvar.setValue(boostAmountSize);
                ImGui::SameLine();
                if (ImGui::Button("Reset###boostamountsizereset")) { boostAmountSizeCvar.ResetToDefault(); }

                // End of disabled by boost amount visibled.
                if (!isVisibleBoostAmount) BaseGUI::PopDisabled();

                // End of boost amount settings.
                ImGui::TreePop();
            }
            ImGui::Separator();

            // End of disabled by map enabled.
            if (!isEnabledMap) BaseGUI::PopDisabled();

            // End of map settings tab.
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

// Do ImGui rendering here
/*
Map size:
 Wedth: 8192 (Goal: 1786)
 Height: 12000 (Goal: 880)
 Aspect: 256:375
*/
void ReplayPlus::Render() {
	if (isWindowRenderable_ == false || gameWrapper->IsInReplay() == false) { return; }
	if (map) { map->Render(); }
}

// Name of the menu that is used to toggle the window.
std::string ReplayPlus::GetMenuName() {
	return "replayplus";
}

// Title to give the menu
std::string ReplayPlus::GetMenuTitle() {
	return menuTitle_;
}

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void ReplayPlus::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
	gameWrapper->GetGUIManager().LoadFont("Japanese", "meiryo.ttc", 32, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesJapanese());
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool ReplayPlus::ShouldBlockInput() {
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool ReplayPlus::IsActiveOverlay() {
	return true;
}

// Called when window is opened
void ReplayPlus::OnOpen() {
	isWindowRenderable_ = true;
}

// Called when window is closed
void ReplayPlus::OnClose() {
	isWindowRenderable_ = false;
}
