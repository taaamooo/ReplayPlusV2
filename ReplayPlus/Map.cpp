#include "pch.h"
#include "Map.h"
#include "bakkesmod/wrappers/GuiManagerWrapper.h"

Map::Map() {
	_globalCvarManager->registerCvar("map_enabled", "1", "Enable map", true, true, 0, true, 1)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		isEnabled = cvar.getBoolValue();
	});

	// Ball size
	_globalCvarManager->registerCvar("map_ballsize", "0.45", "Ball size in map", true, true, 0.01f, true, 10.0f)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		ballSizeInMap = cvar.getFloatValue();
	});
	// Boostpad size
	_globalCvarManager->registerCvar("map_boostpadsize", "0.1", "Boost size in map", true, true, 0.01f, true, 10.0f)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		boostpadSizeInMap = cvar.getFloatValue();
	});
	// Car size
	_globalCvarManager->registerCvar("map_carsize", "0.3", "Car size in map", true, true, 0.01f, true, 10.0f)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		carSizeInMap = cvar.getFloatValue();
	});

	// Is name visible
	_globalCvarManager->registerCvar("map_isnamevisible", "1", "Enable to show name in map", true, true, 0, true, 1)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		isNameVisible = cvar.getBoolValue();
	});
	// Is name scaled by window size
	_globalCvarManager->registerCvar("map_isnamescaledbywindowsize", "1", "Scaled name size by window size", true, true, 0, true, 1)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		isNameRescaledbyWindowSize = cvar.getBoolValue();
	});

	// Name size
	_globalCvarManager->registerCvar("map_namesize", "1.5", "Name size in map", true, true, 0.01f, true, 10.0f)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		nameSizeInMap = cvar.getFloatValue();
	});
	// Is boost amount visible
	_globalCvarManager->registerCvar("map_isboostamountvisible", "1", "Enable to show boost amount in map", true, true, 0, true, 1)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		isBoostAmountVisible = cvar.getBoolValue();
	});
	// Is boost amount size rescale by window size
	_globalCvarManager->registerCvar("map_isboostamountsizerescaledbywindowsize", "1", "Scaled boost amount size by window size", true, true, 0, true, 1)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		isBoostAmountRescaledbyWindowSize = cvar.getBoolValue();
	});

	// BoostAmount size
	_globalCvarManager->registerCvar("map_boostamountsize", "3.0", "Boost amount size in map", true, true, 0.01f, true, 10.0f)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		boostAmountSizeInMap = cvar.getFloatValue();
	});

	_globalCvarManager->registerNotifier("rp_toggle_map", [this](std::vector<std::string> args) { ToggleWindow(args); }, "Toggle map window", PERMISSION_REPLAY);
	_globalCvarManager->setBind("M", "rp_toggle_map");

	_globalGameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.VehiclePickup_TA.SetNetRelevant",
		std::bind(&Map::OnPickUpBoost, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	arenaImage = std::make_shared<ImageWrapper>(_dataFolderPath / "replayplus\\imgs\\arena.png", false, true);
	ballImage = std::make_shared<ImageWrapper>(_dataFolderPath / "replayplus\\imgs\\ball.png", false, true);
	carImage = std::make_shared<ImageWrapper>(_dataFolderPath / "replayplus\\imgs\\car.png", false, true);
	boostpad_emptyImage = std::make_shared<ImageWrapper>(_dataFolderPath / "replayplus\\imgs\\boostpad_empty.png", false, true);
	boostpad_fullImage = std::make_shared<ImageWrapper>(_dataFolderPath / "replayplus\\imgs\\boostpad_full.png", false, true);
	
	font = _globalGameWrapper->GetGUIManager().GetFont("Japanese");
}

void Map::OnBeginReplay() {
	_globalGameWrapper->HookEvent("Function Engine.GameViewportClient.Tick", std::bind(&Map::OnTick, this));
}

void Map::OnTick() {
	if (!isEnabled) return;
	camera = std::make_shared<CameraWrapper>(_globalGameWrapper->GetCamera());
}

