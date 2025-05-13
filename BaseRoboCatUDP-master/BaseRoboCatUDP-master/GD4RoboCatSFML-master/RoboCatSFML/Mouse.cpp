#include "RoboCatPCH.hpp"

Mouse::Mouse() :
mVelocity(Vector3::Zero)
{
	SetScale(GetScale() * 0.5f);
	SetCollisionRadius(20.f);
}

sf::Vector2f Mouse::GetVelocity()
{
	return sf::Vector2f(mVelocity.GetX(), mVelocity.GetY());
}

void Mouse::SetVelocity(sf::Vector2f velocity)
{
	mVelocity.SetX(velocity.x);
	mVelocity.SetY(velocity.y);
}

void Mouse::Update()
{
	float deltaTime = Timing::sInstance.GetDeltaTime();
	// position += velocity * deltaTime
	Vector3 newPos = GetLocation() + mVelocity * deltaTime;
	SetLocation(newPos);

	// Optional: friction
	mVelocity = mVelocity * 0.98f; // slow down slightly
}

bool Mouse::HandleCollisionWithCat(RoboCat* inCat)
{
	(void)inCat;
	return false;
}

bool Mouse::HandleCollisionWithMouse(Mouse* inMouse)
{
	(void)inMouse;
	return false;
}




uint32_t Mouse::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const
{
	uint32_t writtenState = 0;

	if (inDirtyState & EMRS_Pose)
	{
		inOutputStream.Write((bool)true);

		Vector3 location = GetLocation();
		inOutputStream.Write(location.mX);
		inOutputStream.Write(location.mY);

		inOutputStream.Write(GetRotation());

		writtenState |= EMRS_Pose;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	if (inDirtyState & EMRS_Color)
	{
		inOutputStream.Write((bool)true);

		inOutputStream.Write(GetColor());

		writtenState |= EMRS_Color;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}


	return writtenState;
}



