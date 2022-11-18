#pragma once
// Global Variables - Yeah Yeah
// These are here to limit parameter list sizes

extern sf::RenderWindow window(sf::VideoMode(1920, 1080), "SP Game", sf::Style::Fullscreen);
sf::View view(sf::Vector2f(1920, 1080), sf::Vector2f(1920, 1080));
int MAPSIZE = 100;
std::vector<std::vector<Tile>> map;
float MODI = std::max(.2, (MAPSIZE) / (2000.0));

int timer;
C tS = C(0, 0);
C tH = C(0, 0);

int xView = 0;
int yView = 0;