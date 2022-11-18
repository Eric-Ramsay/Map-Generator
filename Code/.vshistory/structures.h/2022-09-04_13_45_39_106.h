#pragma once
#include <deque> //used for faster river generation, and for pathfinding


enum FORMAT {
	LEFT, RIGHT, CENTER
};

enum TERRAIN {
	MEADOW, GRASS_1, GRASS_2, GRASS_3, GRASS_4, GRASS_5, BARREN, ICE, DESERT, SHALLOW, RIVER, LAKE, OCEAN
};
enum ELEVATION {
	WATER, LOW, FLAT, HIGH, HILL, MOUNTAIN
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

	Spot(int x1 = 0, int y1 = 0) {
		x = x1;
		y = y1;
	}

	Spot(C coord) {
		x = coord.x;
		y = coord.y;
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
	std::vector<short> costs = {};
	short time = 0;

	short anim_num = 1;
	short anim_speed = 1; //lower means faster
	UI_Building(std::string n = "", short t = 5, std::vector<short> c = {}, short aN = 1, short aS = 1) {
		name = n;
		time = t;
		costs = c;
		anim_num = aN;
		anim_speed = aS;
	}
};

struct Building {
	short type = -1;
	short level = 0;
	short hp = 10;
};

std::vector<UI_Building> initBuildings() {
	std::vector<UI_Building> buildings = {};

	buildings.push_back(UI_Building("Farm", 5, {3, 0, 5, 0, 0}));
	buildings.push_back(UI_Building("House", 5, {3, 0, 5, 0, 0}));
	buildings.push_back(UI_Building("Lumber Camp", 5, {3, 0, 5, 0, 0}));
	buildings.push_back(UI_Building("Mine", 5, {3, 0, 5, 0, 0}));

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
	float elevation = 25; //0 to 100
	char forest = 'e';
	float forestChance = 0;
	int id[3] = { -1, -1, -1 };
	char it = -1;
	int checked = -1;
	int ways[4] = { 0, 0, 0, 0 };
};

"Influence",
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

enum RES {
	INFLUENCE, WOOD, STONE, GOLD, WORKERS, SPECIALISTS, FOOD, WATER, LUXURIES, COPPER, TIN, BRONZE, IRON, CATTLE, HORSES
};

struct Player {
	int gold = 6;
	char turn = 0;
	float upkeep = 0;
	float debt = 0;

	std::vector<C> units = {};
	std::vector<C> buildings = {};

	std::vector<short> resources = {};
	std::vector<short> max = {100, 25, 25, 4000, 50, 0, 1000, 1000, 50, 50, 50, 50, 50, 10, 10};

	int max_pop = 6;
	int max_buildings = 12;
	bool started = false; //Whether you get a free commander

};


