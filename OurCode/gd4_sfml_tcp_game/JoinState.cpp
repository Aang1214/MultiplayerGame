#include "JoinState.hpp"

JoinState::JoinState(StateStack& stack, Context context)
	:State(stack, context)
{
	sf::Texture& texture = context.textures->Get(TextureID::kTitleScreen);

	m_background_sprite.setTexture(texture);

	auto back_button = std::make_shared<gui::Button>(context);
	back_button->setPosition(382, 500);
	back_button->SetText("Back");
	back_button->SetCallback(std::bind(&JoinState::RequestStackPop, this));



	m_gui_container.Pack(back_button);
}

void JoinState::Draw()
{
	sf::RenderWindow& window = *GetContext().window;
	window.setView(window.getDefaultView());
	window.draw(m_background_sprite);
	window.draw(m_gui_container);
}

bool JoinState::Update(sf::Time dt)
{
    return true;
}

bool JoinState::HandleEvent(const sf::Event& event)
{
	m_gui_container.HandleEvent(event);
	return false;
}
