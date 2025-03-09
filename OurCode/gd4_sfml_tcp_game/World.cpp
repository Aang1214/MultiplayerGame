/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#include "World.hpp"
#include "Pickup.hpp"
#include "Projectile.hpp"
#include "ParticleNode.hpp"
#include "SoundNode.hpp"

World::World(sf::RenderTarget& output_target, FontHolder& font, SoundPlayer& sounds, bool networked)
	:m_target(output_target)
	,m_camera(output_target.getDefaultView())
	,m_textures()
	,m_fonts(font)
	,m_sounds(sounds)
	,m_scenegraph(ReceiverCategories::kNone)
	,m_scene_layers()
	,m_world_bounds(0.f,0.f, m_camera.getSize().x, 3000.f)
	,m_spawn_position(m_camera.getSize().x/2.f, m_world_bounds.height - m_camera.getSize().y/2.f)
	,m_scrollspeed(0.f)
	,m_P1_aircraft(nullptr)
{
	m_scene_texture.create(m_target.getSize().x, m_target.getSize().y);
	LoadTextures();
	BuildScene();
	m_camera.setCenter(m_spawn_position);
}

void World::Update(sf::Time dt)
{
	//Scroll the world (remove this if you want to stop scrolling)
	m_camera.move(0, m_scrollspeed * dt.asSeconds());

	
	m_P1_aircraft->SetVelocity(0.f, 0.f);

	ReboundEntitiesOutsideView();
	GuideMissiles();

	//Forward commands to the scenegraph
	while (!m_command_queue.IsEmpty())
	{
		m_scenegraph.OnCommand(m_command_queue.Pop(), dt);
	}
	AdaptPlayerVelocity();

	HandleCollisions();

	m_scenegraph.RemoveWrecks();

	SpawnEnemies();

	m_scenegraph.Update(dt, m_command_queue);
	AdaptPlayerPosition();
}

void World::Draw()
{
	if (PostEffect::IsSupported())
	{
		m_scene_texture.clear();
		m_scene_texture.setView(m_camera);
		m_scene_texture.draw(m_scenegraph);
		m_scene_texture.display();
		m_bloom_effect.Apply(m_scene_texture, m_target);
	}
	else
	{
		m_target.setView(m_camera);
		m_target.draw(m_scenegraph);
	}
}

CommandQueue& World::GetCommandQueue()
{
	return m_command_queue;
}

//find me
bool World::PlayerLose() const
{
	return m_P1_aircraft->IsDestroyed();
}

bool World::PlayerWin() const
{
	return false;
}

// change/add textures
void World::LoadTextures()
{
	m_textures.Load(TextureID::kPlayer, "Media/Textures/SpaceShipGreen.png");
	m_textures.Load(TextureID::kMeteor, "Media/Textures/Avenger.png");
	m_textures.Load(TextureID::kLandscape, "Media/Textures/Desert.png");
	m_textures.Load(TextureID::kBullet, "Media/Textures/Bullet.png");
	m_textures.Load(TextureID::kMissile, "Media/Textures/Missile.png");

	m_textures.Load(TextureID::kHealthRefill, "Media/Textures/HealthRefill.png");
	m_textures.Load(TextureID::kMissileRefill, "Media/Textures/MissileRefill.png");
	m_textures.Load(TextureID::kFireSpread, "Media/Textures/FireSpread.png");
	m_textures.Load(TextureID::kFireRate, "Media/Textures/FireRate.png");
	m_textures.Load(TextureID::kFinishLine, "Media/Textures/FinishLine.png");

	m_textures.Load(TextureID::kEntities, "Media/Textures/Ships.png");
	m_textures.Load(TextureID::kSpace, "Media/Textures/Space.png");
	m_textures.Load(TextureID::kExplosion, "Media/Textures/Explosion.png");
	m_textures.Load(TextureID::kParticle, "Media/Textures/Particle.png");


}

