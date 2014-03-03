#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <random>

class cScreen
{
public :
    virtual int run (sf::RenderWindow &App) = 0;
};