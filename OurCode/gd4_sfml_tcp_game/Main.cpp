/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "ResourceIdentifiers.hpp"
#include <iostream>
#include "Application.hpp"

int main()
{
	//TextureHolder game_textures;
	try
	{
		Application app;
		app.Run();
	}
	catch(std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}
}