void World::BuildScene()
{
	//Initialize the different layers
	for (std::size_t i = 0; i < static_cast<int>(SceneLayers::kLayerCount); ++i)
	{
		ReceiverCategories category = (i == static_cast<int>(SceneLayers::kLowerAir)) ? ReceiverCategories::kScene : ReceiverCategories::kNone;
		SceneNode::Ptr layer(new SceneNode(category));
		m_scene_layers[i] = layer.get();
		m_scenegraph.AttachChild(std::move(layer));
	}

	//Prepare the background
	sf::Texture& texture = m_textures.Get(TextureID::kSpace);
	sf::IntRect textureRect(m_world_bounds);
	texture.setRepeated(true);

	//Add the background sprite to the world
	std::unique_ptr<SpriteNode> background_sprite(new SpriteNode(texture, textureRect));
	background_sprite->setPosition(m_world_bounds.left, m_world_bounds.top);
	m_scene_layers[static_cast<int>(SceneLayers::kBackground)]->AttachChild(std::move(background_sprite));

	//Add the finish line
	sf::Texture& finish_texture = m_textures.Get(TextureID::kFinishLine);
	std::unique_ptr<SpriteNode> finish_sprite(new SpriteNode(finish_texture));
	finish_sprite->setPosition(0.f, -76.f);

	//Add the player 1 aircraft *** 
	std::unique_ptr<Aircraft> Player1(new Aircraft(AircraftType::kP1, m_textures, m_fonts));
	m_P1_aircraft = Player1.get();
	m_P1_aircraft->setPosition(m_spawn_position.x - 400, m_spawn_position.y);
	m_P1_aircraft->SetVelocity(40.f, m_scrollspeed);
	m_P1_aircraft->SetRotation(90.f);
	m_scene_layers[static_cast<int>(SceneLayers::kUpperAir)]->AttachChild(std::move(Player1));

	//Add the particle nodes to the scene
	std::unique_ptr<ParticleNode> smokeNode(new ParticleNode(ParticleType::kSmoke, m_textures));
	m_scene_layers[static_cast<int>(SceneLayers::kLowerAir)]->AttachChild(std::move(smokeNode));

	std::unique_ptr<ParticleNode> propellantNode(new ParticleNode(ParticleType::kPropellant, m_textures));
	m_scene_layers[static_cast<int>(SceneLayers::kLowerAir)]->AttachChild(std::move(propellantNode));

	// Add sound effect node
	std::unique_ptr<SoundNode> soundNode(new SoundNode(m_sounds));
	m_scenegraph.AttachChild(std::move(soundNode));

	if (m_networked_world)
	{
		std::unique_ptr<NetworkNode> network_node(new NetworkNode());
		m_network_node = network_node.get();
		m_scenegraph.AttachChild(std::move(network_node));
	}

	AddEnemies();


}

//************************************************
void World::AdaptPlayerPosition()
{
	sf::FloatRect view_bounds(m_camera.getCenter() - m_camera.getSize() / 2.f, m_camera.getSize());
	const float border_distance = 40.f;

	//keep the player 1 on the screen
	sf::Vector2f position1 = m_P1_aircraft->getPosition();
	position1.x = std::max(position1.x, view_bounds.left + border_distance);
	position1.x = std::min(position1.x, view_bounds.left + view_bounds.width - border_distance);
	position1.y = std::max(position1.y, view_bounds.top + border_distance);
	position1.y = std::min(position1.y, view_bounds.top + view_bounds.height -border_distance);
	m_P1_aircraft->setPosition(position1);

}

void World::AdaptPlayerVelocity()
{
	sf::Vector2f velocity1 = m_P1_aircraft->GetVelocity();

	//If they are moving diagonally divide by sqrt 2 (remove)
	if (velocity1.x != 0.f && velocity1.y != 0.f)
	{
		m_P1_aircraft->SetVelocity(velocity1 / std::sqrt(2.f));
	}
	//Add scrolling velocity (remove
	m_P1_aircraft->Accelerate(0.f, m_scrollspeed);

}

//chamge to spawn meteors
void World::SpawnEnemies()
{
	//Spawn an enemy when it is relevant i.e when it is in the Battlefieldboudns
	while (!m_enemy_spawn_points.empty() && m_enemy_spawn_points.back().m_y > GetBattleFieldBounds().top)
	{
		SpawnPoint spawn = m_enemy_spawn_points.back();
		std::unique_ptr<Aircraft> enemy(new Aircraft(spawn.m_type, m_textures, m_fonts));
		enemy->setPosition(spawn.m_x, spawn.m_y);
		//random float 
		/*
		float rot = static_cast<float>(rand() % 360);
		enemy->setRotation(rot);*/
		m_scene_layers[static_cast<int>(SceneLayers::kUpperAir)]->AttachChild(std::move(enemy));
		m_enemy_spawn_points.pop_back();
	}
}

