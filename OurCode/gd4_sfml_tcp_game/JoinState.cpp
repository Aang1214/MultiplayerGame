#include "JoinState.hpp"

JoinState::JoinState(StateStack& stack, Context context)
	:State(stack, context), m_ip_input(""), m_font(context.fonts->Get(Font::kMain))
{
	sf::Texture& texture = context.textures->Get(TextureID::kTitleScreen);

	m_background_sprite.setTexture(texture);

	auto back_button = std::make_shared<gui::Button>(context);
	back_button->setPosition(382, 500);
	back_button->SetText("Back");
	back_button->SetCallback(std::bind(&JoinState::RequestStackPop, this));
	m_gui_container.Pack(back_button);


	m_font = context.fonts->Get(Font::kMain);

	
	m_ip_text.setFont(m_font);
	m_ip_text.setCharacterSize(24);
	m_ip_text.setFillColor(sf::Color::White);
	m_ip_text.setPosition(250, 250);
}

void JoinState::Draw()
{
	sf::RenderWindow& window = *GetContext().window;
	window.setView(window.getDefaultView());
	window.draw(m_background_sprite);
	window.draw(m_gui_container);
	window.draw(m_ip_text);
}

bool JoinState::Update(sf::Time dt)
{
    return true;
}

bool JoinState::HandleEvent(const sf::Event& event)
{

	m_gui_container.HandleEvent(event);

	if (event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode == 8)
		{
			if (!m_ip_input.isEmpty())
				m_ip_input.erase(m_ip_input.getSize() - 1);
		}
		else if (event.text.unicode == 13) // Enter key (confirm input)
		{
			SaveIPToFile(m_ip_input.toAnsiString());
		}
		else if (event.text.unicode < 128) 
		{
			m_ip_input += static_cast<char>(event.text.unicode);
		}

		m_ip_text.setString(m_ip_input);
	}

	return false;
}

void JoinState::SaveIPToFile(const std::string& ip)
{
	
	std::ofstream file("ip_address.txt");  
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open ip_address.txt for writing!" << std::endl;
		return;
	}

	file << ip;  // Save the IP address
	file.close();

	std::cout << "IP saved to ip_address.txt in the current directory." << std::endl;
}
