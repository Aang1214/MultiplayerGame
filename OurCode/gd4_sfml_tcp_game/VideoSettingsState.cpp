/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#include "VideoSettingsState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"
int WIDTH = 1920;
int HEIGHT = 1080;

VideoSettingsState::VideoSettingsState(StateStack& stack, Context context)
	:State(stack, context)
{
	sf::Texture& texture = context.textures->Get(TextureID::kTitleScreen);

	m_background_sprite.setTexture(texture);

	auto fullsfc_button = std::make_shared<gui::Button>(context);
	fullsfc_button->setPosition(860, 400);
	fullsfc_button->SetText("Fullscreen");
	fullsfc_button->toggleFullscreen();

	auto back_button = std::make_shared<gui::Button>(context);
	back_button->setPosition(860, 500);
	back_button->SetText("Back");
	back_button->SetCallback(std::bind(&VideoSettingsState::RequestStackPop, this));

	m_gui_container.Pack(fullsfc_button);
	m_gui_container.Pack(back_button);
}

void VideoSettingsState::Draw()
{
	sf::RenderWindow& window = *GetContext().window;
	window.setView(window.getDefaultView());
	window.draw(m_background_sprite);
	window.draw(m_gui_container);
}

bool VideoSettingsState::Update(sf::Time dt)
{
	return true;
}

bool VideoSettingsState::HandleEvent(const sf::Event& event)
{
	m_gui_container.HandleEvent(event);
	return false;
}
