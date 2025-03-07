/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "Player.hpp"

class Game
{
	public:
		Game();
		void Run();

	private:
		void Update(sf::Time delta_time);
		void Render();
		void ProcessInput();

	private:

		
		static const sf::Time kTimePerFrame;
		sf::RenderWindow m_window;
		World m_world;
		Player m_player;
};