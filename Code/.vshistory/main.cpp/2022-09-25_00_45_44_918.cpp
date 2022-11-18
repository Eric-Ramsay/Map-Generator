#define NOMINMAX

// Hides the Console
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
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

//-----------------------------------------------------
float elapsedTime = 0.0f;
float targetTime = .01f;

//--------------------------------------------------------------------------------------------

// Handles main game loop
int main() {
	UI.W = 1920;
	UI.H = 1080;
	window.create(sf::VideoMode(UI.W, UI.H), "TG2 DE", sf::Style::Fullscreen);

	auto clock = sf::Clock{};
	float FPS;
	
	float pop = 10;
	int housing = 100;

	map = createMap(base_map);
	p.turn = 1;
	window.setVerticalSyncEnabled(true);
	srand(time(NULL));
	window.setFramerateLimit(61);

	UI.buildings = initBuildings();

	while (window.isOpen()) {
		//view = sf::View(sf::Vector2f(1920 / 2.0, 1080 / 2.0), sf::Vector2f(1920, 1080));
		//view.zoom(zoom);
		//window.setView(view);
		window.clear();


		FPS = 1.f / clock.getElapsedTime().asSeconds();

		elapsedTime += clock.getElapsedTime().asSeconds();
		clock.restart();

		sf::Event event;
		if (UI.inGame) {
			//tH.x = safeC(xView + (sf::Mouse::getPosition().x - texture.getSize().x / 2.0) / (16.0 * zoom));
			//tH.y = safeC(yView - (sf::Mouse::getPosition().y - texture.getSize().y / 2.0) / (16.0 * zoom));

			//Mouse Coordinates on Screen
			UI.mX = sf::Mouse::getPosition().x;
			UI.mY = sf::Mouse::getPosition().y;

			float tX = texture.getSize().x;
			float tY = texture.getSize().y;

			//Scale Mouse Coordinates down to where they'd be on the texture
			UI.mX /= ((UI.W - 320) / texture.getSize().x);
			UI.mY /= (UI.H / texture.getSize().y);

			UI.tH.x = safeC(UI.xView + floor((UI.mX - (UI.xShift + tX / 2.0)) / 16.0));
			UI.tH.y = safeC(UI.yView + floor((UI.mY - (UI.yShift + tY / 2.0)) / 16.0));

			if (UI.inGame && sf::Mouse::getPosition().x < (UI.W - 320)) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					if (map[UI.tH.y][UI.tH.x].owner == 0) {
						bool canClaim = p.land.size() == 0;
						if (!canClaim) {
							for (int i = -1; i < 2; i++) {
								for (int j = -1; j < 2; j++) {
									if (map[safeC(UI.tH.y + i)][safeC(UI.tH.x + j)].owner == p.turn) {
										canClaim = true;
										i = 2;
										j = 2;
									}
								}
							}
						}
						if (canClaim && p.res[POWER] >= 1) {
							map[UI.tH.y][UI.tH.x].owner = p.turn;
							p.res[POWER] -= 1;
							p.land.push_back(UI.tH);
						}
					}
					else if (UI.building > 0 && map[UI.tH.y][UI.tH.x].owner == p.turn && map[UI.tH.y][UI.tH.x].building == UI.NONE) {
						if (map[UI.tH.y][UI.tH.x].elev > DEEP && map[UI.tH.y][UI.tH.x].forest == UI.NONE) {
							if (p.res[WOOD] > buildings[building].costs[0] && p.res[STONE] > buildings[building].costs[1] && p.res[GOLD] > buildings[building].costs[2]) {
								map[tH.y][tH.x].building = (BUILDING)building;
								p.res[WOOD] -= buildings[building].costs[0];
								p.res[STONE] -= buildings[building].costs[1];
								p.res[GOLD] -= buildings[building].costs[2];
								if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))) {
									building = 0;
								}
							}
						}
					}
				}

				if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					if (map[tH.y][tH.x].owner == p.turn) {
						if (map[tH.y][tH.x].building == NONE) {
							map[tH.y][tH.x].owner = 0;
							p.res[POWER]++;
						}
					}
				}
			}

			UI.mX = sf::Mouse::getPosition().x;
			UI.mY = sf::Mouse::getPosition().y;
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
				if (event.key.code == sf::Keyboard::Escape) {
					building = 0;
				}
				static int sea_level = 0;
				if (event.key.code == sf::Keyboard::R) {
					sea_level = 0;
					map = createMap(base_map);
				}
				if (event.key.code == sf::Keyboard::T) {
					sea_level++;
					for (int i = 0; i < MAPSIZE; i++) {
						for (int j = 0; j < MAPSIZE; j++) {
							if (base_map[i][j].elevation > 0 && base_map[i][j].elevation < sea_level) {
								map[i][j].elev = DEEP;
								map[i][j].forest = BARE;
								map[i][j].type = SHALLOW;
							}
						}
					}
				}
				static int num_reduced = 0;
				if (event.key.code == sf::Keyboard::Y) {
					std::cout << ++num_reduced << std::endl;
					for (int i = 0; i < MAPSIZE; i++) {
						for (int j = 0; j < MAPSIZE; j++) {
							base_map[i][j].rain--;
							//base_map[i][j].forest = 'e';
						}
					}
					finalize(base_map, false);
					for (int i = 0; i < MAPSIZE; i++) {
						for (int j = 0; j < MAPSIZE; j++) {
							map[i][j] = Terrain_Convert(base_map[i][j]);
						}
					}
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
						if (map[tH.y][tH.x].building > NONE) {
							map[tH.y][tH.x].building = NONE;
						}
						else {
							map[tH.y][tH.x].owner = 0;
							p.res[POWER]++;
						}
					}
				}
			}
		}
		if (inGame) {
			int shift = 4 - ((int)zoom) / 2;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				yShift += shift;
				if (zoom < 1) {
					yView -= 3;
					yShift = 0;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				yShift -= shift;
				if (zoom < 1) {
					yView += 3;
					yShift = 0;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				xShift += shift;
				if (zoom < 1) {
					xView -= 3;
					xShift = 0;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				xShift -= shift;
				if (zoom < 1) {
					xView += 3;
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
		}
		// End of Event Handling ============================================================================================

		//Here stuff that shouldn't be linked to framerate is handled
		if (elapsedTime >= targetTime) {
			elapsedTime -= targetTime;
			timer++;
			if (timer >= 300) {
				timer -= 300;
				Income(p);
			}
		}

		//Drawing Code is handled here
		sf::Sprite sprite;
		sprite.setPosition(0, 0);
		texture.create((W - 320) / max(1, zoom), H / max(1, zoom));
		texture.clear(sf::Color::Red);
		DrawMap();
		texture.display();
		sprite.setTexture(texture.getTexture());
		sprite.setScale((W - 320) / texture.getSize().x, H / texture.getSize().y);
		window.draw(sprite);

		//Draw UI Elements Here
		WINDOW_DRAW = TRUE;

		DrawUI();
		
		window.display();
		WINDOW_DRAW = FALSE;
	}

	return 0;
}