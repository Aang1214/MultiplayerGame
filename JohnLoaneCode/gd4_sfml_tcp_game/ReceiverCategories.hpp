#pragma once
/*
	knone = 0000 0000
	kscene = 0000 0001
	kP1 = 0000 0010
	kP2 = 0000 0100
	kP1PowerUp = 0000 1000
	kP2PowerUp = 0001 0000
	kPickup = 0010 0000
	kParticleSystem = 0100 0000
	kSoundEffect = 1000 0000
*/
enum class ReceiverCategories
{
	kNone = 0,
	kScene = 1 << 0,
	kPlayerAircraft = 1 << 1,
	kAlliedAircraft = 1 << 2,
	kEnemyAircraft = 1 << 3,
	kAlliedProjectile = 1 << 4,
	kEnemyProjectile = 1 << 5,
	kPickup = 1 << 6,
	kParticleSystem = 1 << 7,
	kSoundEffect = 1 << 8,

	kAircraft = kPlayerAircraft | kAlliedAircraft | kEnemyAircraft,
	kProjectile = kAlliedProjectile | kEnemyProjectile
};

// A message would be sent to all aircraft
//unsigned int all_aircraft = ReceiverCategories::kPlayerAircraft | ReceiverCategories::kAlliedAircraft | ReceiverCategories::kEnemyAircraft