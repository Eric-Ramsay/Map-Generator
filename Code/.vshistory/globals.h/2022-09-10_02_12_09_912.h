#pragma once
// Global Variables - Yeah Yeah
// These are here to limit parameter list sizes
float W = 1920;
float H = 1080;

int MAPSIZE = 600;
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

sf::RenderTexture texture;
sf::RenderWindow window(sf::VideoMode(W, H), "TG2 DE", sf::Style::Fullscreen);
std::vector<UI_Building> buildings;
bool WINDOW_DRAW = false;

int building = -1;

float mX = 0;
float mY = 0;