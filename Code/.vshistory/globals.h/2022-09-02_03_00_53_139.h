#pragma once
// Global Variables - Yeah Yeah
// These are here to limit parameter list sizes

int W = 1920 / 1.0;
int H = 1080 / 1.0;

extern sf::RenderWindow window(sf::VideoMode(W, H), "SP Game", sf::Style::Fullscreen);
//sf::View view(sf::Vector2f(W/2.0, H/2.0), sf::Vector2f(W, H));
int MAPSIZE = 600;
std::vector<std::vector<Tile>> map;
float MODI = std::max(.2, (MAPSIZE) / (2000.0));
int zoom = 1;

int timer;
C tS = C(0, 0);
C tH = C(0, 0);

int xShift = 0;
int yShift = 0;
int xView = 0;
int yView = 0;