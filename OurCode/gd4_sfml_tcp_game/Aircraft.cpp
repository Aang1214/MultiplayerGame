/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#include "Aircraft.hpp"
#include "TextureID.hpp"
#include "ResourceHolder.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include "DataTables.hpp"
#include "Projectile.hpp"
#include "PickupType.hpp"
#include "Pickup.hpp"
#include "SoundNode.hpp"
#include "EmitterNode.hpp"
#include "ColourID.hpp"
#include "Colours.hpp"
#include "HiddenTextNode.h"

namespace
{
	// Initialize aircraft data
	const std::vector<AircraftData> Table = InitializeAircraftData();
}

//Helper function to convert AircraftType to TextureID
// P1 
TextureID ToTextureID(AircraftType type)
{
	switch (type)
	{
	case AircraftType::kP1:
		return TextureID::kPlayer;
		break;
	case AircraftType::kMeteorA:
		return TextureID::kMeteor;
		break;
	}
	return TextureID::kPlayer;
}

// Constructor for player aircraft
Aircraft::Aircraft(AircraftType type, const TextureHolder& textures, const FontHolder& fonts)  
	: Entity(Table[static_cast<int>(type)].m_hitpoints) //keep health points
	, m_type(type) //keep type
	, m_sprite(textures.Get(Table[static_cast<int>(type)].m_texture), Table[static_cast<int>(type)].m_texture_rect) //keep texture
	, m_explosion(textures.Get(TextureID::kExplosion)) //keep explosion texture
	, m_health_display(nullptr)
	, m_missile_display(nullptr) //replace with powerup display
	, m_distance_travelled(0.f) //remove
	, m_directions_index(0) //**
	, m_fire_rate(1) //remove
	, m_spread_level(1) //remove
	, m_is_firing(false) //remove
	, m_is_launching_missile(false) //keep
	, m_fire_countdown(sf::Time::Zero) //remove
	, m_missile_ammo(100) //change (start with 0)
	, m_is_marked_for_removal(false) //remove
	, m_show_explosion(true) //keep
	, m_spawned_pickup(false) //keep
	, m_played_explosion_sound(false) //keep

{
	m_explosion.SetFrameSize(sf::Vector2i(256, 256)); //change for texture
	m_explosion.SetNumFrames(16); //change for texture
	m_explosion.SetDuration(sf::seconds(1)); //change for texture
	Utility::CentreOrigin(m_sprite); //keep
	Utility::CentreOrigin(m_explosion); //keep

	m_fire_command.category = static_cast<int>(ReceiverCategories::kScene); //remove
	m_fire_command.action = [this, &textures](SceneNode& node, sf::Time dt) //remove
		{
			CreateBullet(node, textures);
		};

	//Missile command (power up)
	m_missile_command.category = static_cast<int>(ReceiverCategories::kScene); //keep
	m_missile_command.action = [this, &textures](SceneNode& node, sf::Time dt) //keep
		{
			CreateProjectile(node, ProjectileType::kMissile, 0.f, 0.5f, textures);
		};

	//Drop pickup command (move to spawner)
	m_drop_pickup_command.category = static_cast<int>(ReceiverCategories::kScene);
	m_drop_pickup_command.action = [this, &textures](SceneNode& node, sf::Time dt)
		{
			CreatePickup(node, textures);
		};

	std::string health = "";  // No need for dynamic allocation
	std::unique_ptr<HiddenTextNode> health_display = std::make_unique<HiddenTextNode>(fonts, health);
	m_health_display = health_display.get();
	static_cast<HiddenTextNode*>(m_health_display)->SetVisible(false);
	//m_health_display->SetVisible(false);
	AttachChild(std::move(health_display));

	if (Aircraft::GetCategory() == static_cast<int>(ReceiverCategories::kP1))
	{
		static_cast<HiddenTextNode*>(m_health_display)->SetVisible(true);
		//m_health_display->SetVisible(true);
		std::unique_ptr<EmitterNode> propellant(new EmitterNode(ParticleType::kPlayerPropellant));
		propellant->setPosition(0.f, GetBoundingRect().height / 2.f);
		AttachChild(std::move(propellant));

		std::unique_ptr<EmitterNode> smoke(new EmitterNode(ParticleType::kSmoke));
		smoke->setPosition(0.f, GetBoundingRect().height / 2.f);
		AttachChild(std::move(smoke));
	}
	//ChangePlayerColor(); find
	UpdateTexts();//keep
}

int	Aircraft::GetIdentifier()
{
	return m_identifier;
}

void Aircraft::SetIdentifier(int identifier)
{
	m_identifier = identifier;
}

//****************************************************************************************************
unsigned int Aircraft::GetCategory() const
{
	if (IsP1())
	{
		return static_cast<unsigned int>(ReceiverCategories::kP1);
	}
	else 
	{
		return static_cast<unsigned int>(ReceiverCategories::kMeteors);
	}
}

