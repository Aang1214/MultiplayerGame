#pragma once
#include "Button.hpp"
#include "Label.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include <array>
#include "State.hpp"
#include "Player.hpp"
#include "Container.hpp"
#include "Action.hpp"

class SettingsState : public State
{
public:
	SettingsState(StateStack& stack, Context context);
	virtual void Draw() override;
	virtual bool Update(sf::Time dt) override;
	virtual bool HandleEvent(const sf::Event& event);

private:
	sf::Sprite m_background_sprite;
	gui::Container m_gui_container;
	std::array<gui::Button::Ptr, static_cast<int>(Action::kActionCount)> m_binding_buttons;
	std::array<gui::Label::Ptr, static_cast<int>(Action::kActionCount)> m_binding_labels;
};

