#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


class PogoTrainer: public BakkesMod::Plugin::BakkesModPlugin
	,public SettingsWindowBase // Uncomment if you wanna render your own tab in the settings menu
	//,public PluginWindowBase // Uncomment if you want to render your own plugin window
{

	bool shouldCloseMenu = false;
	int autoResetTimerMax = 0;

	//Boilerplate
	void onLoad() override;
	void controller();
	void determinePlacements();
	void applySettings();
	void setShot();
	void reset();
	void autoReset();
	void freeze();

	void closeMenu();
	void loadHooks();

	//void onUnload() override; // Uncomment and implement if you need a unload method

public:

	bool pluginActive();
	void disablePlugin();
	void RenderSettings() override; // Uncomment if you wanna render your own tab in the settings menu
	bool carOnlyMode();
	bool offsetCarAndBall();
	bool randomizeCarOrientationOn();
	bool randomizeCarAngularVelocity();
	bool randomizeBallAngularVelocity();
	bool manualResetMode();
	bool pogoRecoveryMode();
	bool disableScoring();
	//void RenderWindow() override; // Uncomment if you want to render your own plugin window
};
