#include "Colours.hpp"

sf::Color Colours::GetColour(ColourID colour_id)
{
	switch (colour_id)
	{
		case ColourID::kOrnage:
			return sf::Color(236, 147, 51);
		case ColourID::kLightOrange:
			return sf::Color(255, 213, 109);
		case ColourID::kYellow:
			return sf::Color(255, 255, 97);
		case ColourID::kOlive:
			return sf::Color(159, 170, 98);
		case ColourID::kGreen:
			return sf::Color(84, 196, 67);
		case ColourID::kLightGreen:
			return sf::Color(133, 231, 118);
		case ColourID::kCyan:
			return sf::Color(113, 235, 184);
		case ColourID::kAqua:
			return sf::Color(53, 192, 158);
		case ColourID::kBlue:
			return sf::Color(62, 146, 198);
		case ColourID::kLightBlue:
			return sf::Color(119, 196, 243);
		case ColourID::kViolet:
			return sf::Color(152, 113, 225);
		case ColourID::kPurple:
			return sf::Color(122, 70, 174);
		case ColourID::kPink:
			return sf::Color(255, 139, 251);
		case ColourID::kMagenta:
			return sf::Color(183, 74, 255);
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
			return ColourID::kGreen;
		case 2:
			return ColourID::kYellow;
		case 3:
			return ColourID::kAqua;
		case 4:
			return ColourID::kViolet;
		case 5:
			return ColourID::kMagenta;
		case 6:
			return ColourID::kCyan;
		case 7:
			return ColourID::kWhite;
		case 8:
			return ColourID::kBlue;
		case 9:
			return ColourID::kPink;
		case 10:
			return ColourID::kLightOrange;
		case 11:
			return ColourID::kPurple;
		case 12:
			return ColourID::kLightGreen;
		case 13:
			return ColourID::kLightBlue;
		case 14:
			return ColourID::kOlive;
		case 15:
			return ColourID::kGrey;
	}
}
