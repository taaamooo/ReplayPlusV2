#pragma once

#include "BaseGUI.h"

class Map : public BaseGUI {
public:
	Map();
	static void SameAspect(ImGuiSizeCallbackData* data);
	void OnBeginReplay();
	void OnEndReplay();
	void Render();

	struct Car {
		Vector location;
		Rotator rotation;
		PriWrapper carPri;
		BoostWrapper boost;
	};
	std::vector<Car> cars;
	std::vector<Car> latestCars;

private:
	void OnTick();
	void OnPickUpBoost(ActorWrapper boostPad, void* params, std::string eventName);
	ImVec2 GetLocationInMap(Vector location, ImVec2 offset, ImVec2 upperleftpos, ImVec2 lowerrightpos);
	bool IsValueInError(float v1, float v2, float error);
	bool IsLargeBoostpad(Vector location, std::string name);

	std::shared_ptr<ImageWrapper> arenaImage;
	std::shared_ptr<ImageWrapper> ballImage;
	std::shared_ptr<ImageWrapper> carImage;
	std::shared_ptr<ImageWrapper> boostpad_emptyImage;
	std::shared_ptr<ImageWrapper> boostpad_fullImage;

	struct BoostPad {
		bool isEmpty = false;
		Vector location;
		bool isLarge = false;
	};
	std::vector<BoostPad> boostpads;
	std::shared_ptr<BallWrapper> ball;
	std::shared_ptr<CameraWrapper> camera;
	ImFont* font;
	std::string mapName;

	// Cvar
	bool isEnabled = true;
	float ballSizeInMap = 0.45f;
	float boostpadSizeInMap = 0.1f;
	float carSizeInMap = 0.3f;
	bool isNameVisible = true;
	bool isNameRescaledbyWindowSize = true;
	float nameSizeInMap = 1.5f;
	bool isBoostAmountVisible = true;
	bool isBoostAmountRescaledbyWindowSize = true;
	float boostAmountSizeInMap = 3.0f;
};