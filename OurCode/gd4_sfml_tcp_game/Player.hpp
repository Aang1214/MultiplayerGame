/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#pragma once
#include <SFML/Window/Event.hpp>
#include "Action.hpp"
#include "CommandQueue.hpp"
#include "MissionStatus.hpp"
#include <map>

class Command;


class Player
{
public:
	Player();
	void ResetPlayerRotations();
	void HandleEvent(const sf::Event& event, CommandQueue& command_queue);
	void HandleRealTimeInput(CommandQueue& command_queue);

	void AssignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key GetAssignedKey(Action action) const;
	void SetMissionStatus(MissionStatus status);
	MissionStatus GetMissionStatus() const;

private:
	void InitialiseActions();
	static bool IsRealTimeAction(Action action);


private:
	std::map<sf::Keyboard::Key, Action> m_key_binding_P1;
	std::map<sf::Keyboard::Key, Action> m_key_binding_God;

	std::map<Action, Command> m_action_binding_P1;
	std::map<Action, Command> m_action_binding_God;
	MissionStatus m_current_mission_status;

};

