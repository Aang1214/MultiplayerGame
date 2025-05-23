/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#pragma once
#include "SceneNode.hpp"
#include "ParticleType.hpp"
#include "ParticleNode.hpp"

class EmitterNode : public SceneNode
{
public:
	explicit EmitterNode(ParticleType type);
	void SetColour(sf::Color color);

private:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands);
	void EmitParticles(sf::Time dt);

private:
	sf::Time m_accumulated_time;
	ParticleType m_type;
	ParticleNode* m_particle_system;
};

