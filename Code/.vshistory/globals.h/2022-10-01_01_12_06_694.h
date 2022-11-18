#pragma once
// Global Variables - Yeah Yeah
// These are here to limit parameter list sizes


int MAPSIZE = 500;
std::vector<std::vector<Tile>> map;
std::vector<std::vector<MapTile>> base_map;
float MODI = std::max(.2, (MAPSIZE) / (2000.0));

int timer;

SEASON season = SPRING;


std::vector<sf::Color> colors = {sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow};
Player p;

sf::RenderTexture texture;
sf::RenderWindow window;

UIState UI;