void Map::OnPickUpBoost(ActorWrapper boostPad, void* params, std::string eventName){
	if (boostPad.IsNull()) return;
	Vector location = boostPad.GetLocation();
	BoostPad* getter = (BoostPad*)params;
	if (getter != nullptr) {
		for (int i = 0; i < boostpads.size(); i++) {
			if (IsValueInError(location.X, boostpads[i].location.X, 1000.0f) && IsValueInError(location.Y, boostpads[i].location.Y, 400.0f)) {
				boostpads[i].isEmpty = getter->isEmpty;
				return;
			}
		}

		BoostPad newBoostPad;
		newBoostPad.isEmpty = getter->isEmpty;
		newBoostPad.location = location;
		if (location.Z > 0.0f) {
			boostpads.push_back(newBoostPad);
		}
	}
}

ImVec2 Map::GetLocationInMap(Vector location, ImVec2 offset, ImVec2 upperleftpos, ImVec2 lowerrightpos) {
	float location_x = 4210.0f - location.X; // 4096
	float location_y = 6140.0f - location.Y;
	float location_x_rate = location_x / 8420.0f; // 8192
	float location_y_rate = location_y / 12280.0f;

	ImVec2 location_inMap{ upperleftpos.x + (lowerrightpos.x - upperleftpos.x) * location_x_rate - offset.x / 2,
							upperleftpos.y + (lowerrightpos.y - upperleftpos.y) * location_y_rate - offset.y / 2 };
	return location_inMap;
}

bool Map::IsValueInError(float v1, float v2, float error) {
	float value = abs(v1 - v2);
	if (value < error) return true;
	else return false;
}

void Map::OnEndReplay() {
	_globalGameWrapper->UnhookEvent("Function Engine.GameViewportClient.Tick");
}

