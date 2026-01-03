#include "pch.h"
#include "PogoTrainer.h"

bool pluginOn = false;
bool manualBeingViewed = false;
bool showManual = false;

static bool carOnlyActive = false;
static bool carAndBallOffset = false;
static bool randomCarOrientation = false;
static bool randomCarAngularVelocity = false;
static bool randomBallAngularVelocity = false;
static bool manualReset = true;
static bool pogoRecovery = false;
static bool scoringDisabled = false;

bool PogoTrainer::pluginActive()
{
	return pluginOn;
}

void PogoTrainer::disablePlugin()
{
	pluginOn = false;
}

void PogoTrainer::RenderSettings()
{
	if(!pluginOn)
		if (ImGui::Button("On"))
		{
			pluginOn = true;
			shouldCloseMenu = true;
		}
	if (pluginOn)
		if (ImGui::Button("Off"))
		{
			pluginOn = false;
		}
	
	ImGui::NewLine; ImGui::Text("  ");

	// car only mode toggle
	ImGui::Checkbox("Car Only Mode", &carOnlyActive);

	ImGui::NewLine; ImGui::Text("  ");

	// offset car and ball toggle
	ImGui::Checkbox("Offset Car And Ball", &carAndBallOffset);

	ImGui::NewLine; ImGui::Text("  ");

	// randomize car orientation toggle
	ImGui::Checkbox("Randomize Car Orientation", &randomCarOrientation);

	ImGui::NewLine; ImGui::Text("  ");

	// randomize car ang vel toggle
	ImGui::Checkbox("Randomize Car Angular Velocity", &randomCarAngularVelocity);

	ImGui::NewLine; ImGui::Text("  ");

	// randomize ball ang vel toggle
	ImGui::Checkbox("Randomize Ball Angular Velocity", &randomBallAngularVelocity);

	ImGui::NewLine; ImGui::Text("  ");

	// pogo recovery mode toggle
	ImGui::Checkbox("Pogo Recovery Mode", &pogoRecovery);

	ImGui::NewLine; ImGui::Text("  ");

	// disable scoring toggle
	ImGui::Checkbox("Disable Scoring", &scoringDisabled);

	ImGui::NewLine; ImGui::Text("  ");
	ImGui::NewLine; ImGui::Text("  ");

	ImGui::Text("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
}

bool PogoTrainer::carOnlyMode()
{
	return carOnlyActive;
}

bool PogoTrainer::offsetCarAndBall()
{
	return carAndBallOffset;
}

bool PogoTrainer::randomizeCarOrientationOn()
{
	return randomCarOrientation;
}

bool PogoTrainer::randomizeCarAngularVelocity()
{
	return randomCarAngularVelocity;
}

bool PogoTrainer::randomizeBallAngularVelocity()
{
	return randomBallAngularVelocity;
}

bool PogoTrainer::manualResetMode()
{
	return manualReset;
}

bool PogoTrainer::pogoRecoveryMode()
{
	return pogoRecovery;
}

bool PogoTrainer::disableScoring()
{
	return scoringDisabled;
}