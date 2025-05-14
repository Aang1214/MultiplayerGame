#include "RoboCatPCH.hpp"
const float WORLD_HEIGHT = 1080.f;
const float WORLD_WIDTH = 1920.f;
template <typename T>
T Clamp(T value, T min, T max)
{
	return (value < min) ? min : (value > max) ? max : value;
}
Yarn::Yarn() :
	mMuzzleSpeed(300.f),
	mVelocity(Vector3::Zero),
	mPlayerId(0)
{
	SetScale(GetScale() * 0.25f);
	SetCollisionRadius(20.f);
}



uint32_t Yarn::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const
{
	uint32_t writtenState = 0;

	if (inDirtyState & EYRS_Pose)
	{
		inOutputStream.Write((bool)true);

		Vector3 location = GetLocation();
		inOutputStream.Write(location.mX);
		inOutputStream.Write(location.mY);

		Vector3 velocity = GetVelocity();
		inOutputStream.Write(velocity.mX);
		inOutputStream.Write(velocity.mY);

		inOutputStream.Write(GetRotation());

		writtenState |= EYRS_Pose;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	if (inDirtyState & EYRS_Color)
	{
		inOutputStream.Write((bool)true);

		inOutputStream.Write(GetColor());

		writtenState |= EYRS_Color;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	if (inDirtyState & EYRS_PlayerId)
	{
		inOutputStream.Write((bool)true);

		inOutputStream.Write(mPlayerId, 8);

		writtenState |= EYRS_PlayerId;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}
	return writtenState;
}



bool Yarn::HandleCollisionWithCat(RoboCat* inCat)
{
	(void)inCat;

	//you hit a cat, so look like you hit a cat
	return false;
}

void Yarn::ProcessCollisionsWithScreenWalls()
{
	/*
	Vector3 location = GetLocation();
	float x = location.mX;
	float y = location.mY;

	float vx = mVelocity.mX;
	float vy = mVelocity.mY;

	float radius = GetCollisionRadius();

	
	if ((y + radius) >= WORLD_HEIGHT && vy > 0)
	{
		mVelocity.mY = -vy * mWallRestitution;
		location.mY = WORLD_HEIGHT - radius;
		SetLocation(location);
	}
	else if (y - radius <= 0 && vy < 0)
	{
		mVelocity.mY = -vy * mWallRestitution;
		location.mY = radius;
		SetLocation(location);
	}

	if ((x + radius) >= WORLD_WIDTH && vx > 0)
	{
		mVelocity.mX = -vx * mWallRestitution;
		location.mX = WORLD_WIDTH - radius;
		SetLocation(location);
	}
	else if (x - radius <= 0 && vx < 0)
	{
		mVelocity.mX = -vx * mWallRestitution;
		location.mX = radius;
		SetLocation(location);
	}
	*/
	///////////////////////////

	Vector3 location = GetLocation();
	Vector3 velocity = GetVelocity();
	float radius = GetCollisionRadius();

	// Define boundaries
	float left = 0.f + radius;
	float right = WORLD_WIDTH - radius;
	float top = 0.f + radius;
	float bottom = WORLD_HEIGHT - radius; 

	// Horizontal bounce (left/right walls)
	if (location.mX < left || location.mX > right)
	{
		velocity.mX = -velocity.mX;// *mWallRestitution;

		// Clamp location to inside the world
		location.mX = std::max(left, std::min(location.mX, right));
	}

	// Vertical bounce (top/bottom walls)
	if (location.mY < top || location.mY > bottom)
	{
		velocity.mY = -velocity.mY;// *mWallRestitution;

		// Clamp location to inside the world
		location.mY = std::max(top, std::min(location.mY, bottom));
	}

	SetVelocity(velocity);
	SetLocation(location);

}

void Yarn::InitFromShooter(RoboCat* inShooter)
{
	SetColor(inShooter->GetColor());
	SetPlayerId(inShooter->GetPlayerId());

	Vector3 forward = inShooter->GetForwardVector();
	SetVelocity(inShooter->GetVelocity() + forward * mMuzzleSpeed);
	SetLocation(inShooter->GetLocation() /* + forward * 0.55f */);

	SetRotation(inShooter->GetRotation());
}

void Yarn::Update()
{
	ProcessCollisionsWithScreenWalls();
	float deltaTime = Timing::sInstance.GetDeltaTime();

	SetLocation(GetLocation() + mVelocity * deltaTime);


	//we'll let the cats handle the collisions
}

