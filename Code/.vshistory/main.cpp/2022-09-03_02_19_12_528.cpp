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

Player p;
std::vector<Player> players = {};

//Map Making Variables
bool MM_active = false;
int MM_selected = 0;
std::deque<Tile> MM_Tile = { Tile(), Tile(), Tile() };

//Game Loop Variables
float fTargetFrameTime = 1.0f / 60.0; // Virtual FPS of 60fps
float fAccumulatedTime = 0.0f;

//Used for Dragging things when buying units/buildings, or moving units on the map
C drag = C(0, 0);
C crown = C(0, 0);
C heal = C(0, 0);
bool can_heal = false;
bool dragging_heal = false;
bool dragging_crown = false;
bool dragging_unit = false;
bool buying_unit = false;
bool buying_building = false;

//List of units, buildings, and policies
std::vector<UI_Unit> units;
std::vector<UI_Building> buildings;
std::vector<Policy> policies;
std::vector<int> UI_Units = {};
bool unit_UI = 0;
bool building_UI = false;
int scrolling = -1;
int unit_selected = 0;
int building_selected = 0;
bool attacking_unit = false;
bool verify_map = false;
bool drawMap = false;
int makeCiv = 0;
bool playMusic = true;
//-----------------------------------------------------

//Server Code---------------------------------------------------------------------------------
void sendData(std::string s) {
	buffer.push_back(s);
}
std::vector<Tile> readRow(std::string in) {
	int which = 0;
	std::vector<Tile> row = {};
	Tile t;
	for (char c : in) {
		switch (which) {
		case 0: t.type = (TERRAIN)(c - '0'); break; //Type
		case 1: t.elev = (ELEVATION)(c - '0');  break; //Elevation
		case 2: t.forest = (FOREST)(c - '0'); break; //Forest

		case 3: t.owner = (c - '0'); break; //Owner
		case 4: t.building = (c - '0'); break; //Building
		case 5: t.HP = (c - '0'); break; //Building HP

		case 6: t.unit.owner = (c - '0'); break; //U Owner
		case 7: t.unit.type = (c - '0'); break; //U Type
		case 8: t.unit.HP = (c - '0'); break; //U HP
		case 9: t.unit.MP = (c - '0'); break; //U MP
		case 10: t.unit.gold = (c - '0'); //U Gold
			row.push_back(t); t = Tile(); if (t.elev < FLAT) {
				if (t.type >= GRASS) { t.elev = FLAT; }
			}
			which = -1; break;
		}
		which++;
	}
	return row;
}
std::string sendTile(int x, int y) {
	std::string s = "t";
	//First the Coordinates
	s += to_str(x) + ".";
	s += to_str(y) + ".";
	//Now the Building Data
	s += '0' + map[y][x].type;
	s += '0' + map[y][x].elev;
	s += '0' + map[y][x].forest;

	return s;
}
std::string sendUnit(int x, int y) {
	std::string s = "";
	//First the Coordinates
	s += to_str(x) + ".";
	s += to_str(y) + ".";
	//Now the Unit Data
	s += '0' + map[y][x].unit.owner;
	s += '0' + map[y][x].unit.type;
	s += '0' + map[y][x].unit.HP;
	s += '0' + map[y][x].unit.MP;
	s += '0' + map[y][x].unit.gold;

	return s;
}
std::string sendBuilding(int x, int y) {
	std::string s = "";
	//First the Coordinates
	s += to_str(x) + ".";
	s += to_str(y) + ".";
	//Now the Building Data
	s += '0' + map[y][x].owner;
	s += '0' + map[y][x].building;
	s += '0' + map[y][x].HP;

	return s;
}
std::string requestRow(int index) {
	std::string s = "r";
	s += to_str(index) + ".";
	std::cout << "Requesting: " << s << std::endl;
	return s;
}
void unitChange(std::string s) {
	if (s == "") {
		return;
	}
	int x = readInt(s);
	int y = readInt(s);

	map[y][x].unit.owner = s[0] - '0';
	map[y][x].unit.type = s[1] - '0';
	map[y][x].unit.HP = s[2] - '0';
	map[y][x].unit.MP = s[3] - '0';
	map[y][x].unit.gold = s[4] - '0';

	unitChange(s.substr(5));
}
void buildingChange(std::string s) {
	if (s == "") {
		return;
	}
	int x = readInt(s);
	int y = readInt(s);

	map[y][x].owner = s[0] - '0';
	map[y][x].building = s[1] - '0';
	map[y][x].HP = s[2] - '0';

	if (map[y][x].forest != NONE && map[y][x].building > 0) {
		map[y][x].forest = NONE;
	}

	buildingChange(s.substr(3));
}
void tileChange(std::string s) {
	int x = readInt(s);
	int y = readInt(s);

	map[y][x].type = (TERRAIN)(s[0] - '0');
	map[y][x].elev = (ELEVATION)(s[1] - '0');
	map[y][x].forest = (FOREST)(s[2] - '0');
}

