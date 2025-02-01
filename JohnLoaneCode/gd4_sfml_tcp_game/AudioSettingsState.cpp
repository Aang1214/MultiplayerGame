#include "AudioSettingsState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"
#include "PauseState.hpp"
#include "Button.hpp"
#include <iostream>

template <typename T>
const T& clamp(const T& value, const T& min, const T& max) {
	return (value < min) ? min : (value > max) ? max : value;
}

AudioSettingsState::AudioSettingsState(StateStack& stack, Context context)
	:State(stack, context)
{
	sf::Texture& texture = context.textures->Get(TextureID::kTitleScreen);

	m_background_sprite.setTexture(texture);

	auto back_button = std::make_shared<gui::Button>(context);
	back_button->setPosition(382, 500);
	back_button->SetText("Back");
	back_button->SetCallback(std::bind(&AudioSettingsState::RequestStackPop, this));



	m_gui_container.Pack(back_button);
}

void AudioSettingsState::Draw()
{
	sf::RenderWindow& window = *GetContext().window;
	window.setView(window.getDefaultView());
	window.draw(m_background_sprite);
	window.draw(m_gui_container);
}

bool AudioSettingsState::Update(sf::Time dt)
{
	return true;
}

bool AudioSettingsState::HandleEvent(const sf::Event& event)
{
	m_gui_container.HandleEvent(event);
	return false;
}