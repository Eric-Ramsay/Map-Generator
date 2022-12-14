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



// Handles main game loop
int main() {
	//Game Loop Variables
	float fTargetFrameTime = 1.0f / 60.0; // Virtual FPS of 60fps
	float fAccumulatedTime = 0.0f;
	float elapsedTime = 0.0f;
	float targetTime = .01f;
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
					if (UI.click == CLAIM && map[UI.tH.y][UI.tH.x].owner == 0) {
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
					else if (UI.click == BUILD && map[UI.tH.y][UI.tH.x].owner == p.turn && map[UI.tH.y][UI.tH.x].building == -1) {
						if (map[UI.tH.y][UI.tH.x].elev > DEEP && map[UI.tH.y][UI.tH.x].forest == BARE) {
							if (p.res[WOOD] >= UI.buildings[UI.bSelected].costs[0] && p.res[STONE] >= UI.buildings[UI.bSelected].costs[1] && p.res[GOLD] >= UI.buildings[UI.bSelected].costs[2]) {
								map[UI.tH.y][UI.tH.x].building = (BUILDING)UI.bSelected;
								p.res[WOOD] -= UI.buildings[UI.bSelected].costs[0];
								p.res[STONE] -= UI.buildings[UI.bSelected].costs[1];
								p.res[GOLD] -= UI.buildings[UI.bSelected].costs[2];
								if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))) {
									UI.click = CLAIM;
								}
							}
						}
					}
					else if (UI.click == CLEAR && map[UI.tH.y][UI.tH.x].owner == p.turn && map[UI.tH.y][UI.tH.x].forest != BARE) {
						if (p.res[WORKERS] >= 2) {
							p.res[WOOD]++;
							p.res[WORKERS] -= 2;
							map[UI.tH.y][UI.tH.x].forest = BARE;
						}
					}
					else if (UI.click == PLANT && map[UI.tH.y][UI.tH.x].owner == p.turn && map[UI.tH.y][UI.tH.x].building == -1 && map[UI.tH.y][UI.tH.x].forest == BARE) {
						if (p.res[WORKERS] >= 1 && p.res[WORKERS] >= 1) {
							p.res[WORKERS]--;
							p.res[WOOD]--;
							map[UI.tH.y][UI.tH.x].forest = GLADE;
						}
					}
				}

				if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					if (map[UI.tH.y][UI.tH.x].owner == p.turn) {
						if (map[UI.tH.y][UI.tH.x].building == -1) {
							map[UI.tH.y][UI.tH.x].owner = 0;
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
				UI.inGame = false;
			}
			else if (event.type == sf::Event::GainedFocus) {
				UI.inGame = true;
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					UI.click = CLAIM;
				}
				if (event.key.code == sf::Keyboard::C) {
					UI.click = CLEAR;
				}
				if (event.key.code == sf::Keyboard::P) {
					UI.click = PLANT;
				}
			}
			else if (event.type == sf::Event::MouseWheelMoved) {
				if (UI.zoom < 2) {
					if (event.mouseWheel.delta < 0 && UI.zoom >= 1) {
						UI.zoom -= .1;
					}
					else if (event.mouseWheel.delta > 0) {
						UI.zoom += .1;
					}
				}
				else {
					if (event.mouseWheel.delta < 0 && UI.zoom > 1) {
						UI.zoom -= .25;
					}
					else if (event.mouseWheel.delta > 0 && UI.zoom < 6) {
						UI.zoom += .25;
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (map[UI.tH.y][UI.tH.x].owner == p.turn) {
					//	map[UI.tH.y][UI.tH.x].building.type = (map[UI.tH.y][UI.tH.x].building.type + 1) % 4;
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					if (map[UI.tH.y][UI.tH.x].owner == p.turn) {
						if (map[UI.tH.y][UI.tH.x].building > -1) {
							map[UI.tH.y][UI.tH.x].building = -1;
						}
						else {
							map[UI.tH.y][UI.tH.x].owner = 0;
							p.res[POWER]++;
						}
					}
				}
			}
		}
		if (UI.inGame) {
			int shift = 4 - ((int)UI.zoom) / 2;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				UI.yShift += shift;
				if (UI.zoom < 1) {
					UI.yView -= 3;
					UI.yShift = 0;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				UI.yShift -= shift;
				if (UI.zoom < 1) {
					UI.yView += 3;
					UI.yShift = 0;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				UI.xShift += shift;
				if (UI.zoom < 1) {
					UI.xView -= 3;
					UI.xShift = 0;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				UI.xShift -= shift;
				if (UI.zoom < 1) {
					UI.xView += 3;
					UI.xShift = 0;
				}
			}
			if (UI.xShift >= 16) {
				UI.xView--;
				UI.xShift -= 16;
			}
			if (UI.yShift >= 16) {
				UI.yView--;
				UI.yShift -= 16;
			}
			if (UI.xShift <= -16) {
				UI.xView++;
				UI.xShift += 16;
			}
			if (UI.yShift <= -16) {
				UI.yView++;
				UI.yShift += 16;
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
		texture.create((UI.W - 320) / max(1, UI.zoom), UI.H / max(1, UI.zoom));
		texture.clear(sf::Color::Red);
		DrawMap();
		texture.display();
		sprite.setTexture(texture.getTexture());
		sprite.setScale((UI.W - 320) / texture.getSize().x, UI.H / texture.getSize().y);
		window.draw(sprite);

		//Draw UI Elements Here
		UI.WINDOW_DRAW = TRUE;

		DrawUI();
		
		window.display();
		UI.WINDOW_DRAW = FALSE;
	}

	return 0;
}