/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#include "SoundNode.hpp"

#include "SoundPlayer.hpp"
#include "ReceiverCategories.hpp"


SoundNode::SoundNode(SoundPlayer& player)
	: m_sounds(player)
{
}

void SoundNode::PlaySound(SoundEffect sound, sf::Vector2f position)
{
	m_sounds.Play(sound, position);
}

unsigned int SoundNode::GetCategory() const
{
	return static_cast<int>(ReceiverCategories::kSoundEffect);
}
