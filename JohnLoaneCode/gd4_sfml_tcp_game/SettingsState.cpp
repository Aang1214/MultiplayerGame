#include "SettingsState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"
#include "Button.hpp"


SettingsState::SettingsState(StateStack& stack, Context context)
	:State(stack, context)
{
	sf::Texture& texture = context.textures->Get(TextureID::kTitleScreen);

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

	auto controls_button = std::make_shared<gui::Button>(context);
	controls_button->setPosition(382, 250);
	controls_button->SetText("Controls");
	controls_button->SetCallback([this]()
		{
			RequestStackPush(StateID::kKeySettings);
		});

	auto video_button = std::make_shared<gui::Button>(context);
	video_button->setPosition(382, 300);
	video_button->SetText("Video");
	video_button->SetCallback([this]()
		{
			RequestStackPush(StateID::kVideoSettings);
		});

	auto audio_button = std::make_shared<gui::Button>(context);
	audio_button->setPosition(382, 350);
	audio_button->SetText("Audio");
	audio_button->SetCallback([this]()
		{
			RequestStackPush(StateID::kAudioSettings);
		});

	auto back_button = std::make_shared<gui::Button>(context);
	back_button->setPosition(382, 500);
	back_button->SetText("Back");
	back_button->SetCallback(std::bind(&SettingsState::RequestStackPop, this));
	
	
	m_gui_container.Pack(back_button);
	m_gui_container.Pack(controls_button);
	m_gui_container.Pack(video_button);
	m_gui_container.Pack(audio_button);
}

void SettingsState::Draw()
{
	sf::RenderWindow& window = *GetContext().window;
	window.setView(window.getDefaultView());
	window.draw(m_background_sprite);
	window.draw(m_gui_container);
}

bool SettingsState::Update(sf::Time dt)
{
	return true;
}

bool SettingsState::HandleEvent(const sf::Event& event)
{
	m_gui_container.HandleEvent(event);
	return false;
}
void SettingsState::UpdateLabels()
{
	Player& player = *GetContext().player;
	for (std::size_t i = 0; i < static_cast<int>(Action::kActionCount); ++i)
	{
		sf::Keyboard::Key key = player.GetAssignedKey(static_cast<Action>(i));
		m_binding_labels[i]->SetText(Utility::toString(key));
	}
}

void SettingsState::AddButtonLabel(Action action, float x, float y, const std::string& text, Context context)
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
