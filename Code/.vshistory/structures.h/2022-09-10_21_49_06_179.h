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
	NONE, TEMPERATE, JUNGLE, TAIGA, GLADE, FORAGE
};


struct C {
	int x;
	int y;
	C(int x1, int y1) {
		x = x1;
		y = y1;
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

	//Costs - Unskilled Pop, Skilled Pop, Wood, Stone, Gold
	int costs[6] = { 0, 0, 0, 0, 0, 0 };
	short time = 0;

	short anim_num = 1;
	short anim_speed = 1; //lower means faster
	UI_Building(std::string n = "", short w = 0, short s = 0, short g = 0, short aN = 1, short aS = 1) {
		name = n;
		time = t;
		anim_num = aN;
		anim_speed = aS;
		costs[0] = work;
		costs[1] = spec;
		costs[2] = w;
		costs[3] = s;
		costs[4] = g;
	}
};

struct Building {
	short type = -1;
	short level = 0;
	short hp = 10;
};

enum BUILDINGS {
	HOUSE, FARM, WATER_NET, MILL, LUMBER_CAMP, MINE, STOREHOUSE
};

std::vector<UI_Building> initBuildings() {
	std::vector<UI_Building> buildings = {};

	//------------------------------------WOOD STONE GOLD
	buildings.push_back(UI_Building("House", 5, 0, 0));
	buildings.push_back(UI_Building("Farm", 5, 0, 0));
	buildings.push_back(UI_Building("Water Net", 3, 0, 0));
	buildings.push_back(UI_Building("Well", 1, 3, 0));
	buildings.push_back(UI_Building("Mill", 10, 1, 0));
	buildings.push_back(UI_Building("Lumber Camp", 15, 0, 0));
	buildings.push_back(UI_Building("Mine", 10, 5, 0));
	buildings.push_back(UI_Building("Storehouse", 5, 10, 0));

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
	float rain = 150; //0 to 1000
	float river = 0;
	char type = 'g';
	float elevation = 5; //0 to 100
	char forest = 'e';
	float forestChance = 0;
	int id[3] = { -1, -1, -1 };
	char it = -1;
	int checked = -1;
	int ways[4] = { 0, 0, 0, 0 };
};

enum RES {
	WOOD, STONE, GOLD, WORKERS, W_JOBS, SPECIALISTS, S_JOBS, FOOD, WATER, LUXURIES, COPPER, TIN, BRONZE, IRON, CATTLE, HORSES
};

struct Player {
	int turn = 1;
	int power = 250;
	float power_cost = 0;

	std::vector<C> units = {};
	std::vector<C> land = {};

	float resources[HORSES+1] = {5, 5, 5, 25, 0, 0, 0, 500, 500, 0, 0, 0, 0, 0, 0, 0};
	float max[HORSES+1] = {25, 25, -1, 50, -1, 0, -1, 1000, 1000, 50, 50, 50, 50, 50, 10, 10,};

};


