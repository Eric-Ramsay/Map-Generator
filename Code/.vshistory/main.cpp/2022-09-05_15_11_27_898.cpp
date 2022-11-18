#define NOMINMAX

// Hides the Console
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <algorithm>
namespace Gdiplus
{
	using std::min;
	using std::max;
};

#include "server.h"

#include <SFML\Graphics.hpp>

#include <vector>
#include <deque>
#include <sstream>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>

#include "structures.h"
#include "globals.h"
#include "functions.h"
#include "income.h"
#include "draw.h"
#include <thread>

#include "map.h"

std::deque<std::string> buffer = {}; //a buffer of msgs to send to the server
int turn = 0; //variable telling whose turn it is
int harvest = 1; // used to prevent fish from being harvested more than  once per turn
//SERVER SETTINGS -------------------------------------
bool connected = true;
std::string IP = "70.136.29.184";
//std::string IP = "192.168.1.140";
int port = 1234;
SOCKET sock;
std::vector<Player> players = {};

//Map Making Variables
bool MM_active = false;
int MM_selected = 0;
std::deque<Tile> MM_Tile = { Tile(), Tile(), Tile() };

//Game Loop Variables
float fTargetFrameTime = 1.0f / 60.0; // Virtual FPS of 60fps
float fAccumulatedTime = 0.0f;


//List of units, buildings, and policies
std::vector<UI_Building> buildings;
//-----------------------------------------------------

float mX = 0;
float mY = 0;
float elapsedTime = 0.0f;
float targetTime = .01f;

//--------------------------------------------------------------------------------------------

