#include <SFML/Graphics.hpp>
#include <windows.h>
#include "Game.h"
#include "Menu.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
	std::vector<cScreen*> Screens;
	int screen=0;
	sf::RenderWindow App(sf::VideoMode(640, 480, 32), "Tank Battle");
	Menu m;
	Screens.push_back((cScreen*)&m);
	Game* g=new Game(1);
	Screens.push_back((cScreen*)g);
	while (screen >= 0 || !App.isOpen())
	{
		screen = Screens[screen]->run(App);
		if(screen==0) 
		{
			delete Screens[1];
			Screens[1]=(cScreen*)new Game(1);
		}
		else if(screen==999)
		{
			delete Screens[1];
			Screens[1]=(cScreen*)new Game(1);
			screen=0;
		}
		else if(screen>=10)
		{
			delete Screens[1];
			screen-=10;
			Screens[1]=(cScreen*)new Game(screen);
			screen=1;
		}
	}
	//Game g;
	//g.run();
	return 0;
}