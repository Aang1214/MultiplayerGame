#include "KeyBinding.hpp"

#include <string>
#include <algorithm>

KeyBinding::KeyBinding(int control_preconfiguration)
	: m_key_map()
{
	// Set initial key bindings for player 1
	if (control_preconfiguration == 1)
	{
		m_key_map[sf::Keyboard::A] = Action::kP1TiltLeft; 
		m_key_map[sf::Keyboard::D] = Action::kP1TiltRight; 
		m_key_map[sf::Keyboard::W] = Action::kP1MoveUp;
		m_key_map[sf::Keyboard::Space] = Action::kP1UsePowerUp; 
	}
	else if (control_preconfiguration == 2)
	{
		// Player 2
		m_key_map[sf::Keyboard::J] = Action::kP1TiltLeft;
		m_key_map[sf::Keyboard::L] = Action::kP1TiltRight;
		m_key_map[sf::Keyboard::I] = Action::kP1MoveUp;
		m_key_map[sf::Keyboard::BackSlash] = Action::kP1UsePowerUp;
	}
}

void KeyBinding::AssignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = m_key_map.begin(); itr != m_key_map.end(); )
	{
		if (itr->second == action)
			m_key_map.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	m_key_map[key] = action;
}

sf::Keyboard::Key KeyBinding::GetAssignedKey(Action action) const
{
	for (auto pair : m_key_map)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

bool KeyBinding::CheckAction(sf::Keyboard::Key key, Action& out) const
{
	auto found = m_key_map.find(key);
	if (found == m_key_map.end())
	{
		return false;
	}
	else
	{
		out = found->second;
		return true;
	}
}

std::vector<Action> KeyBinding::GetRealtimeActions() const
{
	// Return all realtime actions that are currently active.
	std::vector<Action> actions;

	for (auto pair : m_key_map)
	{
		// If key is pressed and an action is a realtime action, store it
		if (sf::Keyboard::isKeyPressed(pair.first) && IsRealtimeAction(pair.second))
			actions.push_back(pair.second);
	}

	return actions;
}

bool IsRealtimeAction(Action action)
{
	switch (action)
	{
	case Action::kP1TiltLeft: 
	case Action::kP1TiltRight:
	case Action::kP1MoveUp: 
	case Action::kP1UsePowerUp: 
		return true;

	default:
		return false;
	}
}
