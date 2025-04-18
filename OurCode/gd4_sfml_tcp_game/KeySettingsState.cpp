/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#include "KeySettingsState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

KeySettingsState::KeySettingsState(StateStack& stack, Context context)
	: State(stack, context)
	, m_gui_container()
{
	m_background_sprite.setTexture(context.textures->Get(TextureID::kTitleScreen));
	for (std::size_t x = 0; x < 2; ++x)
	{
		AddButtonLabel(static_cast<int>(Action::kP1TiltLeft), x, 0, "Move Left", context);
		AddButtonLabel(static_cast<int>(Action::kP1TiltRight), x, 1, "Move Right", context);
		AddButtonLabel(static_cast<int>(Action::kP1MoveUp), x, 2, "Move Up", context);
		AddButtonLabel(static_cast<int>(Action::kP1UsePowerUp), x, 5, "Missile", context);
	}

	UpdateLabels();

	auto back_button = std::make_shared<gui::Button>(context);
	back_button->setPosition(860.f, 620.f);
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
	for (std::size_t action = 0; action < 2 * (static_cast<int>(Action::kActionCount)); ++action)
	{
		if (m_binding_buttons[action]->IsActive())
		{
			is_key_binding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				// Player 1
				if (action < static_cast<int>(Action::kActionCount))
					GetContext().keys1->AssignKey(static_cast<Action>(action), event.key.code);

				// Player 2
				else
					GetContext().keys2->AssignKey(static_cast<Action>(action - static_cast<int>(Action::kActionCount)), event.key.code);

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
	for (std::size_t i = 0; i < static_cast<int>(Action::kActionCount); ++i)
	{
		auto action = static_cast<Action>(i);

		// Get keys of both players
		sf::Keyboard::Key key1 = GetContext().keys1->GetAssignedKey(action);
		sf::Keyboard::Key key2 = GetContext().keys2->GetAssignedKey(action);

		// Assign both key strings to labels
		m_binding_labels[i]->SetText(Utility::toString(key1));
		m_binding_labels[i + static_cast<int>(Action::kActionCount)]->SetText(Utility::toString(key2));
	}
}

void KeySettingsState::AddButtonLabel(std::size_t index, std::size_t x, std::size_t y, const std::string& text, Context context)
{
	// For x==0, start at index 0, otherwise start at half of array
	index += static_cast<int>(Action::kActionCount) * x;

	m_binding_buttons[index] = std::make_shared<gui::Button>(context);
	m_binding_buttons[index]->setPosition(400.f * x + 80.f, 50.f * y + 300.f);
	m_binding_buttons[index]->SetText(text);
	m_binding_buttons[index]->SetToggle(true);

	m_binding_labels[index] = std::make_shared<gui::Label>("", *context.fonts);
	m_binding_labels[index]->setPosition(400.f * x + 300.f, 50.f * y + 315.f);

	m_gui_container.Pack(m_binding_buttons[index]);
	m_gui_container.Pack(m_binding_labels[index]);
}
