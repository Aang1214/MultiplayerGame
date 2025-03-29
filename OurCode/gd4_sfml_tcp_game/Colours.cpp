#include "Colours.hpp"

sf::Color Colours::GetColour(ColourID colour_id)
{
	switch (colour_id)
	{
		case ColourID::kOrnage:
			return sf::Color(255, 165, 0);
		case ColourID::kBrown:
			return sf::Color(165, 42, 42);
		case ColourID::kYellow:
			return sf::Color(255, 255, 0);
		case ColourID::kOlive:
			return sf::Color(128, 128, 0);
		case ColourID::kGreen:
			return sf::Color(0, 128, 0);
		case ColourID::kLightGreen:
			return sf::Color(144, 238, 144);
		case ColourID::kCyan:
			return sf::Color(0, 255, 255);
		case ColourID::kAqua:
			return sf::Color(0, 128, 128);
		case ColourID::kBlue:
			return sf::Color(0, 0, 255);
		case ColourID::kLightBlue:
			return sf::Color(173, 216, 230);
		case ColourID::kViolet:
			return sf::Color(238, 130, 238);
		case ColourID::kPurple:
			return sf::Color(128, 0, 128);
		case ColourID::kPink:
			return sf::Color(255, 192, 203);
		case ColourID::kMagenta:
			return sf::Color(255, 0, 255);
		case ColourID::kWhite:
			return sf::Color(255, 255, 255);
		case ColourID::kGrey:
			return sf::Color(128, 128, 128);	
	}
}

ColourID Colours::GetColourID(int colour_id)
{
	switch (colour_id)
	{
		case 0:
			return ColourID::kOrnage;
		case 1:
			return ColourID::kBrown;
		case 2:
			return ColourID::kYellow;
		case 3:
			return ColourID::kOlive;
		case 4:
			return ColourID::kGreen;
		case 5:
			return ColourID::kLightGreen;
		case 6:
			return ColourID::kCyan;
		case 7:
			return ColourID::kAqua;
		case 8:
			return ColourID::kBlue;
		case 9:
			return ColourID::kLightBlue;
		case 10:
			return ColourID::kViolet;
		case 11:
			return ColourID::kPurple;
		case 12:
			return ColourID::kPink;
		case 13:
			return ColourID::kMagenta;
		case 14:
			return ColourID::kWhite;
		case 15:
			return ColourID::kGrey;
	}
}
