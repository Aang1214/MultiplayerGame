/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/
#ifndef VIDEO_SETTINGS_HPP
#define VIDEO_SETTINGS_HPP

extern int WIDTH;
extern int HEIGHT;

#endif VIDEO_SETTINGS_HPP
#pragma once
#include "State.hpp"
#include "Player.hpp"
#include "Container.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <vector>
#include <array>

class VideoSettingsState : public State
{
public:
	VideoSettingsState(StateStack& stack, Context context);
	virtual void Draw() override;
	virtual bool Update(sf::Time dt) override;
	virtual bool HandleEvent(const sf::Event& event) override;

private:
	sf::Sprite m_background_sprite;
	gui::Container m_gui_container;
};

