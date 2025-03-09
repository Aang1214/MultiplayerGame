/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#pragma once
/*
	knone = 0000 0000
	kscene = 0000 0001
	kP1 = 0000 0010
	kPowerUp = 0000 1000
	kPickup = 0001 0000
	kParticleSystem = 0010 0000
	kSoundEffect = 0100 0000
*/
enum class ReceiverCategories
{
	kNone = 0,
	kScene = 1 << 0,
	kP1 = 1 << 1,
	kMeteors = 1 << 3,
	kAlliedProjectile = 1 << 4,
	kEnemyProjectile = 1 << 5,
	kPickup = 1 << 6,
	kParticleSystem = 1 << 7,
	kSoundEffect = 1 << 8, 
	kNetwork = 1 << 9,

	kAircraft = kP1 | kMeteors,
	kProjectile = kAlliedProjectile | kEnemyProjectile
};

// A message would be sent to all aircraft
//unsigned int all_aircraft = ReceiverCategories::kPlayerAircraft | ReceiverCategories::kAlliedAircraft | ReceiverCategories::kEnemyAircraft