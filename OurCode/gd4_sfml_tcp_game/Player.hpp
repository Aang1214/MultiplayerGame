/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#pragma once
#include "Command.hpp"
#include "Action.hpp"
#include "MissionStatus.hpp"
#include <SFML/Window/Event.hpp>
#include <map>
#include "CommandQueue.hpp"
#include <SFML/Network/TcpSocket.hpp>

class Command;


class Player
{
public:
	Player();
	Player(sf::TcpSocket* socket, sf::Int32 identifier, const KeyBinding* binding);
	void ResetPlayerRotations();
	void HandleEvent(const sf::Event& event, CommandQueue& command_queue);
	bool IsLocal() const;
	void DisableAllRealtimeActions();
	void HandleRealTimeInput(CommandQueue& command_queue);

	void HandleRealtimeNetworkInput(CommandQueue& commands);

	void HandleNetworkEvent(Action action, CommandQueue& commands);

	void HandleNetworkRealtimeChange(Action action, bool actionEnabled);

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

	std::map<Action, Command> m_action_binding;
	std::map<Action, bool> m_action_proxies;
	int m_identifier;
	sf::TcpSocket* m_socket;
};

