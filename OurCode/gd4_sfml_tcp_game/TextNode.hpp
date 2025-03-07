/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#pragma once
#include "SceneNode.hpp"
#include "ResourceIdentifiers.hpp"

class TextNode : public SceneNode
{
public:
	explicit TextNode(const FontHolder& fonts, std::string& text);
	void SetString(const std::string& text);

private:
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	sf::Text m_text;
};