// Handles main game loop
int main() {
	auto clock = sf::Clock{};
	float FPS;
	bool inGame = true;
	
	float pop = 100;
	int housing = 200;

	for (int i = 0; i < 50; i++) {
		pop = pop * (1 + .03 * pop / housing);
		pop *= .999;
	}

	map = createMap(base_map);
	p.turn = 1;
	window.setVerticalSyncEnabled(true);
	srand(time(NULL));
	window.setFramerateLimit(61);

	std::vector<std::string> resources = {
		"Wood",
		"Stone",
		"Gold",
		"Workers",
		"Specialists",
		"Food",
		"Water",
		"Luxuries",
		"Copper",
		"Tin",
		"Bronze",
		"Iron",
		"Cattle",
		"Horses"
	};

	while (window.isOpen()) {
		//view = sf::View(sf::Vector2f(1920 / 2.0, 1080 / 2.0), sf::Vector2f(1920, 1080));
		//view.zoom(zoom);
		//window.setView(view);
		window.clear();


		FPS = 1.f / clock.getElapsedTime().asSeconds();

		elapsedTime += clock.getElapsedTime().asSeconds();
		clock.restart();

		sf::Event event;
		if (inGame) {
			//tH.x = safeC(xView + (sf::Mouse::getPosition().x - texture.getSize().x / 2.0) / (16.0 * zoom));
			//tH.y = safeC(yView - (sf::Mouse::getPosition().y - texture.getSize().y / 2.0) / (16.0 * zoom));

			//Mouse Coordinates on Screen
			mX = sf::Mouse::getPosition().x;
			mY = sf::Mouse::getPosition().y;

			float tX = texture.getSize().x;
			float tY = texture.getSize().y;

			//Scale Mouse Coordinates down to where they'd be on the texture
			mX /= (1920.0 / texture.getSize().x);
			mY /= (1080.0 / texture.getSize().y);

			tH.x = safeC(xView + floor((mX - (xShift + tX / 2.0)) / 16.0));
			tH.y = safeC(yView + floor((mY - (yShift + tY / 2.0)) / 16.0));

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (map[tH.y][tH.x].owner == 0) {
					bool canClaim = p.land.size() == 0 && map[tH.y][tH.x].elev > DEEP;
					if (!canClaim) {
						for (int i = -1; i < 2; i++) {
							for (int j = -1; j < 2; j++) {
								if (map[safeC(tH.y + i)][safeC(tH.x + j)].elev > DEEP) {
									if (map[safeC(tH.y + i)][safeC(tH.x + j)].owner == p.turn) {
										canClaim = true;
										break;
									}
								}
							}
						}
					}
					if (canClaim) {
						map[tH.y][tH.x].owner = p.turn;
						p.land.push_back(tH);
						p.power_cost = tilePower(p);
						if (p.power_cost > p.power) {
							p.land.pop_back();
							map[tH.y][tH.x].owner = 0;
							p.power_cost = tilePower(p);
						}
					}
				}
			}
		}

		// Event Handling ===================================================================================================
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::LostFocus) {
				inGame = false;
			}
			else if (event.type == sf::Event::GainedFocus) {
				inGame = true;
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::P) {
					elevSpread(base_map);
					float total = 0.0;
					for (int i = 0; i < MAPSIZE; i++) {
						for (int j = 0; j < MAPSIZE; j++) {
							map[i][j] = Terrain_Convert(base_map[i][j]);
							total += base_map[i][j].elevation;
						}
					}
					std::cout << "Avg Elevation: " << total / (MAPSIZE * MAPSIZE) << std::endl;
				}
				if (event.key.code == sf::Keyboard::R) {
					map = createMap(base_map);
				}
			}
			else if (event.type == sf::Event::MouseWheelMoved) {
				if (zoom < 2) {
					if (event.mouseWheel.delta < 0 && zoom >= 1) {
						zoom -= .1;
					}
					else if (event.mouseWheel.delta > 0) {
						zoom += .1;
					}
				}
				else {
					if (event.mouseWheel.delta < 0 && zoom > 1) {
						zoom -= .25;
					}
					else if (event.mouseWheel.delta > 0 && zoom < 6) {
						zoom += .25;
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (map[tH.y][tH.x].owner == p.turn) {
					//	map[tH.y][tH.x].building.type = (map[tH.y][tH.x].building.type + 1) % 4;
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					if (map[tH.y][tH.x].owner == p.turn) {
						if (map[tH.y][tH.x].building.type > -1) {
							map[tH.y][tH.x].building.type = -1;
						}
						else {
							map[tH.y][tH.x].owner = 0;
						}
					}
				}
			}
		}
		int shift = 4 - ((int)zoom)/2;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			yShift += shift;
			if (zoom < 1) {
				yView-=3;
				yShift = 0;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			yShift -= shift;
			if (zoom < 1) {
				yView+=3;
				yShift = 0;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			xShift += shift;
			if (zoom < 1) {
				xView-=3;
				xShift = 0;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			xShift -= shift;
			if (zoom < 1) {
				xView+=3;
				xShift = 0;
			}
		}
		if (xShift >= 16) {
			xView--;
			xShift -= 16;
		}
		if (yShift >= 16) {
			yView--;
			yShift -= 16;
		}
		if (xShift <= -16) {
			xView++;
			xShift += 16;
		}
		if (yShift <= -16) {
			yView++;
			yShift += 16;
		}
		// End of Event Handling ============================================================================================

		//Here stuff that shouldn't be linked to framerate is handled
		if (elapsedTime >= targetTime) {
			elapsedTime -= targetTime;
			timer++;
			if (timer >= 60) {
				timer -= 60;
				Income(p);
			}
		}

		//Drawing Code is handled here
		sf::Sprite sprite;
		texture.create(1920.0 / max(1, zoom), 1080.0 / max(1, zoom));
		texture.clear(sf::Color::Red);
		DrawMap();
		texture.display();
		sprite.setTexture(texture.getTexture());
		sprite.setScale(1920.0 / texture.getSize().x, 1080.0 / texture.getSize().y);
		window.draw(sprite);

		//Draw UI Elements Here
		WINDOW_DRAW = TRUE;

		drawSprite(528, 16, 16, 16, 3, 3, 20, 56);

		Print("Influence: " + trimNum(p.power - p.power_cost), 10, 10, 2, false);

		for (int i = 0; i < resources.size(); i++) {
			std::string prnt = resources[i] + ": " + trimNum(p.resources[i]);
			if (p.max[i] > -1) {
				prnt += +" / " + trimNum(p.max[i]);
			}
			Print(prnt, 10, 70 + 60 * i, 2, false);
		}
		
		window.display();
		WINDOW_DRAW = FALSE;
	}

	return 0;
}