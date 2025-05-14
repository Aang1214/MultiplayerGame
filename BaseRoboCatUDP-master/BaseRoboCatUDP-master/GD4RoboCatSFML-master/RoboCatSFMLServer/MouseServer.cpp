#include "RoboCatServerPCH.hpp"

MouseServer::MouseServer()
{
}

void MouseServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}




bool MouseServer::HandleCollisionWithCat(RoboCat* inCat)
{
	//kill yourself!
	//SetDoesWantToDie(true);
	//ScoreBoardManager::sInstance->IncScore(inCat->GetPlayerId(), 1);
    // Knockback logic: transfer Cat's velocity to the mouse
    Vector3 initCatVel = inCat->GetVelocity();
    sf::Vector2f catVelocity = sf::Vector2f(initCatVel.GetX(), initCatVel.GetY());
    Vector3 initMouseVel = GetVelocity();
    sf::Vector2f mouseVelocity = sf::Vector2f(initMouseVel.GetX(), initMouseVel.GetY());
    // Simple elastic-ish collision response
    if (catVelocity == sf::Vector2f(0.f, 0.f))
    {
        sf::Vector2f convert = mouseVelocity / 2.f;
        Vector3 result(convert.x, convert.y, 0);
        SetVelocity(result); // mouse gets pushed slightly
    }
    else
    {
        sf::Vector2f convert = catVelocity + mouseVelocity * 0.5f;
        Vector3 result(convert.x, convert.y, 0);
        SetVelocity(result); // combine velocity influence
    }

    if (inCat->GetPlayerId() != GetPlayerId())
    {
        //kill yourself!
        //SetDoesWantToDie(true);

        static_cast<RoboCatServer*>(inCat)->TakeDamage(GetPlayerId());

    }

    // Mouse "dies"
   // SetDoesWantToDie(true);

    // Update score
   // ScoreBoardManager::sInstance->IncScore(inCat->GetPlayerId(), 1);

    return true;
}
void MouseServer::Update()
{
    Mouse::Update();



}
/*
bool MouseServer::HandleCollisionWithMouse(Mouse* inMouse)
{
    
    Vector3 initMouseVel = inMouse->GetVelocity();
    sf::Vector2f mouseVelocity(initMouseVel.GetX(), initMouseVel.GetY());

    // Swap velocities (basic elastic collision)
    if (mouseVelocity == sf::Vector2f(0.f, 0.f))
    {
        SetVelocity(otherVelocity);
        inMouse->SetVelocity(otherVelocity / 2.f); // reduce the other slightly
    }
    else if (otherVelocity == sf::Vector2f(0.f, 0.f))
    {
        inMouse->SetVelocity(myVelocity);
        SetVelocity(myVelocity / 2.f);
    }
    else
    {
        SetVelocity(otherVelocity);
        inMouse->SetVelocity(myVelocity);
    }

    return true;
}*/







