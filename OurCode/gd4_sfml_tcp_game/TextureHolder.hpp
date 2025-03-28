/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#pragma once
#include "TextureID.hpp"
#include <string>
#include <SFML/Graphics.hpp>


class TextureHolder
{
public:
	void Load(TextureID id, const std::string& filename);
	sf::Texture& Get(TextureID id);
	const sf::Texture& Get(TextureID id) const;

private:
	std::map<TextureID, std::unique_ptr<sf::Texture>> m_texture_map;
};

