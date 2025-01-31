#include "Player.hpp"
#include "ReceiverCategories.hpp"
#include "Aircraft.hpp"


//struct AircraftRotater (accepts and playerID and a float of rotation)
//update mP1Rotation or mP2Rotation based on playerID
struct AircraftRotater
{
    // Constructor takes an additional ID parameter
    AircraftRotater(float rotation) : m_rotation(rotation)
    {}
    
    void operator()(Aircraft& aircraft, sf::Time) const
    {
        // get the current rotation of the aircraft
        float rot = aircraft.GetRotation();
        // Update the rotation of the aircraft
        aircraft.rotate(m_rotation);
        // Update the appropriate rotation variable based on the ID
        aircraft.SetRotation(rot + m_rotation);
    }

    int m_id; // ID to differentiate between different entities, like player 1 or player 2
    float m_rotation;
};



//struct AircraftMover
struct AircraftMover
{
    // Constructor takes an ID and a speed
    AircraftMover(float speed) : m_speed(speed)
    {}

    void operator()(Aircraft& aircraft, sf::Time) const
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

    int m_id;
    float m_speed;
};

struct AircraftRotReset
{
    void operator()(Aircraft& aircraft, sf::Time) const
    {
		aircraft.SetRotation(90.f);
	}
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

//reset player rotations using (aircraft.SetRotation(90.f))
void Player::ResetPlayerRotations()
{
    AircraftRotReset;
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
    const float kPlayerSpeed = 400.f;
    const float kPlayerRotation = 2.f;
    m_action_binding[Action::kP1MoveUp].action = DerivedAction<Aircraft>(AircraftMover(-kPlayerSpeed));
    m_action_binding[Action::kP1TiltLeft].action =  DerivedAction<Aircraft>(AircraftRotater(-kPlayerRotation));
    m_action_binding[Action::kP1TiltRight].action = DerivedAction<Aircraft>(AircraftRotater(kPlayerRotation));
    m_action_binding[Action::kP1UsePowerUp].action = DerivedAction<Aircraft>([](Aircraft& a, sf::Time dt)
        {
            a.LaunchMissile();
        }
    );

    m_action_binding[Action::kP2MoveUp].action = DerivedAction<Aircraft>(AircraftMover(-kPlayerSpeed));
    m_action_binding[Action::kP2TiltLeft].action = DerivedAction<Aircraft>(AircraftRotater(-kPlayerRotation));
    m_action_binding[Action::kP2TiltRight].action = DerivedAction<Aircraft>(AircraftRotater(kPlayerRotation));
    m_action_binding[Action::kP2UsePowerUp].action = DerivedAction<Aircraft>([](Aircraft& a, sf::Time dt)
        {
            a.LaunchMissile();
        }
    );

    m_action_binding[Action::kMeteorSpawn].action = DerivedAction<Aircraft>([](Aircraft& a, sf::Time dt)
            {
                a.Fire();
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
