/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#pragma once
#include "SceneNode.hpp"
#include "ResourceIdentifiers.hpp"


class SoundPlayer;

class SoundNode : public SceneNode
{
public:
	explicit SoundNode(SoundPlayer& player);
	void PlaySound(SoundEffect sound, sf::Vector2f position);

	virtual unsigned int GetCategory() const override;


private:
	SoundPlayer& m_sounds;
};