void Map::Render() {
	if (!isWindowOpen_ || !isEnabled) { return; }
	ImGui::SetNextWindowSizeConstraints(ImVec2(256, 375), ImVec2(512, 750), SameAspect);
	if (!ImGui::Begin("Map", &isWindowOpen_, ImGuiWindowFlags_None)) {
		LOG("Failed to open map window.");
		ImGui::End();
		return;
	}

	// Render arena image
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 windowRect = ImGui::GetWindowSize();
	float windowAspect = windowRect.x / 750;
	if (!arenaImage->IsLoadedForImGui()) { LOG("Failed to load arena image."); ImGui::End(); return; }
	ImVec2 arenaUpperLeftPos = windowPos + ImVec2(10.0f, 10.0f + windowBarHeight);
	ImVec2 arenaLowerRightPos = windowPos + windowRect - ImVec2(10.0f, 10.0f);
	drawList->AddImage(arenaImage->GetImGuiTex(), arenaUpperLeftPos, arenaLowerRightPos);
	
	// Render boostpad image
	if (!boostpad_emptyImage->IsLoadedForImGui() || !boostpad_fullImage->IsLoadedForImGui()) { ImGui::End(); return; }
	if (boostpads.empty()) { ImGui::End(); return; }
	for (int i = 0; i < boostpads.size(); i++) {
		BoostPad boostpad = boostpads[i];
		void* texID = boostpad_fullImage->GetImGuiTex();
		if (boostpad.isEmpty) { texID = boostpad_emptyImage->GetImGuiTex(); }
		ImVec2 rect = ImVec2(boostpad_emptyImage->GetSize().X, boostpad_emptyImage->GetSize().Y) * windowAspect * boostpadSizeInMap;
		if (boostpad.location.Z > 73.0f) { rect *= 1.5f; }
		ImVec2 locInMap = GetLocationInMap(boostpad.location, rect, arenaUpperLeftPos, arenaLowerRightPos);
		drawList->AddImage(texID, locInMap, locInMap + rect);
	}

	// Render ball image
	ServerWrapper server = _globalGameWrapper->GetCurrentGameState();
	BallWrapper ball = server.GetBall();
	if (!ball || !ballImage->IsLoadedForImGui()) { LOG("Ball is null."); ImGui::End(); return; }
	Vector ballLocation = ball.GetLocation();
	ImVec2 ballImageRect = ImVec2(ballImage->GetSize().X, ballImage->GetSize().Y) * windowAspect * ballSizeInMap;
	ImVec2 ballImagePos = GetLocationInMap(ballLocation, ballImageRect, arenaUpperLeftPos, arenaLowerRightPos);
	drawList->AddImage(ballImage->GetImGuiTex(), ballImagePos, ballImagePos + ballImageRect);
	
	// Render car image
	if (!carImage->IsLoadedForImGui()) { LOG("Faield to load car image."); ImGui::End(); return; }
	ImVec2 carImageRect = ImVec2(carImage->GetSize().X, carImage->GetSize().Y) * windowAspect * carSizeInMap;
	font = _globalGameWrapper->GetGUIManager().GetFont("Japanese");
	if (!font) { LOG("Failed to load font."); ImGui::End(); return; }
	float nameFontSize = font->FontSize * nameSizeInMap;
	if (isNameRescaledbyWindowSize) nameFontSize *= windowAspect;
	float boostAmountFontSize = ImGui::GetFontSize() * boostAmountSizeInMap;
	if (isBoostAmountRescaledbyWindowSize) boostAmountFontSize *= windowAspect;

	if (!server) { LOG("Failed to get server."); ImGui::End(); return; }
	ArrayWrapper<CarWrapper> cws = server.GetCars();
	if (cws.IsNull()) { LOG("Cars is null."); ImGui::End(); return; }
	for (int i = 0; i < cws.Count(); i++) {
		CarWrapper car = cws.Get(i);
		if (car.IsNull()) { LOG("Car is null."); continue; }
		// IMPORTANT: Getting rotation is first!!!
		Rotator rotation = car.GetRotation();
		Vector direction = RotatorToVector(rotation);
		Vector location = car.GetLocation();
		ImVec2 carImagePos = GetLocationInMap(location, carImageRect, arenaUpperLeftPos, arenaLowerRightPos);
		ImVec2 carImageCenter = carImagePos + carImageRect / 2;
		PriWrapper carPri = car.GetPRI();
		if (!carPri) { LOG("PRI is null."); continue; }
		TeamInfoWrapper team = carPri.GetTeam();
		if (!team) { LOG("Team is null."); continue; }
		ImColor carColor = team.GetTeamIndex() == 0 ? ImColor(0.0f, 0.0f, 255.0f) : ImColor(255.0f, 0.0f, 0.0f);
		if (camera) {
			PriWrapper pri = PriWrapper(reinterpret_cast<std::uintptr_t>(camera->GetViewTarget().PRI));
			if (!pri.IsNull()) {
				UnrealStringWrapper targetName = pri.GetPlayerName();
				UnrealStringWrapper carName = carPri.GetPlayerName();
				if (targetName.ToString() == carName.ToString()) carColor = ImColor(0.0f, 255.0f, 0.0f);
			}
		}
		RenderRotatedImage(carImage->GetImGuiTex(), carImageCenter, carImageRect, atan2(-direction.X, direction.Y), carColor, drawList);
	
		// Render player name
		if (isNameVisible) {
			UnrealStringWrapper carNameUE = carPri.GetPlayerName();
			if (!carNameUE.IsNull() && font != nullptr) {
				ImGui::PushFont(font);
				ImVec2 offset = ImGui::GetFont()->CalcTextSizeA(nameFontSize, FLT_MAX, 0.0f, carNameUE.ToString().c_str());
				drawList->AddText(font, nameFontSize, carImageCenter - ImVec2(offset.x / 2, 60.0f * windowAspect), IM_COL32_WHITE, carNameUE.ToString().c_str());
				ImGui::PopFont();
			}
		}

		// Render car boost amount.
		if (isBoostAmountVisible) {
			BoostWrapper boost = car.GetBoostComponent();
			if (!boost) { LOG("Failed to get boost component."); continue; }
			float boostAmount = boost.GetCurrentBoostAmount() * 100;
			std::string boostAmountStr = std::to_string((int)boostAmount);
			ImVec2 offset = ImGui::GetFont()->CalcTextSizeA(boostAmountFontSize, FLT_MAX, 0.0f, boostAmountStr.c_str());
			drawList->AddText(ImGui::GetFont(), boostAmountFontSize, carImageCenter - ImVec2(offset.x / 2, -30.0f * windowAspect), IM_COL32_WHITE, boostAmountStr.c_str());
		}
	}
	ImGui::End();
}

void Map::SameAspect(ImGuiSizeCallbackData* data) {
	data->DesiredSize.y = data->DesiredSize.x * 1.478f;
}
