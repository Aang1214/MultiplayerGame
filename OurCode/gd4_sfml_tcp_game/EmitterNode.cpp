/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#include "EmitterNode.hpp"
//******************************************Particle effects*********************************************

EmitterNode::EmitterNode(ParticleType type)
	:SceneNode()
	, m_accumulated_time(sf::Time::Zero)
	, m_type(type)
	, m_particle_system(nullptr)
{
}

void EmitterNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (m_particle_system)
	{
		EmitParticles(dt);
	}
	else
	{
		// Find particle node with the same type as the emitter
		auto finder = [this](ParticleNode& container, sf::Time)
			{
				if (container.GetParticleType() == m_type)
					m_particle_system = &container;
			};
		Command command;
		command.category = static_cast<int>(ReceiverCategories::kParticleSystem);
		command.action = DerivedAction<ParticleNode>(finder);

		commands.Push(command);
	}
}

void EmitterNode::EmitParticles(sf::Time dt)
{
	const float emissionRate = 30.f;
	const sf::Time interval = sf::seconds(1.f) / emissionRate;

	m_accumulated_time += dt;
	while (m_accumulated_time > interval)
	{
		m_accumulated_time -= interval;
		m_particle_system->AddParticle(GetWorldPosition());
	}
}

//setcolor of the particle
void EmitterNode::SetColour(sf::Color color)
{
	m_particle_system->SetColour(color);
}
