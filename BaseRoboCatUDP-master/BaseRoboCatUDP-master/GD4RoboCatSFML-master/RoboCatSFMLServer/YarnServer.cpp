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
	Vector3 initMouseVel = inMouse->GetVelocity();
	sf::Vector2f mouseVelocity = sf::Vector2f(initMouseVel.GetX(), initMouseVel.GetY());
	Vector3 localMouseVel = GetVelocity();
	sf::Vector2f localVelocity = sf::Vector2f(initMouseVel.GetX(), initMouseVel.GetY());
	// Simple elastic-ish collision response
	if (localVelocity == sf::Vector2f(0.f, 0.f))
	{
		sf::Vector2f convert = localVelocity / 2.f;
		Vector3 result(convert.x, convert.y, 0);
		SetVelocity(result); // mouse gets pushed slightly
	}
	else
	{
		sf::Vector2f convert = localVelocity + mouseVelocity * 0.5f;
		Vector3 result(convert.x, convert.y, 0);
		SetVelocity(result); // combine velocity influence
	}
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



