/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#include "Player.hpp"
#include "CommandQueue.hpp"
#include "Aircraft.hpp"
#include "NetworkProtocol.hpp"
#include <SFML/Network/Packet.hpp>
#include "KeyBinding.hpp"

#include <map>
#include <string>
#include <algorithm>
#include <iostream>


//struct AircraftRotater (accepts and playerID and a float of rotation)
//update mP1Rotation 
struct AircraftRotater
{
    // Constructor takes an additional ID parameter
    AircraftRotater(float rotation, int identifier)
        : m_rotation(rotation),
        aircraft_id(identifier)
    {}

    void operator()(Aircraft& aircraft, sf::Time) const
    {
        if (aircraft.GetIdentifier() == aircraft_id)
        {
            // get the current rotation of the aircraft
            float rot = aircraft.GetRotation();
            // Update the rotation of the aircraft
            aircraft.rotate(m_rotation);
            // Update the appropriate rotation variable based on the ID
            aircraft.SetRotation(rot + m_rotation);
        }
    }

    int aircraft_id;
    float m_rotation;
};

//struct AircraftMover
struct AircraftMover
{
    // Constructor takes an ID and a speed
    AircraftMover(float speed, int identifier) :
        m_speed(speed),
        aircraft_id(identifier)
    {}

    void operator()(Aircraft& aircraft, sf::Time) const
    {
        if (aircraft.GetIdentifier() == aircraft_id)
        {
            // Select the correct rotation based on the ID
            float rot = aircraft.GetRotation();

            // Convert degrees to radians for trigonometric functions
            float radian = rot * 3.14159265f / 180.0f;

            // Calculate the forward velocity based on the angle
            sf::Vector2f velocity(std::cos(radian) * m_speed, std::sin(radian) * m_speed);

            // Move the aircraft by this velocity
            aircraft.Accelerate(velocity);
        }
    }

    int aircraft_id;
    float m_speed;
};

struct AircraftRotReset
{
    void operator()(Aircraft& aircraft, sf::Time) const
    {
        aircraft.SetRotation(90);
    }
};

struct AircraftMissileTrigger
{
    AircraftMissileTrigger(int identifier)
        : aircraft_id(identifier)
    {
    }

    void operator() (Aircraft& aircraft, sf::Time) const
    {
        if (aircraft.GetIdentifier() == aircraft_id)
            aircraft.LaunchMissile();
    }

    int aircraft_id;
};

Player::Player(sf::TcpSocket* socket, sf::Int32 identifier, const KeyBinding* binding)
    : m_key_binding(binding),
    m_current_mission_status(MissionStatus::kMissionRunning),
    m_identifier(identifier), m_socket(socket)
{
    //Set initial action bindings
    InitialiseActions();

    //Assign all categories to a player's aircraft
    for (auto& pair : m_action_binding)
    {
        pair.second.category = static_cast<unsigned int>(ReceiverCategories::kP1);
    }

}
//reset player rotations using (aircraft.SetRotation(90.f))
void Player::ResetPlayerRotations()
{
    AircraftRotReset;
}


void Player::HandleEvent(const sf::Event& event, CommandQueue& commands)
{
    if (event.type == sf::Event::KeyPressed)
    {
        Action action;
        if (m_key_binding && m_key_binding->CheckAction(event.key.code, action) && !IsRealtimeAction(action))
        {
            // Network connected -> send event over network
            if (m_socket)
            {
                sf::Packet packet;
                packet << static_cast<sf::Int32>(Client::PacketType::kPlayerEvent);
                packet << m_identifier;
                packet << static_cast<sf::Int32>(action);
                m_socket->send(packet);
            }

            // Network disconnected -> local event
            else
            {
                commands.Push(m_action_binding[action]);
            }
        }
    }

    // Realtime change (network connected)
    if ((event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) && m_socket)
    {
        Action action;
        if (m_key_binding && m_key_binding->CheckAction(event.key.code, action) && IsRealtimeAction(action))
        {
            // Send realtime change over network
            sf::Packet packet;
            packet << static_cast<sf::Int32>(Client::PacketType::kPlayerRealtimeChange);
            packet << m_identifier;
            packet << static_cast<sf::Int32>(action);
            packet << (event.type == sf::Event::KeyPressed);
            m_socket->send(packet);
        }
    }
}

bool Player::IsLocal() const
{
    // No key binding means this player is remote
    return m_key_binding != nullptr;
}

void Player::DisableAllRealtimeActions()
{
    for (auto& action : m_action_proxies)
    {
        sf::Packet packet;
        packet << static_cast<sf::Int32>(Client::PacketType::kPlayerRealtimeChange);
        packet << m_identifier;
        packet << static_cast<sf::Int32>(action.first);
        packet << false;
        m_socket->send(packet);
    }
}

void Player::HandleRealtimeInput(CommandQueue& commands) 
{
    // Check if this is a networked game and local player or just a single player game
    if ((m_socket && IsLocal()) || !m_socket) 
    {
        // Lookup all actions and push corresponding commands to queue
        std::vector<Action> activeActions = m_key_binding->GetRealtimeActions(); 
        for (Action action : activeActions) 
            commands.Push(m_action_binding[action]); 
    } 
}

void Player::HandleRealtimeNetworkInput(CommandQueue& commands)
{
    if (m_socket && !IsLocal())
    {
        // Traverse all realtime input proxies. Because this is a networked game, the input isn't handled directly
        for (auto pair : m_action_proxies)
        {
            if (pair.second && IsRealtimeAction(pair.first))
                commands.Push(m_action_binding[pair.first]);
        }
    }
}

void Player::HandleNetworkEvent(Action action, CommandQueue& commands)
{
    commands.Push(m_action_binding[action]);
}

void Player::HandleNetworkRealtimeChange(Action action, bool actionEnabled)
{
    m_action_proxies[action] = actionEnabled;
}

void Player::SetMissionStatus(MissionStatus status)
{
    m_current_mission_status = status;
}

MissionStatus Player::GetMissionStatus() const
{
    return m_current_mission_status;
}

void Player::InitialiseActions()
{
    const float kPlayerSpeed = 400.f;
    const float kPlayerRotation = 4.f;

    m_action_binding[Action::kP1TiltLeft].action = DerivedAction<Aircraft>(AircraftRotater(-kPlayerRotation, m_identifier));
    m_action_binding[Action::kP1TiltRight].action = DerivedAction<Aircraft>(AircraftRotater(kPlayerRotation, m_identifier));
    m_action_binding[Action::kP1MoveUp].action = DerivedAction<Aircraft>(AircraftMover(kPlayerSpeed, m_identifier));
    m_action_binding[Action::kP1UsePowerUp].action = DerivedAction<Aircraft>(AircraftMissileTrigger(m_identifier));
}
