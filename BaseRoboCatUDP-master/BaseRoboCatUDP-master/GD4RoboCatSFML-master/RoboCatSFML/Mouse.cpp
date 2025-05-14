#include "RoboCatPCH.hpp"
const float WORLD_HEIGHT = 1080.f;
const float WORLD_WIDTH = 1920.f;
Mouse::Mouse() :
mVelocity(Vector3::Zero),
mPlayerId(0)
{
	SetScale(GetScale() * 2.f);
	SetCollisionRadius(55.f);
}



void Mouse::SetVelocity(Vector3 velocity)
{
	mVelocity.SetX(velocity.mX);
	mVelocity.SetY(velocity.mY);
}

void Mouse::Update()
{
	ProcessCollisionsWithScreenWalls();
	ProcessCollisions();
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

void Mouse::ProcessCollisions()
{
	//ProcessCollisionsWithScreenWalls();

	float sourceRadius = GetCollisionRadius();
	Vector3 sourceLocation = GetLocation();

	for (auto goIt = World::sInstance->GetGameObjects().begin(), end = World::sInstance->GetGameObjects().end(); goIt != end; ++goIt)
	{
		GameObject* target = goIt->get();
		if (target != this && !target->DoesWantToDie())
		{
			// Only collide with other mice
			/*Mouse* targetMouse = target->GetAsMouse();
			if (!targetMouse)
			{
				//continue;
			}*/

			Vector3 targetLocation = target->GetLocation();
			float targetRadius = target->GetCollisionRadius();

			Vector3 delta = targetLocation - sourceLocation;
			float distSq = delta.LengthSq2D();
			float collisionDist = (sourceRadius + targetRadius);

			if (distSq < (collisionDist * collisionDist))
			{
				// Let both mice know a collision occurred
				
				if (target->HandleCollisionWithMouse(this))
				{
					Vector3 dirToTarget = delta;
					dirToTarget.Normalize2D();
					Vector3 acceptableDelta = dirToTarget * collisionDist;
					SetLocation(targetLocation - acceptableDelta);

					Vector3 relVel = mVelocity;

					Mouse* targetMouse = target->GetAsMouse();

					float relVelDotDir = Dot2D(relVel, dirToTarget);

					if (relVelDotDir > 0.f)
					{
						Vector3 impulse = relVelDotDir * dirToTarget;

						// Apply equal and opposite impulses
						if (targetMouse)
						{
							mVelocity -= impulse;
							mVelocity *= mMouseRestitution;
						}
						else
						{
							mVelocity -= impulse * 2.f;
							mVelocity *= mWallRestitution;
						}
					}
				}
			}
		}
	}
}

void Mouse::ProcessCollisionsWithScreenWalls()
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
	float left = radius;
	float right = WORLD_WIDTH - radius;
	float top = radius;
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




uint32_t Mouse::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const
{
	uint32_t writtenState = 0;

	if (inDirtyState & EMRS_Pose)
	{
		inOutputStream.Write((bool)true);

		Vector3 location = GetLocation();
		inOutputStream.Write(location.mX);
		inOutputStream.Write(location.mY);

		Vector3 velocity = GetVelocity();
		inOutputStream.Write(velocity.mX);
		inOutputStream.Write(velocity.mY);

		writtenState |= EMRS_Pose;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}
	/**/
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



