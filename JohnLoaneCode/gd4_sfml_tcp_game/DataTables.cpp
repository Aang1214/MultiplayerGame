/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#include "DataTables.hpp"
#include "AircraftType.hpp"
#include "ProjectileType.hpp"
#include "PickupType.hpp"
#include "Aircraft.hpp"
#include "ParticleType.hpp"

std::vector<AircraftData> InitializeAircraftData()
{
    std::vector<AircraftData> data(static_cast<int>(AircraftType::kAircraftCount));

    //p1
    data[static_cast<int>(AircraftType::kP1)].m_hitpoints = 3;
    data[static_cast<int>(AircraftType::kP1)].m_speed = 200.f;
    data[static_cast<int>(AircraftType::kP1)].m_fire_interval = sf::seconds(5);
    data[static_cast<int>(AircraftType::kP1)].m_texture = TextureID::kEagle;
    data[static_cast<int>(AircraftType::kP1)].m_texture_rect = sf::IntRect(0, 0, 32, 32);
    data[static_cast<int>(AircraftType::kP1)].m_has_roll_animation = true;

    // Large meteor
    data[static_cast<int>(AircraftType::kMeteorA)].m_hitpoints = 100;
    data[static_cast<int>(AircraftType::kMeteorA)].m_speed = 50.f;
    data[static_cast<int>(AircraftType::kMeteorA)].m_texture = TextureID::kEntities;
    data[static_cast<int>(AircraftType::kMeteorA)].m_texture_rect = sf::IntRect(0, 64, 96, 64);
    data[static_cast<int>(AircraftType::kMeteorA)].m_has_roll_animation = false;

    // Small meteor
    data[static_cast<int>(AircraftType::kMeteorB)].m_hitpoints = 100;
    data[static_cast<int>(AircraftType::kMeteorB)].m_speed = 50.f;
    data[static_cast<int>(AircraftType::kMeteorB)].m_texture = TextureID::kEntities;
    data[static_cast<int>(AircraftType::kMeteorB)].m_texture_rect = sf::IntRect(96, 64, 64, 64);
    data[static_cast<int>(AircraftType::kMeteorB)].m_has_roll_animation = false;



    //AI for Avenger (remove)
    //data[static_cast<int>(AircraftType::kMeteorA)].m_directions.emplace_back(Direction(+45.f, 50.f));

    return data;
}

// keep and rework
std::vector<ProjectileData> InitializeProjectileData()
{
    std::vector<ProjectileData> data(static_cast<int>(ProjectileType::kProjectileCount));
    //allied bullet
    data[static_cast<int>(ProjectileType::kAlliedBullet)].m_damage = 10;
    data[static_cast<int>(ProjectileType::kAlliedBullet)].m_speed = 300;
    data[static_cast<int>(ProjectileType::kAlliedBullet)].m_texture = TextureID::kEntities;
    data[static_cast<int>(ProjectileType::kAlliedBullet)].m_texture_rect = sf::IntRect(96, 0, 8, 16);
    //enemy bullet
    data[static_cast<int>(ProjectileType::kEnemyBullet)].m_damage = 10;
    data[static_cast<int>(ProjectileType::kEnemyBullet)].m_speed = 300;
    data[static_cast<int>(ProjectileType::kEnemyBullet)].m_texture = TextureID::kEntities;
    data[static_cast<int>(ProjectileType::kEnemyBullet)].m_texture_rect = sf::IntRect(96, 0, 8, 16);

    //missile
    data[static_cast<int>(ProjectileType::kMissile)].m_damage = 200;
    data[static_cast<int>(ProjectileType::kMissile)].m_speed = 150;
    data[static_cast<int>(ProjectileType::kMissile)].m_texture = TextureID::kEntities;
    data[static_cast<int>(ProjectileType::kMissile)].m_texture_rect = sf::IntRect(96, 0, 8, 16);

    return data;
}

// keep and rework
std::vector<PickupData> InitializePickupData()
{
    std::vector<PickupData> data(static_cast<int>(PickupType::kPickupCount));
    data[static_cast<int>(PickupType::kHealthRefill)].m_texture = TextureID::kEntities;
    data[static_cast<int>(PickupType::kHealthRefill)].m_texture_rect = sf::IntRect(0, 64, 40, 40);
    data[static_cast<int>(PickupType::kHealthRefill)].m_action = [](Aircraft& a)
        {
            a.Repair(25);
        };

    data[static_cast<int>(PickupType::kMissileRefill)].m_texture = TextureID::kEntities;
    data[static_cast<int>(PickupType::kMissileRefill)].m_texture_rect = sf::IntRect(40, 64, 40, 40);
    
    data[static_cast<int>(PickupType::kMissileRefill)].m_action = std::bind(&Aircraft::CollectMissile, std::placeholders::_1, 3);

    data[static_cast<int>(PickupType::kFireSpread)].m_texture = TextureID::kEntities;
    data[static_cast<int>(PickupType::kFireSpread)].m_texture_rect = sf::IntRect(80, 64, 40, 40);
    data[static_cast<int>(PickupType::kFireSpread)].m_action = std::bind(&Aircraft::IncreaseFireSpread, std::placeholders::_1);

    data[static_cast<int>(PickupType::kFireRate)].m_texture = TextureID::kEntities;
    data[static_cast<int>(PickupType::kFireRate)].m_texture_rect = sf::IntRect(120, 64, 40, 40);
    data[static_cast<int>(PickupType::kFireRate)].m_action = std::bind(&Aircraft::IncreaseFireRate, std::placeholders::_1);
    
    return data;
}

// keep and rework
std::vector<ParticleData> InitializeParticleData()
{
    std::vector<ParticleData> data(static_cast<int>(ParticleType::kParticleCount));

    data[static_cast<int>(ParticleType::kPropellant)].m_color = sf::Color(115, 239, 232);
    data[static_cast<int>(ParticleType::kPropellant)].m_lifetime = sf::seconds(0.5f);

    data[static_cast<int>(ParticleType::kSmoke)].m_color = sf::Color(50, 50, 50);
    data[static_cast<int>(ParticleType::kSmoke)].m_lifetime = sf::seconds(2.5f);

    return data;
}
