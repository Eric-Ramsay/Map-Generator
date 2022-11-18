#pragma once
// A place for misc. generic functions to reside

template <typename Type> std::string to_str(const Type& t) {
	std::ostringstream os;
	os << t;
	return os.str();
}


int safeC(int a, int MAX = MAPSIZE) {
	if (a >= MAX) {
		return a % MAX;
	}
	else if (a < 0) {
		return safeC(a + MAX, MAX);
	}
	return a;
}

int readInt(std::string& s) {
	int num = 0;
	bool reading_num = true;
	std::string copy = "";
	for (char c : s) {
		if (reading_num) {
			if (c >= '0' && c <= '9') {
				num *= 10;
				num += (int)(c - '0');
			}
			else {
				reading_num = false;
			}
		}
		else {
			copy += c;
		}
	}
	s = copy;
	return num;
}

std::string low(std::string a) {
	std::string word = "";
	bool skip = false;
	for (char c : a) {
		if (c == '*') {
			skip = !skip;
		}
		if (skip) {
			word += c;
		}
		else {
			word += std::tolower(c);
		}
	}
	return word;
}

Tile Terrain_Convert(MapTile t) {
	Tile til;
	static int mmax = 0;
	if (t.elevation <= 0 || t.type == 'e' || t.type == 'w' || t.type == 'r' || t.type == 'a') {
		til.elev = WATER;
	}
	else if (t.elevation >= 30) {
		if (t.elevation <= 50) {
			til.elev = FLAT;
		}
		else {
			til.elev = MOUNTAIN;
		}
	}
	if (t.forest != 'e') {
		switch (t.forest) {
		case 'e': til.forest = NONE; break;
		case 'f': til.forest = TEMPERATE; break;
		case 'b': til.forest = TAIGA; break;
		case 'n': til.forest = JUNGLE; break;
		case 'l': til.forest = GLADE; break;
		}
	}
	switch (t.type) {
	case 'w': til.type = OCEAN; break;
	case 'r': til.type = RIVER; break;
	case 'a': til.type = LAKE; break;
	case 'g': til.type = GRASS; break;
	case 'p': til.type = STEPPE; break;
	case 'v': til.type = SAVANNA; break;
	case 'q': til.type = COLD_DESERT; break;
	case 'd': til.type = DESERT; break;
	case 't': til.type = TUNDRA; break;
	case 'c': til.type = ICE; break;
	case 'o': til.type = BOG; break;
	case 'm': til.type = MEADOW; break;
	case 'e': til.type = SHALLOW; break;
	default: std::cout << t.type << std::endl; til.type = GRASS; break;
	}
	int max = 0;
	for (int i = 1; i < 4; i++) {
		if (t.ways[i] > t.ways[max]) {
			max = i;
		}
	}
	/*til.prevailing = max;
	til.num = t.ways[0] + t.ways[1] + t.ways[2] + t.ways[3];
	til.temp = t.heat;
	til.forest_C = std::min(255, t.forestChance);*/
	return til;
}


int measureText(std::string text) {
	bool skipping = false;
	int size = 0;
	for (char& c : text) {
		if (c == '*') {
			skipping = !skipping;
		}
		else if (!skipping) {
			if (c != '\n') {
				size++;
			}
		}
	}
	return size;
}
bool range(int x1, int y1, int x2, int y2, int width, int height) {
	if (height == -1) {
		height = width;
	}
	return (x1 >= x2 && y1 >= y2 && x1 <= x2 + width && y1 <= y2 + height);
}

std::string trimNum(int x, int throttle = -1, bool prettify = false) {
	char append = '!';
	int excess = 0;
	int num = x;
	if (num >= 1000) {
		excess = num;
		if (num >= 1000000) {
			append = 'm';
			num /= 1000000;
			excess -= num * 1000000;
			excess = (excess / 100000);
		}
		else {
			append = 'k';
			num /= 1000;
			excess -= num * 1000;
			excess = (excess / 100);
		}
	}
	std::string result = std::to_string(num);
	if (excess > 0 && (throttle == -1 || num < pow(10, throttle - 1))) {
		if (excess - (excess / 100) * 100 == 0) {
			excess = 100 * (excess / 100);
		}
		result += "." + std::to_string(excess);
	}
	if (append == '!') {
		return result;
	}
	if (prettify) {
		return result + "*GREY*" + append + "*GREY*";
	}
	return  result + append;
}

int typePrecedent(TERRAIN a) {
	switch (a) {
	case GRASS: return 1;
	case MEADOW: return 2;
	case STEPPE: return 3;
	case SAVANNA: return 4;
	case DESERT: case COLD_DESERT: return 13;
	case TUNDRA: return 6;
	case BOG: return 7;
	case ICE: return 12;
	case OCEAN: return -1;
	case SHALLOW: return 0;
	case RIVER: return -3;
	case LAKE: return -2;
	}
	return -1;
}

unsigned long long int srandC(unsigned long long int seed) {
	unsigned long long int a = 1103515245;
	int c = 12345;
	unsigned long long int m = 2 ^ 32;
	return ((a * seed + c) % m);
}

float randC(int x1, int y1) {
	unsigned long long int x = (unsigned long long int)x1;
	unsigned long long int y = (unsigned long long int)y1;
	unsigned long long int a = 1103515245;
	int c = 12345;
	long m = 2 ^ 32;
	unsigned long long int seed = srandC(x + y + x * y + x * x + y * y);
	return fabsf((float)((a * seed + c) % m) / m);
}
float square(float x) {
	return x * x;
}
int square(int x) {
	return x * x;
}
double square(double x) {
	return x * x;
}
int dist(int x1, int y1, int x2, int y2) {
	//This is 'Manhattan dist', since units can't move diagonally
	int xDist = std::abs(x1 - x2);
	int yDist = std::abs(y1 - y2);

	if (xDist > MAPSIZE / 2) {
		xDist = MAPSIZE - xDist;
	}
	if (yDist > MAPSIZE / 2) {
		yDist = MAPSIZE - yDist;
	}

	return xDist + yDist;
}

