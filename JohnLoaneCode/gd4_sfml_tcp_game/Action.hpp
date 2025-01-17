#pragma once
//Controls the actions that the player can take
/*
	kP1MoveForward,
	kP1TiltLeft,
	kP1TiltRight,
	kP1UsePowerup,

	kP2MoveForward,
	kP2TiltLeft,
	kP2TiltRight,
	kP2UsePowerup,

	kSpawnMeteor
*/
enum class Action
{
	kMoveLeft,
	kMoveRight,
	kMoveUp,
	kMoveDown,
	kMissileFire,
	kBulletFire,
	kActionCount
};