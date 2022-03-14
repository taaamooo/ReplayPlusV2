#pragma once

#include "pch.h"

class BaseGUI
{
public:
	virtual void Render() = 0;
	void ToggleWindow(std::vector<std::string> params);
	
	/*
	*  PushDisabled()
	*  Disabled below inputs.
	*  At the end, you must write PopDisabled().
	*/
	static void PushDisabled();

	/*
	* PopDisabled()
	* Enabled inputs between this and latest called PushDisabled().
	* You must write PushDisabled() before calling this.
	*/
	static void PopDisabled();

protected:
	void RenderRotatedImage(ImTextureID id, ImVec2 center, ImVec2 size, float angle, ImColor color, ImDrawList* draw_list);

	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string windowTitle = "";
	const float windowBarHeight = 20.0f;
};