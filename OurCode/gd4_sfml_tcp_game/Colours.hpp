#pragma once
#include "ColourID.hpp"

class Colours
{
public:
	// Returns a colour based on the ColourID
	static sf::Color GetColour(ColourID colour_id);
	// Returns a ColourID based on int value
	static ColourID GetColourID(int colour_id);
};

