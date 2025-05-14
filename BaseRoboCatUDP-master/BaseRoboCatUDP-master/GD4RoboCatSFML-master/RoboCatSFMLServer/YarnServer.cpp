#include "RoboCatServerPCH.hpp"



YarnServer::YarnServer()
{
	//yarn lives a second...
	mTimeToDie = Timing::sInstance.GetFrameStartTime() + 1.f;
}

void YarnServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}

bool YarnServer::HandleCollisionWithMouse(Mouse* inMouse)
{
	// Get the current velocity of the Yarn
	Vector3 yarnVelocity = GetVelocity();

	// Transfer the Yarn's velocity to the Mouse
	inMouse->SetVelocity(yarnVelocity);

	// Destroy the Yarn
	SetDoesWantToDie(true);

	return true;
}
void YarnServer::Update()
{

	Yarn::Update();

	const Vector3& pos = GetLocation();
	if (pos.mX < 0.0f || pos.mX > 1920.0f || pos.mY < 0.0f || pos.mY > 1080.0f) 
	{
		SetDoesWantToDie(true); 
	}

}

/*
bool YarnServer::HandleCollisionWithCat(RoboCat* inCat)
{
	if (inCat->GetPlayerId() != GetPlayerId())
	{
		//kill yourself! yes
		SetDoesWantToDie(true);

		static_cast<RoboCatServer*>(inCat)->TakeDamage(GetPlayerId());

	}

	return false;
}*/