//change to spawn meteors
void World::AddEnemies()
{
	AddEnemy(AircraftType::kMeteorA, 240.f, 100.f);
	AddEnemy(AircraftType::kMeteorB, 240.f, 300.f);
	AddEnemy(AircraftType::kMeteorB, 240.f, -100.f);
	AddEnemy(AircraftType::kMeteorA, 240.f, -300.f);
	AddEnemy(AircraftType::kMeteorB, -240.f, 100.f);
	AddEnemy(AircraftType::kMeteorA, -240.f, 300.f);
	AddEnemy(AircraftType::kMeteorA, -240.f, -100.f);
	AddEnemy(AircraftType::kMeteorB, -240.f, -300.f);
	AddEnemy(AircraftType::kMeteorA, 0.f, 150.f);
	AddEnemy(AircraftType::kMeteorB, 0.f, 0.f);
	AddEnemy(AircraftType::kMeteorA, 0.f, -150.f);

	//Sort the enemies according to y-value so that enemies are checked first
	std::sort(m_enemy_spawn_points.begin(), m_enemy_spawn_points.end(), [](SpawnPoint lhs, SpawnPoint rhs)
	{
		return lhs.m_y < rhs.m_y;
	});

}

void World::AddEnemy(AircraftType type, float relx, float rely)
{
	SpawnPoint spawn(type, m_spawn_position.x + relx, m_spawn_position.y - rely);
	m_enemy_spawn_points.emplace_back(spawn);
}

sf::FloatRect World::GetViewBounds() const
{
	return sf::FloatRect(m_camera.getCenter() - m_camera.getSize()/2.f, m_camera.getSize());
}

//change for meteors
sf::FloatRect World::GetBattleFieldBounds() const
{
	//Return camera bounds + a small area at the top where enemies spawn
	sf::FloatRect bounds = GetViewBounds();
	return bounds;

}

void World::DestroyEntitiesOutsideView()
{
	Command command;
	command.category = static_cast<int>(ReceiverCategories::kProjectile);
	command.action = DerivedAction<Entity>([this](Entity& e, sf::Time dt)
		{
			//Does the object intersect with the battlefield
			if (!GetBattleFieldBounds().intersects(e.GetBoundingRect()))
			{
				e.Remove();
			}
		});
	m_command_queue.Push(command);
}

void World::ReboundEntitiesOutsideView()
{
	Command command;
	command.category = static_cast<int>(ReceiverCategories::kMeteors);
	command.action = DerivedAction<Entity>([this](Entity& e, sf::Time dt)
		{
			auto bounds = GetBattleFieldBounds();
			auto rect = e.GetBoundingRect();
			sf::Vector2f v = e.GetVelocity();

			// Check horizontal collision (left or right side)
			if (rect.left < bounds.left || rect.left + rect.width > bounds.left + bounds.width) {
				v.x = -v.x;  // Reverse horizontal velocity
			}

			// Check vertical collision (top or bottom)
			if (rect.top < bounds.top || rect.top + rect.height > bounds.top + bounds.height) {
				v.y = -v.y;  // Reverse vertical velocity
			}

			e.SetVelocity(v);
		});
	m_command_queue.Push(command);

}

// keep maybe
void World::GuideMissiles()
{
	//Target the closest enemy in the world
	Command enemyCollector;
	enemyCollector.category = static_cast<int>(ReceiverCategories::kMeteors);
	enemyCollector.action = DerivedAction<Aircraft>([this](Aircraft& enemy, sf::Time)
		{
			if (!enemy.IsDestroyed())
			{
				m_active_enemies.emplace_back(&enemy);
			}
		});

	Command missileGuider;
	missileGuider.category = static_cast<int>(ReceiverCategories::kAlliedProjectile);
	missileGuider.action = DerivedAction<Projectile>([this](Projectile& missile, sf::Time dt)
		{
			if (!missile.IsGuided())
			{
				return;
			}

			float min_distance = std::numeric_limits<float>::max();
			Aircraft* closest_enemy = nullptr;

			for (Aircraft* enemy : m_active_enemies)
			{
				float enemy_distance = Distance(missile, *enemy);
				if (enemy_distance < min_distance)
				{
					closest_enemy = enemy;
					min_distance = enemy_distance;
				}
			}

			if (closest_enemy)
			{
				missile.GuideTowards(closest_enemy->GetWorldPosition());
			}
		});

	m_command_queue.Push(enemyCollector);
	m_command_queue.Push(missileGuider);
	m_active_enemies.clear();
}