// remove
void Aircraft::IncreaseFireRate()
{
	if (m_fire_rate < 5)
	{
		++m_fire_rate;
	}
}

// remove
void Aircraft::IncreaseFireSpread()
{
	if (m_spread_level < 3)
	{
		++m_spread_level;
	}
}

// keep
void Aircraft::CollectMissile(unsigned int count)
{
	m_missile_ammo += count;
}

// keep
void Aircraft::UpdateTexts()
{
	if (IsDestroyed())
	{
		m_health_display->SetString("");
	}
	else
	{
		m_health_display->SetString(std::to_string(GetHitPoints()) + "HP");
	}
	m_health_display->setPosition(0.f, 50.f);
	m_health_display->setRotation(-getRotation());

	if (m_missile_display)
	{
		m_missile_display->setPosition(0.f, 70.f);
		if (m_missile_ammo == 0)
		{
			m_missile_display->SetString("");
		}
		else
		{
			m_missile_display->SetString("M: " + std::to_string(m_missile_ammo));
		}
	}
}

// remove
void Aircraft::UpdateMovementPattern(sf::Time dt)
{
	//Enemy AI (remove)
	const std::vector<Direction>& directions = Table[static_cast<int>(m_type)].m_directions;
	if (!directions.empty())
	{
		//Move along the current direction, then change direction
		if (m_distance_travelled > directions[m_directions_index].m_distance)
		{
			m_directions_index = (m_directions_index + 1) % directions.size();
			m_distance_travelled = 0.f;
		}

		//Compute velocity
		//Add 90 to move down the screen, 0 is right

		double radians = Utility::ToRadians(directions[m_directions_index].m_angle + 90.f);
		float vx = GetMaxSpeed() * std::cos(radians);
		float vy = GetMaxSpeed() * std::sin(radians);

		SetVelocity(vx, vy);
		m_distance_travelled += GetMaxSpeed() * dt.asSeconds();
	}
}

//keep
float Aircraft::GetMaxSpeed() const
{
	return Table[static_cast<int>(m_type)].m_speed;
}

// remove
void Aircraft::Fire()
{
	if (Table[static_cast<int>(m_type)].m_fire_interval != sf::Time::Zero)
	{
		m_is_firing = true;
	}
}

//rename to use powerup
void Aircraft::LaunchMissile()
{
	if (m_missile_ammo > 0)
	{
		m_is_launching_missile = true;
		m_missile_ammo;
	}
}

// remove
void Aircraft::CreateBullet(SceneNode& node, const TextureHolder& textures) const
{
	ProjectileType type = IsP1() ? ProjectileType::kAlliedBullet : ProjectileType::kEnemyBullet;
	switch (m_spread_level)
	{
	case 1:
		CreateProjectile(node, type, 0.0f, 0.5f, textures);
		break;
	case 2:
		CreateProjectile(node, type, -0.5f, 0.5f, textures);
		CreateProjectile(node, type, 0.5f, 0.5f, textures);
		break;
	case 3:
		CreateProjectile(node, type, 0.0f, 0.5f, textures);
		CreateProjectile(node, type, -0.5f, 0.5f, textures);
		CreateProjectile(node, type, 0.5f, 0.5f, textures);
		break;
	}
	
}

//keep (change for texture/ powerup)
void Aircraft::CreateProjectile(SceneNode& node, ProjectileType type, float x_offset, float y_offset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));
	sf::Vector2f offset(x_offset * m_sprite.getGlobalBounds().width, y_offset * m_sprite.getGlobalBounds().height);

	// Select the correct rotation based on the ID
	float rot = Aircraft::GetRotation();

	// Convert degrees to radians for trigonometric functions
	float radian = rot * 3.14159265f / 180.0f;

	// Calculate the forward velocity based on the angle
	sf::Vector2f velocity(std::cos(radian) * projectile->GetMaxSpeed(), std::sin(radian) * projectile->GetMaxSpeed());

	//set rotation of projectile to match aircraft
	projectile->setPosition(GetWorldPosition() + offset);
	projectile->SetVelocity(velocity);
	projectile->setRotation(rot + 90.f);
	node.AttachChild(std::move(projectile));
}

// keep

sf::FloatRect Aircraft::GetBoundingRect() const
{
	if (IsP1()) {
		sf::Vector2f position = this->getPosition();
		sf::Vector2f hitboxSize(20.f, 20.f);
		sf::FloatRect hitbox(position.x - hitboxSize.x / 2, position.y - hitboxSize.y / 2, hitboxSize.x, hitboxSize.y);

		return hitbox;
	}
	return GetWorldTransform().transformRect(m_sprite.getGlobalBounds());
}

