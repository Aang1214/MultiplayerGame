/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#pragma once
#include "SceneNode.hpp"
#include "CommandQueue.hpp"

class Entity : public SceneNode
{
public:
	Entity(int hitpoints);
	void SetVelocity(sf::Vector2f velocity);
	void SetVelocity(float vx, float vy);
	sf::Vector2f GetVelocity() const;
	void SetRotation(float angle);
	float GetRotation() const;
	void Accelerate(sf::Vector2f velocity);
	void Accelerate(float vx, float vy);

	int GetHitPoints() const;
	void SetHitpoints(int points);
	void Repair(int points);
	void Damage(int points);
	void Destroy();
	virtual bool IsDestroyed() const override;

	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands);

	void Remove();
	

private:
	sf::Vector2f m_velocity;
	int m_hitpoints;
	float m_rotation;
};

