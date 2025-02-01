#include "KeySettingsState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

KeySettingsState::KeySettingsState(StateStack& stack, Context context)
	: State(stack, context)
	, m_gui_container()
{
	m_background_sprite.setTexture(context.textures->Get(TextureID::kTitleScreen));

	//Build key binding buttons and labels
	AddButtonLabel(Action::kP1MoveUp, 80.f, 150.f, "P1 Move Up", context);
	AddButtonLabel(Action::kP1TiltRight, 80.f, 200.f, "P1 Tilt Right", context);
	AddButtonLabel(Action::kP1TiltLeft, 80.f, 250.f, "P1 Tilt Left", context);
	AddButtonLabel(Action::kP1UsePowerUp, 80.f, 350.f, "P1 Pulse Bomb", context);

	AddButtonLabel(Action::kP2MoveUp, 400.f, 150.f, "P2 Move Up", context);
	AddButtonLabel(Action::kP2TiltRight, 400.f, 200.f, "P2 Tilt Right", context);
	AddButtonLabel(Action::kP2TiltLeft, 400.f, 250.f, "P2 Tilt Left", context);
	AddButtonLabel(Action::kP2UsePowerUp, 400.f, 350.f, "P2 Pulse Bomb", context);

	AddButtonLabel(Action::kMeteorSpawn, 720.f, 150.f, "Meteor Span", context);
	UpdateLabels();

	auto back_button = std::make_shared<gui::Button>(context);
	back_button->setPosition(80.f, 475.f);
	back_button->SetText("Back");
	back_button->SetCallback(std::bind(&KeySettingsState::RequestStackPop, this));
	m_gui_container.Pack(back_button);
}

void KeySettingsState::Draw()
{
	sf::RenderWindow& window = *GetContext().window;
	window.draw(m_background_sprite);
	window.draw(m_gui_container);
}

bool KeySettingsState::Update(sf::Time dt)
{
	return true;
}

bool KeySettingsState::HandleEvent(const sf::Event& event)
{
	bool is_key_binding = false;

	//Iterate through all of the key binding buttons to see if they are being presssed, waiting for the user to enter a key
	for (std::size_t action = 0; action < static_cast<int>(Action::kActionCount); ++action)
	{
		if (m_binding_buttons[action]->IsActive())
		{
			is_key_binding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				GetContext().player->AssignKey(static_cast<Action>(action), event.key.code);
				m_binding_buttons[action]->Deactivate();
			}
			break;
		}
	}

	//If pressed button changed key bindings, then update the labels
	if (is_key_binding)
	{
		UpdateLabels();
	}
	else
	{
		m_gui_container.HandleEvent(event);
	}
	return false;
}

void KeySettingsState::UpdateLabels()
{
	Player& player = *GetContext().player;
	for (std::size_t i = 0; i < static_cast<int>(Action::kActionCount); ++i)
	{
		sf::Keyboard::Key key = player.GetAssignedKey(static_cast<Action>(i));
		m_binding_labels[i]->SetText(Utility::toString(key));
	}
}

void KeySettingsState::AddButtonLabel(Action action, float x, float y, const std::string& text, Context context)
{
	m_binding_buttons[static_cast<int>(action)] = std::make_shared<gui::Button>(context);
	m_binding_buttons[static_cast<int>(action)]->setPosition(x, y);
	m_binding_buttons[static_cast<int>(action)]->SetText(text);
	m_binding_buttons[static_cast<int>(action)]->SetToggle(true);

	m_binding_labels[static_cast<int>(action)] = std::make_shared<gui::Label>("", *context.fonts);
	m_binding_labels[static_cast<int>(action)]->setPosition(x + 220.f, y + 15.f);

	m_gui_container.Pack(m_binding_buttons[static_cast<int>(action)]);
	m_gui_container.Pack(m_binding_labels[static_cast<int>(action)]);
}