void client_join() {
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	WSAStartup(ver, &data);

	sock = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, IP.c_str(), &hint.sin_addr);

	int connection = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connection == SOCKET_ERROR) {
		std::cout << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
	}
}
void client_send() {
	int target = 0;
	while (connected) {
		if (buffer.size() > 0) {
			if (timer == target) {
				std::string s = buffer.front();
				send(sock, s.c_str(), s.size() + 1, 0);
				target = (timer + 5) % 500;
				buffer.pop_front();
			}
		}
		else {
			target = (timer + 5) % 500;
		}
	}
	closesocket(sock);
	WSACleanup();
}
void client_listen() {
	char buf[600];
	while (connected) {
		int bytesReceived = recv(sock, buf, 600, 0);
		if (bytesReceived > 1) {
			//std::cout << "Server: " << buf << std::endl;
			std::string msg(buf);
			char c = msg[0];
			char context = msg[1] - '0';
			char sender = msg[2] - '0';
			static int num = 0;
			bool getMessage = (sender == 0) || (context == 0 && sender == p.turn) || (context == 1 && sender != p.turn);

			if (p.turn == 0) {
				if (c == 't') { //Assign Player Turn
					std::cout << "My turn is now " << '0' + sender << std::endl;
					p.turn = sender;
				}
				num++;
			}
			else if (getMessage) {
				std::cout << "Server: " << num++ << " " << c << std::endl;
				msg = msg.substr(3);

				if (c == 'r') { //Get Row
					if (map.size() > 0) {
						int index = readInt(msg);
						std::cout << "Received Row: " << index << std::endl;
						std::vector<Tile> row = readRow(msg);
						map[index] = row;
					}
				}
				else if (c == 'm') { //Map Size
					std::cout << "Server: " << num << std::endl;
					MAPSIZE = std::stoi(msg);
					if (!drawMap) {
						xView = MAPSIZE / 2;
						yView = MAPSIZE / 2;
					}
					if (map.size() == 0) {
						map.resize(MAPSIZE);
						MODI = std::max(.4, (MAPSIZE) / (1000.0));
					}
					else {
						verify_map = true;
					}
				}
				else if (c == 'u') { //Unit Change
					unitChange(msg);
				}
				else if (c == 'b') { //Building/Owner Change
					buildingChange(msg);
				}
				else if (c == 'g') { //Tile Type Changed - prob not used
					tileChange(msg);
				}
				else if (c == 'e') { //Set Turn
					turn = std::stoi(msg);
				}
				else if (c == 'G') { //Set Gold
					p.gold = std::stoi(msg);
				}
			}
			ZeroMemory(buf, 600);
		}
		else {
			closesocket(sock);
			WSACleanup();
			exit(1);
		}
	}
}
void serverInit() {
	//Set Up Server--------------------------------------
	//std::cout << "IP: ";
	//std::cin >> IP;
	//std::cout << "Port: ";
	//std::cin >> port;
	client_join();
	std::thread cListA(&client_listen);
	std::thread cListB(&client_send);
	cListA.detach();
	cListB.detach();
	//-----------------------------------------------------
}

float mX = 0;
float mY = 0;
float elapsedTime = 0.0f;
float targetTime = .01f;

//--------------------------------------------------------------------------------------------

// Handles main game loop
int main() {
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "TG2 DE", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
	srand(time(NULL));
	
	view.zoom(zoom);
	auto clock = sf::Clock{};
	window.setFramerateLimit(61);
	float FPS;
	sf::RectangleShape shape(view.getSize());
	bool inGame = true;
	//serverInit();
	std::vector<std::vector<MapTile>> base_map;
	map = createMap(base_map);

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
			float mX = sf::Mouse::getPosition().x;
			float mY = sf::Mouse::getPosition().y;
			float tX = texture.getSize().x;
			float tY = texture.getSize().y;

			//Scale Mouse Coordinates down to where they'd be on the texture
			mX /= (1920.0 / texture.getSize().x);
			mY /= (1080.0 / texture.getSize().y);

			tH.x = safeC(xView + ((mX - xShift) - tX/2.0) / 16);
			tH.y = safeC(yView + ((mY - yShift) - tY / 2.0) / 16);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				map[tH.y][tH.x].elev = MOUNTAIN;
				map[safeC(tH.y + 1)][tH.x].elev = HILL;
				map[safeC(tH.y + 2)][tH.x].elev = HIGH;
				map[safeC(tH.y + 3)][tH.x].elev = FLAT;
				map[safeC(tH.y + 4)][tH.x].elev = LOW;
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
				if (event.KeyCode == sf::Key::P) {
					elevSpread(base_map);
				}
			}
			else if (event.type == sf::Event::MouseWheelMoved) {
				if (zoom < 2) {
					if (event.mouseWheel.delta < 0 && zoom > 1) {
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
		}
		int shift = 4 - ((int)zoom)/2;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			yShift += shift;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			yShift -= shift;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			xShift += shift;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			xShift -= shift;
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
			
		}

		//Drawing Code is handled here
		texture.create(1920.0 / zoom, 1080.0 / zoom);
		texture.clear(sf::Color::Red);
		DrawMap();

		texture.display();
		sf::Sprite sprite;
		sprite.setTexture(texture.getTexture());
		sprite.setScale(1920.0/texture.getSize().x, 1080.0/texture.getSize().y);
		window.draw(sprite);
		window.display();
	}

	return 0;
}