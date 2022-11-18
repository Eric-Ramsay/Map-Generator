#pragma once
// Global Variables - Yeah Yeah
// These are here to limit parameter list sizes
sf::RenderTexture texture;
sf::View view(sf::Vector2f(1920/2.0, 1080/2.0), sf::Vector2f(1920, 1080));
int MAPSIZE = 60;
std::vector<std::vector<Tile>> map;
std::vector<std::vector<MapTile>> base_map;
float MODI = std::max(.2, (MAPSIZE) / (2000.0));
float zoom = 2;

int timer;
C tS = C(0, 0);
C tH = C(0, 0);

int xShift = 0;
int yShift = 0;
int xView = 0;
int yView = 0;

std::vector<sf::Color> colors = {sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow};
Player p;