int dist(Spot start, Spot end) {
	return dist(start.x, start.y, end.x, end.y);
}

float fDist(int x1, int y1, int x2, int y2) {
	int xDist = std::abs(x1 - x2);
	int yDist = std::abs(y1 - y2);

	if (xDist > MAPSIZE / 2) {
		xDist = MAPSIZE - xDist;
	}
	if (yDist > MAPSIZE / 2) {
		yDist = MAPSIZE - yDist;
	}

	return std::sqrt(xDist * xDist + yDist * yDist);
}

double f(Spot S) {
	return S.g + S.h;
}

double tileCost(Tile t) {
	double price = 1.0;
	if (t.elev == WATER) {
		price = 3.0;
	}
	else if (t.elev == HILL) {
		price = 5.0;
	}
	else if (t.elev == MOUNTAIN) {
		price = 10.0;
	}
	if (t.forest != NONE) {
		price += 3.0;
	}
	return price;
}


float min(float a, float b) {
	if (a < b) {
		return a;
	}
	return b;
}

float max(float a, float b) {
	if (a < b) {
		return b;
	}
	return a;
}

bool inBox(int x, int y, Box b) {
	return (x >= b.x && y >= b.y && x <= b.x + b.w && y <= b.y + b.h);
}

std::deque<Spot> pathfind(std::vector<std::vector<Tile>>& map, Spot start, Spot end, bool fly = false) {
	std::deque<Spot> open;
	std::deque<Spot> closed;
	std::deque<Spot> path;

	bool flying = fly;

	if (dist(start, end) > 50) { //flyo mode. just be greedy
		flying = true;
	}

	if (start == end) {
		return {};
	}

	Spot current = start;
	Spot consider;

	current.g = 0;
	current.h = dist(start, end);
	open = { current };

	bool valid = true;
	int index = 0;

	while (open.size() > 0) {
		index = 0;
		for (int i = 1; i < open.size(); i++) {
			if (open[i].g + open[i].h < open[index].g + open[index].h) {
				index = i;
			}
		}
		current = open[index];
		open.erase(open.begin() + index);
		closed.push_back(current);

		if (current == end || (open.size() > 100 && flying)) { //At location. Return path.
			if (open.size() > 100 && flying) {
				path.push_front(end);
				index = 0;
				for (int i = 1; i < closed.size(); i++) {
					if (closed[i].g + closed[i].h < closed[index].h + closed[index].g) {
						index = i;
					}
				}
			}
			else {
				index = closed.size() - 1;
			}
			while (index > -1) {
				path.push_front(closed[index]);
				index = closed[index].parent;
			}
			return path;
		}

		for (int a = -1; a < 2; a++) {
			for (int b = -1; b < 2; b++) {
				if (a != 0 || b != 0) { //only check 4 adjacent tiles
					consider = Spot(safeC(current.x + a), safeC(current.y + b));

					consider.parent = closed.size() - 1; //Index of parent node, aka current's index in the closed list.
					if (!flying) {
						consider.g = current.g + 2 * tileCost(map[consider.y][consider.x]); //Distance to start
					}
					else {
						consider.g = 0;
					}
					consider.h = current.g + dist(consider, end);
					valid = true;

					for (int i = 0; i < std::max(open.size(), closed.size()); i++) {
						if (i < open.size()) {
							if (open[i].x == consider.x && open[i].y == consider.y) {
								valid = false;
								break;
							}
						}
						if (i < closed.size()) {
							if (closed[i].x == consider.x && closed[i].y == consider.y) {
								valid = false;
								break;
							}
						}
					}
					if (valid) { //Location has not been considered before. Add it to the open list.
						open.push_back(consider);
					}
				}
			}
		}
	}
	return {};
}

int path_length(std::vector<std::vector<Tile>>& map, int startX, int startY, int endX, int endY) {

	return 1;
}

int coord_dist(int a, int b) {
	int d = std::abs(a - b);
	if (d > MAPSIZE / 2) {
		return MAPSIZE - d;
	}
	return d;
}


float c_dist(C a, C b) {
	float x_dist = coord_dist(a.x, b.x);
	float y_dist = coord_dist(a.y, b.y);
	x_dist *= x_dist;
	y_dist *= y_dist;
	return sqrt(x_dist + y_dist);
}


bool c_less(int a, int b) {
	int d = std::abs(a - b); //coord less

	if (d > MAPSIZE / 2) {
		if (a > MAPSIZE / 2) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return a < b;
	}
}

C stoc(Spot s) {
	return C(s.x, s.y);
}

//SHALLOW, OCEAN, RIVER, LAKE, GRASS, STEPPE, SAVANNA, MEADOW, DESERT, TUNDRA, ICE, BOG
//TUNDRA, DESERT, STEPPE, GRASS, SAVANNAH, SHALLOW, RIVER, SHALLOW, ICE, BOG, MEADOW, OCEAN, COLD_DESERT
int sourceX(TERRAIN type) {
	return type * 32;
	return 0;
}

//NONE, TEMPERATE, JUNGLE, TAIGA, GLADE
int forestX(FOREST type) {
	int sX[GLADE + 1] = { 224, 0, 16, 32, 48 };
	return sX[type];
}



