#pragma once
// Global Variables - Yeah Yeah
// These are here to limit parameter list sizes

extern sf::RenderWindow window(sf::VideoMode(1920, 1080), "SP Game", sf::Style::Fullscreen);
int MAPSIZE = 100;
std::vector<std::vector<Tile>> map;
float MODI = std::max(.2, (MAPSIZE) / (2000.0));