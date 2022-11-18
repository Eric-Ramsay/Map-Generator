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

struct UI_Building {
	std::string name = "";
	short gold_cost = 0;
	short stone_cost = 0;
	short wood_cost = 0;
	short time = 0;
};

struct UI_Building {
	bool top_color = false;
	bool bot_color = false;

	short HP = 0;
	short loot = 0;

	short cost = 0;
	short type = 0;
	short max_type = 0;

	short sX = 0;
	short sY = 16;

	short anim_num = 1;
	short anim_speed = 1; //lower means faster

	UI_Building(short c, short t, short h, bool tc = false, bool bc = false, short an = 1, short as = 1) {
		cost = c;
		type = t;
		HP = h;
		anim_num = an;
		anim_speed = as;
		top_color = tc;
		bot_color = bc;
	}
};

struct Tile {
	char owner = 0;

	TERRAIN type = GRASS_1;
	ELEVATION elev = FLAT;
	FOREST forest = NONE;

	int harvest = 0;

	char building = 0;
	char HP = 0;
	Unit unit;
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

struct Player {
	int gold = 6;
	char turn = 0;
	float upkeep = 0;
	float debt = 0;

	std::vector<C> units = {};
	std::vector<C> buildings = {};

	int max_pop = 6;
	int max_buildings = 12;
	bool started = false; //Whether you get a free commander

};


