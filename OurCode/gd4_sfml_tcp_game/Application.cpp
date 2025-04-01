/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#include "Application.hpp"
#include "GameState.hpp"
#include "TitleState.hpp"
#include "MenuState.hpp"
#include "PauseState.hpp"
#include "KeySettingsState.hpp"
#include "GameOverState.hpp"
#include "VideoSettingsState.hpp"
#include "AudioSettingsState.hpp"
#include "SettingsState.hpp"
#include "JoinState.hpp"
#include "MultiplayerGameState.hpp"
#include "KeyBinding.hpp"

//***************************************      Main      **********************************************

//frame rate
const sf::Time Application::kTimePerFrame = sf::seconds(1.f/60.f);

//constructor (size of screen, title of window)
Application::Application() 
	: m_window(sf::VideoMode(WIDTH, HEIGHT), "Networked", sf::Style::Close)
	, m_key_binding_1(1), m_key_binding_2(2)
	, m_stack(State::Context(m_window, m_textures, m_fonts, m_music, m_sound, m_key_binding_1, m_key_binding_2))
{
	m_window.setKeyRepeatEnabled(false);
	m_fonts.Load(Font::kMain, "Media/Fonts/Sansation.ttf");
	m_textures.Load(TextureID::kTitleScreen, "Media/Textures/TitleScreen2.png");
	m_textures.Load(TextureID::kButtonNormal, "Media/Textures/ButtonNormal.png");
	m_textures.Load(TextureID::kButtonSelected, "Media/Textures/ButtonSelected.png");
	m_textures.Load(TextureID::kButtonActivated, "Media/Textures/ButtonPressed.png");
	m_textures.Load(TextureID::kButtons, "Media/Textures/Buttons.png");

	RegisterStates();
	m_stack.PushState(StateID::kTitle);
}

//run the game
void Application::Run()
{
	sf::Clock clock;
	sf::Time time_since_last_update = sf::Time::Zero;
	while (m_window.isOpen())
	{
		time_since_last_update += clock.restart();
		while(time_since_last_update > kTimePerFrame)
		{
			time_since_last_update -= kTimePerFrame;
			ProcessInput();
			Update(kTimePerFrame);

			if (m_stack.IsEmpty())
			{
				m_window.close();
			}
		}
		Render();

	}
}

void Application::ProcessInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		m_stack.HandleEvent(event);

		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
	}
}

void Application::Update(sf::Time dt)
{
	m_stack.Update(dt);

}

void Application::Render()
{
	m_window.clear();
	m_stack.Draw();
	m_window.display();
}

void Application::RegisterStates()
{
	m_stack.RegisterState<TitleState>(StateID::kTitle);
	m_stack.RegisterState<MenuState>(StateID::kMenu);
	m_stack.RegisterState<GameState>(StateID::kGame);

	m_stack.RegisterState<PauseState>(StateID::kPause);

	//m_stack.RegisterState<KeySettingsState>(StateID::kKeySettings);
	m_stack.RegisterState<GameOverState>(StateID::kGameOver, "Congratulation!");
	m_stack.RegisterState<VideoSettingsState>(StateID::kVideoSettings);
	m_stack.RegisterState<AudioSettingsState>(StateID::kAudioSettings);
	m_stack.RegisterState<SettingsState>(StateID::kSettings);
	m_stack.RegisterState<JoinState>(StateID::kJoin);
	m_stack.RegisterState<MultiplayerGameState>(StateID::kHostGame, true);

	m_stack.RegisterState<MultiplayerGameState>(StateID::kJoinGame, false); 
	m_stack.RegisterState<PauseState>(StateID::kNetworkPause, true);
}
