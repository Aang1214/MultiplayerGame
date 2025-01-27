#include "Player.hpp"
#include "ReceiverCategories.hpp"
#include "Aircraft.hpp"

struct AircraftRotater
{
    AircraftRotater(float rotation) :m_rotation(rotation)
	{}
    void operator()(Aircraft& aircraft, sf::Time) const
    {
		aircraft.rotate(m_rotation);
	}

	float m_rotation;
};

//struct AircraftMover
struct AircraftMover
{
    AircraftMover(float vx, float vy) :velocity(vx, vy)
    {}
    void operator()(Aircraft& aircraft, sf::Time) const
    {
        aircraft.Accelerate(velocity);
    }

    sf::Vector2f velocity;
};

//controls
Player::Player(): m_current_mission_status(MissionStatus::kMissionRunning)
{
    //Set initial key bindings
    //P1
    m_key_binding[sf::Keyboard::A] = Action::kP1TiltLeft; 
    m_key_binding[sf::Keyboard::D] = Action::kP1TiltRight; 
    m_key_binding[sf::Keyboard::W] = Action::kP1MoveUp; 
    m_key_binding[sf::Keyboard::C] = Action::kP1UsePowerUp;
    //P2
    m_key_binding[sf::Keyboard::J] = Action::kP2TiltLeft;
    m_key_binding[sf::Keyboard::L] = Action::kP2TiltRight;
    m_key_binding[sf::Keyboard::I] = Action::kP2MoveUp;
    m_key_binding[sf::Keyboard::M] = Action::kP2UsePowerUp;

    m_key_binding[sf::Keyboard::Space] = Action::kMeteorSpawn; //remove or temp meteor spawn

    //Set initial action bindings
    InitialiseActions();

    //Assign all categories to a player's aircraft
    for (auto& pair : m_action_binding)
    {
        pair.second.category = static_cast<unsigned int>(ReceiverCategories::kPlayerAircraft);
    }
}

void Player::HandleEvent(const sf::Event& event, CommandQueue& command_queue)
{
    if (event.type == sf::Event::KeyPressed)
    {
        auto found = m_key_binding.find(event.key.code);
        if (found != m_key_binding.end() && !IsRealTimeAction(found->second))
        {
            command_queue.Push(m_action_binding[found->second]);
        }
    }
}

void Player::HandleRealTimeInput(CommandQueue& command_queue)
{
    //Check if any of the key bindings are pressed
    for (auto pair : m_key_binding)
    {
        if (sf::Keyboard::isKeyPressed(pair.first) && IsRealTimeAction(pair.second))
        {
            command_queue.Push(m_action_binding[pair.second]);
        }
    }
}

void Player::AssignKey(Action action, sf::Keyboard::Key key)
{
    //Remove keys that are currently bound to the action
    for (auto itr = m_key_binding.begin(); itr != m_key_binding.end();)
    {
        if (itr->second == action)
        {
            m_key_binding.erase(itr++);
        }
        else
        {
            ++itr;
        }
    }
    m_key_binding[key] = action;
}

sf::Keyboard::Key Player::GetAssignedKey(Action action) const
{
    for (auto pair : m_key_binding)
    {
        if (pair.second == action)
        {
            return pair.first;
        }
    }
    return sf::Keyboard::Unknown;
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
    //add p2
    const float kPlayerSpeed = 200.f;
    const float kPlayerRotation = 1.f;
    m_action_binding[Action::kP1TiltLeft].action =  DerivedAction<Aircraft>(AircraftRotater(-kPlayerRotation));
    m_action_binding[Action::kP1TiltRight].action = DerivedAction<Aircraft>(AircraftRotater(kPlayerRotation));

    m_action_binding[Action::kP1MoveUp].action = DerivedAction<Aircraft>(AircraftMover(0.f, -kPlayerSpeed));

    m_action_binding[Action::kMeteorSpawn].action = DerivedAction<Aircraft>([](Aircraft& a, sf::Time dt)
        {
            a.Fire();
        }
    );

    m_action_binding[Action::kP1UsePowerUp].action = DerivedAction<Aircraft>([](Aircraft& a, sf::Time dt)
        {
            a.LaunchMissile();
        }
    );

}

bool Player::IsRealTimeAction(Action action)
{
    switch (action)
    {
        //add for P2
    case Action::kP1TiltLeft:
    case Action::kP1TiltRight:
    case Action::kP1MoveUp:

    case Action::kMeteorSpawn:
        return true;
    default:
        return false;
    }
}
