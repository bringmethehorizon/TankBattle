#pragma once
#include "screens.h"
class Menu:cScreen
{
public:
	Menu(void);
	virtual int run(sf::RenderWindow& App);
	~Menu(void);
private:
	int alpha_max;
	int alpha_div;
	bool playing;
};