// keepF
bool Aircraft::IsMarkedForRemoval() const
{
	return IsDestroyed();
}

// keep
void Aircraft::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (IsDestroyed() && m_show_explosion)
	{
		
	}
	else
	{
		target.draw(m_sprite, states);
	}
}

// keep
void Aircraft::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	

	if (IsDestroyed())
	{
		CheckPickupDrop(commands);
		m_explosion.Update(dt);
		// Play explosion sound only once
		if (!m_played_explosion_sound)
		{
			SoundEffect soundEffect = (Utility::RandomInt(2) == 0) ? SoundEffect::kExplosion1 : SoundEffect::kExplosion2;
			PlayLocalSound(commands, soundEffect);

			m_played_explosion_sound = true;
		}
		return;
	}

	Entity::UpdateCurrent(dt, commands);
	UpdateTexts();
	UpdateMovementPattern(dt);

	UpdateRollAnimation();

	//Check if bullets or misiles are fired
	CheckProjectileLaunch(dt, commands);
}


// keep
void Aircraft::CheckProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	if (m_is_firing && m_fire_countdown <= sf::Time::Zero)
	{
		PlayLocalSound(commands, IsP1() ? SoundEffect::kEnemyGunfire : SoundEffect::kAlliedGunfire);
		commands.Push(m_fire_command);
		m_fire_countdown += Table[static_cast<int>(m_type)].m_fire_interval / (m_fire_rate + 1.f);
		m_is_firing = false;
	}
	else if (m_fire_countdown > sf::Time::Zero)
	{
		//Wait, can't fire
		m_fire_countdown -= dt;
		m_is_firing = false;
	}

	//Missile launch
	if (m_is_launching_missile && m_fire_countdown <= sf::Time::Zero)
	{
		PlayLocalSound(commands, SoundEffect::kLaunchMissile);
		commands.Push(m_missile_command);
		m_fire_countdown += Table[static_cast<int>(m_type)].m_fire_interval / (m_fire_rate + 1.f);
		m_is_launching_missile = false;
	}
	else if (m_fire_countdown > sf::Time::Zero)
	{
		//Wait, can't fire
		m_fire_countdown -= dt;
		m_is_launching_missile = false;
	}
}

// keep
bool Aircraft::IsP1() const
{
	return m_type == AircraftType::kP1;
}

// move to spawner
void Aircraft::CreatePickup(SceneNode& node, const TextureHolder& textures) const
{
	auto type = static_cast<PickupType>(Utility::RandomInt(static_cast<int>(PickupType::kPickupCount)));
	std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
	pickup->setPosition(GetWorldPosition());
	pickup->SetVelocity(0.f, 0.f);
	node.AttachChild(std::move(pickup));
}

// keep (change for powerup)
void Aircraft::CheckPickupDrop(CommandQueue& commands)
{
	//TODO Get rid of the magic number 3 here 
	if (!IsP1() && Utility::RandomInt(3) == 0 && !m_spawned_pickup)
	{
		commands.Push(m_drop_pickup_command);
	}
	m_spawned_pickup = true;
}

// rename to tilt
void Aircraft::UpdateRollAnimation()
{
	if (Table[static_cast<int>(m_type)].m_has_roll_animation)
	{
		sf::IntRect textureRect = Table[static_cast<int>(m_type)].m_texture_rect;

		//Roll left: Texture rect is offset once
		if (GetVelocity().x < 0.f)
		{
			textureRect.left += textureRect.width;
		}
		else if (GetVelocity().x > 0.f)
		{
			textureRect.left += 2 * textureRect.width;
		}
		m_sprite.setTextureRect(textureRect);

	}
}

// keep
void Aircraft::PlayLocalSound(CommandQueue& commands, SoundEffect effect)
{
	sf::Vector2f world_position = GetWorldPosition();

	Command command;
	command.category = static_cast<int>(ReceiverCategories::kSoundEffect);
	command.action = DerivedAction<SoundNode>(
		[effect, world_position](SoundNode& node, sf::Time)
		{
			node.PlaySound(effect, world_position);
		});

	commands.Push(command);
}

void Aircraft::ChangePlayerColor(sf::Int32 aircraft_identifier) {
	if (IsP1()) {

		ColourID colourId = Colours::GetColourID(aircraft_identifier); 
		sf::Color colour = Colours::GetColour(colourId); 

		m_sprite.setColor(colour);
	}
}

void Aircraft::ChangeParticleColour(sf::Int32 aircraft_identifier)
{
	std::unique_ptr<EmitterNode> propellant(new EmitterNode(ParticleType::kPlayerPropellant));

	ColourID colourId = Colours::GetColourID(aircraft_identifier);
	sf::Color colour = Colours::GetColour(colourId);

	propellant->SetColour(colour);
}