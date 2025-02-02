/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#pragma once
//Controls the actions that the player can take
enum class Action
{
	kP1TiltLeft,
	kP1TiltRight,
	kP1MoveUp,

	kP2TiltLeft,
	kP2TiltRight,
	kP2MoveUp,

	kP1UsePowerUp, 
	kP2UsePowerUp,

	kMeteorSpawn,
	kActionCount
};