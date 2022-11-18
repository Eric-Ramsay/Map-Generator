#pragma once
#include <deque> //used for faster river generation, and for pathfinding


enum FORMAT {
	LEFT, RIGHT, CENTER
};

enum TERRAIN {
	MEADOW, GRASS_1, GRASS_2, GRASS_3, GRASS_4, GRASS_5, BARREN, ICE, DESERT, SHALLOW, RIVER, LAKE, OCEAN
};
enum ELEVATION {
	DEEP, LOW, FLAT, HIGH, HILL, MOUNTAIN
};

enum FOREST {
	NONE, TEMPERATE, JUNGLE, TAIGA, GLADE, SHRUB, FORAGE
};


struct C {
	int x;
	int y;
	C(int x1, int y1) {
		x = x1;
		y = y1;
	}
	bool operator==(const C rhs) {
		return this->x == rhs.x && this->y == rhs.y;
	}
	bool operator!=(const C	 rhs) {
		return this->x != rhs.x || this->y != rhs.y;
	}
};

struct Spot {
	int x;
	int y;
	short parent = -1; //Used only for pathfinding.
	float g = -1; //Distance to start. pathfinding.
	float h = -1; //Distance to end.   pathfinding.

	Spot(int x1 = 0, int y1 = 0, short p = -1) {
		x = x1;
		y = y1;
		parent = p;
	}

	bool operator==(const Spot rhs) {
		return this->x == rhs.x && this->y == rhs.y;
	}
	bool operator!=(const Spot rhs) {
		return this->x != rhs.x || this->y != rhs.y;
	}
};

struct Box {
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	Box() {};
	Box(int x1, int y1, int width, int height) {
		x = x1;
		y = y1;
		w = width;
		h = height;
	}
};

struct UI_Building {
	std::string name = "";

	//Costs - Wood, Stone, Gold, Workers, Specialists
	int costs[5] = {0, 0, 0, 0, 0};
	short time = 0;

	short anim_num = 1;
	short anim_speed = 1; //lower means faster
	UI_Building(std::string n = "", short w = 0, short s = 0, short g = 0, short work = 0, short spec = 0, short aN = 1, short aS = 1) {
		name = n;
		time = 5;
		anim_num = aN;
		anim_speed = aS;
		costs[0] = w;
		costs[1] = s;
		costs[2] = g;
		costs[3] = work;
		costs[4] = spec;
	}
};

enum BUILDING {
	NONE, ROAD, HOUSE, FARM, WATER_NET, MILL, LUMBER_CAMP, WELL, MINE, STOREHOUSE, ADMIN_OFFICE, WOOD_SHOP
};

struct Building {
	BUILDING type = NONE;
	short level = 0;
	short hp = 10;
};

std::vector<UI_Building> initBuildings() {
	std::vector<UI_Building> buildings = {};

	//------------------------------------WOOD STONE GOLD
	buildings.push_back(UI_Building("Road", 0, 4, 2, 0, 0));
	buildings.push_back(UI_Building("House", 5, 0, 5, 0, 0));
	buildings.push_back(UI_Building("Farm", 6, 0, 0, 4, 0));
	buildings.push_back(UI_Building("Water Net", 6, 0, 0, 3, 0));
	buildings.push_back(UI_Building("Well", 2, 10, 0, 0, 0));
	buildings.push_back(UI_Building("Lumber Camp", 15, 0, 0, 3, 1));
	buildings.push_back(UI_Building("Mill", 15, 5, 5, 3, 1));
	buildings.push_back(UI_Building("Mine", 20, 0, 15, 6, 2));
	buildings.push_back(UI_Building("Storehouse", 5, 15, 10, 4, 2));
	buildings.push_back(UI_Building("Administrative Office", 5, 5, 30, 0, 4));
	buildings.push_back(UI_Building("Wood Workshop", 15, 10, 15, 0, 4));

	return buildings;
}

struct Tile {
	char owner = 0;

	TERRAIN type = GRASS_1;
	ELEVATION elev = FLAT;
	FOREST forest = NONE;

	Building building;
};

struct MapTile {
	float heat = 50; // 0 to 100
	float rain = 50; //0 to 1000
	float river = 0;
	char type = 'g';
	float elevation = 5; //0 to 100
	bool forest = false;
	float forestChance = 0;
	int id[3] = { -1, -1, -1 };
	char it = -1;
	int checked = -1;
	int ways[4] = { 0, 0, 0, 0 };
};

enum RES {
	WORKERS, SPECIALISTS, W_WELLBEING, S_WELLBEING, G_JOBS, W_JOBS, S_JOBS, POWER, WOOD, STONE, GOLD, FOOD, WATER
};

//LUXURIES, COPPER, TIN, BRONZE, IRON, CATTLE, HORSES

struct Player {
	int turn = 1;

	std::vector<C> units = {};
	std::vector<C> land = {};
						//W   WW  S  SW  G  W  S  P
	float res[WATER+1] = {25, 0, 50, 50, 0, 0, 0, 50, 5, 5, 10, 1000, 1000};
	float max[WATER+1] = {50, 0, 100, 100, -1, -1, -1, 250, 25, 25, 300, 1000, 1000};
	float income[WATER + 1] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
};


