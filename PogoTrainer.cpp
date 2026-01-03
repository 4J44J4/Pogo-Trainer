#include "pch.h"
#include "PogoTrainer.h"
#include <cmath>
#include <ctime>


BAKKESMOD_PLUGIN(PogoTrainer, "write a plugin description here", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

Vector carPos;
Vector carVel;
Vector carAngVel;
Rotator carRot;
Vector ballPos;
Vector ballVel;
Vector ballAngVel;

bool placementsDetermined = false;
bool settingsApplied = false;
bool shotSet = false;
bool shotCreated = false;
bool hasReset = false;

int resetTimer = 0;
int autoResetTimer = -1;
int freezeTimer = 0;

void PogoTrainer::onLoad()
{

	_globalCvarManager = cvarManager;
	
	this->loadHooks();
}

void PogoTrainer::controller()
{

	if (!gameWrapper->IsInFreeplay())
	{
		LOG("Plugin disabled because the user is not in freeplay.");
		disablePlugin();
	}

	else if (!shotCreated)
	{
		if (resetTimer > 0)
			resetTimer--;
		else
		{
			determinePlacements();
			applySettings();
			setShot();
		}
	}

	else if (!manualResetMode() || autoResetTimer > -1)
	{
		autoReset();
	}

	if (shouldCloseMenu)
	{
		closeMenu();
	}

	if (disableScoring() && freezeTimer == 0)
	{
		freeze();
	}
	else if (freezeTimer > 0 && autoResetTimer == -1)
		freezeTimer--;

}

void PogoTrainer::determinePlacements()
{
	if (!placementsDetermined)
	{
		srand(time(0));
		carPos = { float((rand() % 501) - (rand() % 501)), float(-700 - (rand() % 1301)), 1000 };
		carVel = { 0, float(1200 + (rand() % 501)), 200 };
		carAngVel = { 0, 0, 0 };
		carRot = { 0, 16384, 0 };
		ballPos = { carPos.X, carPos.Y + 100, 1100 };
		ballVel = { 0, carVel.Y + 50, 200 };
		ballAngVel = { 0, 0, 0 };

		placementsDetermined = true;
	}
}

void PogoTrainer::applySettings()
{
	if (!settingsApplied && placementsDetermined)
	{
		srand(time(0));
		if (carOnlyMode())
		{
			ballPos = { 0, 4000, 100 };
			ballVel = { 0, 0, 0 };
		}
		if (offsetCarAndBall())
		{
			if (!carOnlyMode())
			{
				ballPos = { float(carPos.X + (rand() % 36) - (rand() % 36)), float(carPos.Y + 150 + (rand() % 151)), float(1100 + (rand() % 101)) };
				ballVel = { 0, float(carVel.Y + (rand() % 101)), 200 };
			}
		}
		if (randomizeCarOrientationOn())
		{
			carRot = { (rand() % 32000 - rand() % 32000), (rand() % 32000 - rand() % 32000), (rand() % 32000 - rand() % 32000) };
		}
		if (randomizeCarAngularVelocity())
		{
			carAngVel = { float(rand() % 1001 - rand() % 1001), float(rand() % 1001 - rand() % 1001), float(rand() % 1001 - rand() % 1001) };
		}
		if (randomizeBallAngularVelocity())
		{
			if (!carOnlyMode())
			{
				ballAngVel = { float(rand() % 2001 - rand() % 2001), float(rand() % 2001 - rand() % 2001), float(rand() % 2001 - rand() % 2001) };
			}
		}
		if (pogoRecoveryMode())
		{
			autoResetTimerMax = 300;
			carPos = { carPos.X, carPos.Y - 1000, carPos.Z + 300 };
			ballPos = { ballPos.X, ballPos.Y - 1000, ballPos.Z + 300 };
		}
		else
		{
			autoResetTimerMax = 120;
		}

		settingsApplied = true;
	}
}

void PogoTrainer::setShot()
{
	if (!shotSet && settingsApplied && placementsDetermined)
	{
		ServerWrapper server = gameWrapper->GetCurrentGameState();
		CarWrapper car = gameWrapper->GetLocalCar();
		BallWrapper ball = server.GetBall();
		if (!server || !car || !ball)
		{
			if (!server)
				LOG("There is no server (from setShot)");
			if (!car)
				LOG("There is no car (from setShot)");
			if (!ball)
				LOG("There is no ball (from setShot)");

			reset();
			disablePlugin();
			LOG("Plugin disabled");
		}

		car.SetLocation({carPos});
		car.SetVelocity({carVel});
		car.SetAngularVelocity({carAngVel}, false);
		car.SetRotation({carRot});
		ball.SetLocation({ballPos});
		ball.SetVelocity({ballVel});
		ball.SetAngularVelocity({ballAngVel}, false);
		
		shotSet = true;
	}

	if (shotSet && settingsApplied && placementsDetermined)
		shotCreated = true;
}

void PogoTrainer::reset()
{
	ServerWrapper server = gameWrapper->GetCurrentGameState();
	CarWrapper car = gameWrapper->GetLocalCar();
	BallWrapper ball = server.GetBall();
	if (!server || !car || !ball)
	{
		if (!server)
			LOG("There is no server (from reset)");
		if (!car)
			LOG("There is no car (from reset)");
		if (!ball)
			LOG("There is no ball (from reset)");

		reset();
		disablePlugin();
		LOG("Plugin disabled");
	}
	placementsDetermined = false;
	settingsApplied = false;
	shotSet = false;
	shotCreated = false;
	resetTimer = 50;
	autoResetTimer = -1;

	car.SetFrozen(false);
	ball.SetFrozen(false);
}

void PogoTrainer::autoReset()
{
	ServerWrapper server = gameWrapper->GetCurrentGameState();
	BallWrapper ball = server.GetBall();
	CarWrapper car = gameWrapper->GetLocalCar();
	if (!car)
	{
		disablePlugin();
		LOG("Plugin disabled because there is no car in autoReset().");
	}

	if (car.GetLocation().Z < 200 && autoResetTimer == -1)
		autoResetTimer = autoResetTimerMax;

	if (autoResetTimer != -1)
	{
		autoResetTimer--;
		if (autoResetTimer == 0)
			reset();
		if (autoResetTimer < 10)
		{
			car.SetRotation({ 0, 0, 0 });
			car.SetLocation({ car.GetLocation().X, car.GetLocation().Y, 17 });
		}
	}
}

void PogoTrainer::freeze()
{
	ServerWrapper server = gameWrapper->GetCurrentGameState();
	CarWrapper car = gameWrapper->GetLocalCar();
	BallWrapper ball = server.GetBall();
	if (!server || !car || !ball)
	{
		if (!server)
			LOG("There is no server (from freeze)");
		if (!car)
			LOG("There is no car (from freeze)");
		if (!ball)
			LOG("There is no ball (from freeze)");

		reset();
		disablePlugin();
		LOG("Plugin disabled");
	}

	if (ball.GetLocation().Y > 5000)
	{
		ball.SetVelocity({ 0, 0 ,0 });
		car.SetVelocity({ 0, 0, 0 });
		ball.SetFrozen(true);
		car.SetFrozen(true);
		if (autoResetTimer == -1 && freezeTimer == 0)
		{
			autoResetTimer = 60;
			freezeTimer = 100;
		}
	}
}

void PogoTrainer::closeMenu()
{
	cvarManager->executeCommand("togglemenu settings");
	shouldCloseMenu = false;
}

void PogoTrainer::loadHooks()
{
	gameWrapper->HookEvent("Function TAGame.Car_TA.SetVehicleInput",
		[this](std::string eventName) {
			if (pluginActive())
				controller();
		});

	gameWrapper->HookEvent("Function TAGame.FreeplayCommands_TA.ResetTraining",
		[this](std::string eventName) {
			if (pluginActive())
				reset();
		});
}