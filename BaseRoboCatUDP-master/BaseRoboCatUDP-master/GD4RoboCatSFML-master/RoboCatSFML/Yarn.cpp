#include "RoboCatPCH.hpp"
const float WORLD_HEIGHT = 1080.f;
const float WORLD_WIDTH = 1920.f;

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
	/*Vector3 location = GetLocation();
	float x = location.mX;
	float y = location.mY;

	float vx = mVelocity.mX;
	float vy = mVelocity.mY;

	float radius = GetCollisionRadius();

	//if the cat collides against a wall, the quick solution is to push it off
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
	}*/

	Vector3 location = GetLocation();
	Vector3 velocity = mVelocity;
	float radius = GetCollisionRadius();

	bool bounced = false;

	// Bottom Wall
	if (location.mY + radius >= WORLD_HEIGHT && velocity.mY > 0)
	{
		location.mY = WORLD_HEIGHT - radius;
		velocity.mY = -velocity.mY * mWallRestitution;
		bounced = true;
	}
	// Top Wall
	else if (location.mY - radius <= 0 && velocity.mY < 0)
	{
		location.mY = radius;
		velocity.mY = -velocity.mY * mWallRestitution;
		bounced = true;
	}

	// Right Wall
	if (location.mX + radius >= WORLD_WIDTH && velocity.mX > 0)
	{
		location.mX = WORLD_WIDTH - radius;
		velocity.mX = -velocity.mX * mWallRestitution;
		bounced = true;
	}
	// Left Wall
	else if (location.mX - radius <= 0 && velocity.mX < 0)
	{
		location.mX = radius;
		velocity.mX = -velocity.mX * mWallRestitution;
		bounced = true;
	}

	if (bounced)
	{
		SetLocation(location);
		SetVelocity(velocity);
	}
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

