/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#include "GameState.hpp"
#include "Player.hpp"
#include "MissionStatus.hpp"

GameState::GameState(StateStack& stack, Context context) : State(stack, context), m_world(*context.window, *context.fonts, *context.sounds, false), m_player(nullptr, 1, context.keys1)
{
	m_world.AddAircraft(1);
	m_player.ResetPlayerRotations(); 
	m_player.SetMissionStatus(MissionStatus::kMissionRunning);
	//Play the music
	context.music->Play(MusicThemes::kMissionTheme);
}

void GameState::Draw()
{
	m_world.Draw();
}

bool GameState::Update(sf::Time dt)
{

	m_world.Update(dt);
	if (m_world.PlayerLose())
	{
		m_player.SetMissionStatus(MissionStatus::PlayerLose);
		RequestStackPush(StateID::kGameOver);
	}
	else if(m_world.PlayerWin())
	{ 
		m_player.SetMissionStatus(MissionStatus::PlayerWin);
		RequestStackPush(StateID::kGameOver);
	}
	CommandQueue& commands = m_world.GetCommandQueue();
	m_player.HandleRealtimeInput(commands);
	return true;
}

bool GameState::HandleEvent(const sf::Event& event)
{
	CommandQueue& commands = m_world.GetCommandQueue();
	m_player.HandleEvent(event, commands);

	//Escape should bring up the pause menu
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		RequestStackPush(StateID::kPause);
	}
	return true;
}