bool MatchesCategories(SceneNode::Pair& colliders, ReceiverCategories type1, ReceiverCategories type2)
{
	unsigned int category1 = colliders.first->GetCategory();
	unsigned int category2 = colliders.second->GetCategory();

	if (static_cast<int>(type1) & category1 && static_cast<int>(type2) & category2)
	{
		return true;
	}
	else if (static_cast<int>(type1) & category2 && static_cast<int>(type2) & category1)
	{ 
		std::swap(colliders.first, colliders.second);
	}
	else
	{
		return false;
	}
}

// change to knockback/damage
void World::HandleCollisions()
{
	std::set<SceneNode::Pair> collision_pairs;
	m_scenegraph.CheckSceneCollision(m_scenegraph, collision_pairs);
	for (SceneNode::Pair pair : collision_pairs)
	{
		//colision between players and meteors
		if (MatchesCategories(pair, ReceiverCategories::kP1, ReceiverCategories::kMeteors))
		{
			auto& player = static_cast<Aircraft&>(*pair.first);
			auto& meteor = static_cast<Aircraft&>(*pair.second);

			sf::Vector2f v1 = player.GetVelocity();
			sf::Vector2f v2 = meteor.GetVelocity();

			//Collision response
			player.Damage(1);

			if (v1 == sf::Vector2f(0.f, 0.f))
			{
				meteor.SetVelocity(-v2);
			}
			else 
			{
				meteor.SetVelocity(v1 * 1.f);
			}
			player.SetVelocity(-v1*10.f);
			
		}

		/*else if (MatchesCategories(pair, ReceiverCategories::kP1, ReceiverCategories::kPickup))
		{
			auto& player = static_cast<Aircraft&>(*pair.first);
			auto& pickup = static_cast<Pickup&>(*pair.second);
			//Collision response
			pickup.Apply(player);
			pickup.Destroy();
			player.PlayLocalSound(m_command_queue, SoundEffect::kCollectPickup);
		}*/
		//missile and meteor collision
		else if (MatchesCategories(pair, ReceiverCategories::kMeteors, ReceiverCategories::kAlliedProjectile))
		{
			auto& meteor = static_cast<Aircraft&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);
			//Collision response - knockback (transfer all momentum from projectile to the meteor)
			sf::Vector2f v1 = projectile.GetVelocity();
			sf::Vector2f v2 = meteor.GetVelocity();

			meteor.SetVelocity(v2 + (v1));
			projectile.Destroy();
		}
		//meteor and meteor collision
		else if (MatchesCategories(pair, ReceiverCategories::kMeteors, ReceiverCategories::kMeteors))
		{
			auto& meteor1 = static_cast<Aircraft&>(*pair.first);
			auto& meteor2 = static_cast<Aircraft&>(*pair.second);
			//Colission responce - knockback (transfer all momentum from one to the other)
			sf::Vector2f v1 = meteor1.GetVelocity();
			sf::Vector2f v2 = meteor2.GetVelocity();
			//check if one of the meteors v is 0, make both v the other meteors v and the one that was 0 half it and in the opposite direction
			if (v1 == sf::Vector2f(0.f, 0.f))
			{
				meteor1.SetVelocity(v2 );
				meteor2.SetVelocity(v2 / 3.f);
			}
			else if (v2 == sf::Vector2f(0.f, 0.f))
			{
				meteor2.SetVelocity(v1);
				meteor1.SetVelocity(v1 / 3.f);
			}
			else
			{
				meteor1.SetVelocity(v2);
				meteor2.SetVelocity(v1);
			}
		}
	}
}

void World::UpdateSounds()
{
	// Set listener's position to player position
	m_sounds.SetListenerPosition(m_P1_aircraft->GetWorldPosition());

	// Remove unused sounds
	m_sounds.RemoveStoppedSounds();
}
