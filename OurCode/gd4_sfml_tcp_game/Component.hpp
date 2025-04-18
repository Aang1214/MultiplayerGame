/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#pragma once
#include <SFML/Graphics.hpp>
namespace gui
{
	class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
	{
	public:
		typedef std::shared_ptr<Component> Ptr;

	public:
		Component();
		virtual ~Component();
		virtual bool IsSelectable() const = 0;
		bool IsSelected() const;
		virtual void Select();
		virtual void Deselect();
		virtual bool IsActive() const;
		virtual void Activate();
		virtual void Deactivate();

		virtual void HandleEvent(const sf::Event& event) = 0;

	private:
		bool m_is_selected;
		bool m_is_active;
	};
}

