#include "pch.h"
#include "ReplayPlus.h"


BAKKESMOD_PLUGIN(ReplayPlus, "Replay_Plus_V2", plugin_version, PLUGINTYPE_REPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
std::shared_ptr<GameWrapper> _globalGameWrapper;
std::filesystem::path _dataFolderPath;

void ReplayPlus::onLoad() {
	_globalCvarManager = cvarManager;
	_globalGameWrapper = gameWrapper;
	_dataFolderPath = gameWrapper->GetDataFolder();

	cvarManager->executeCommand("togglemenu " + GetMenuName());
	cvarManager->setBind("F8", "togglemenu " + GetMenuName());

	map = std::make_shared<Map>();

	_globalGameWrapper->HookEvent("Function TAGame.Team_TA.PostBeginPlay", std::bind(&Map::OnBeginReplay, map));
	_globalGameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.Destroyed", std::bind(&Map::OnEndReplay, map));
	_globalGameWrapper->HookEvent("Function TAGame.GFxData_MainMenu_TA.MainMenuAdded", std::bind(&Map::OnEndReplay, map));
}

void ReplayPlus::onUnload() {

}