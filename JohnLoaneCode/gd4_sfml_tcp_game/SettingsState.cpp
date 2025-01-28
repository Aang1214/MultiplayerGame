#include "SettingsState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"
#include "Button.hpp"


SettingsState::SettingsState(StateStack& stack, Context context)
	:State(stack, context)
{
	sf::Texture& texture = context.textures->Get(TextureID::kTitleScreen);

	m_background_sprite.setTexture(texture);

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
