#include "Menu.h"

Menu::Menu(void)
{
	alpha_max = 3 * 255;
	alpha_div = 3;
	playing = false;
}


Menu::~Menu(void)
{
}
int Menu::run(sf::RenderWindow &App)
{
	sf::Event Event;
	bool Running = true;
	sf::Texture Texture;
	sf::Sprite Sprite;
	int alpha = 0;
	sf::Font Font;
	sf::Text Menu1;
	sf::Text Menu2;
	sf::Text Menu3;
	int menu = 0;

	if (!Texture.loadFromFile("menubg.jpg"))
	{
		std::cerr << "Error loading picture!" << std::endl;
		return (-1);
	}
	Sprite.setTexture(Texture);
	Sprite.setColor(sf::Color(255, 255, 255, alpha));
	if (!Font.loadFromFile("Computerfont.ttf"))
	{
		std::cerr << "Error loading *.ttf" << std::endl;
		return (-1);
	}
	Menu1.setFont(Font);
	Menu1.setCharacterSize(30);
	Menu1.setString("New Game");
	Menu1.setPosition(280.f, 160.f);

	Menu2.setFont(Font);
	Menu2.setCharacterSize(30);
	Menu2.setString("Exit");
	Menu2.setPosition(280.f, 220.f);

	Menu3.setFont(Font);
	Menu3.setCharacterSize(30);
	Menu3.setString("Continue");
	Menu3.setPosition(280.f, 160.f);

	if (playing)
	{
		alpha = alpha_max;
	}

	while (Running)
	{
		while (App.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
			{
				return (-1);
			}
			if (Event.type == sf::Event::KeyPressed)
			{
				switch (Event.key.code)
				{
				case sf::Keyboard::Up:
					menu = 0;
					break;
				case sf::Keyboard::Down:
					menu = 1;
					break;
				case sf::Keyboard::Return:
					if (menu == 0)
					{
						playing = true;
						return (1);
					}
					else
					{
						return (-1);
					}
					break;
				default:
					break;
				}
			}
		}
		if (alpha<alpha_max)
		{
			alpha++;
		}
		Sprite.setColor(sf::Color(255, 255, 255, alpha / alpha_div));
		if (menu == 0)
		{
			Menu1.setColor(sf::Color(255, 0, 0, 255));
			Menu2.setColor(sf::Color(255, 255, 255, 255));
			Menu3.setColor(sf::Color(255, 0, 0, 255));
		}
		else
		{
			Menu1.setColor(sf::Color(255, 255, 255, 255));
			Menu2.setColor(sf::Color(255, 0, 0, 255));
			Menu3.setColor(sf::Color(255, 255, 255, 255));
		}
		App.clear();
		App.draw(Sprite);
		if (alpha == alpha_max)
		{
			//if (playing)
			//{
			//	App.draw(Menu3);
			//}
			//else
			//{
				App.draw(Menu1);
			//}
			App.draw(Menu2);
		}
		App.display();
	}

	return (-1);
}