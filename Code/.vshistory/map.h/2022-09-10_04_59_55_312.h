#pragma once

int NUM_THREADS = 4;

float latTemp(int x) {
	//return 100 - square((float)(x - MAPSIZE / 2.0) / (MAPSIZE / 20.0));
	//return (100.0 / square(1.0 + square((x - MAPSIZE/2) / (MAPSIZE/3.0))));
	return 100 * (1 - (abs(MAPSIZE / 2 - x) / (float)(MAPSIZE / 2)));
}


void elev(std::vector<std::vector<MapTile>>& map, int size, int num, int elev) {
	int s = size;
	int n = num;
	int dir = 0;
	int lastDir = 0;
	int yC = (int)floor((((double)rand() / RAND_MAX)) * MAPSIZE);
	int xC = (int)floor((((double)rand() / RAND_MAX)) * MAPSIZE);
	while (s > 0 && n > 0) {
		if (s == 1) {
			n--;
			s = size; //Ocean size is too small, go to new location to begin making ocean
			yC = (int)floor((((double)rand() / RAND_MAX)) * MAPSIZE);
			xC = (int)floor((((double)rand() / RAND_MAX)) * MAPSIZE);
		}
		else {
			yC = safeC(yC);
			xC = safeC(xC);
			if (map[yC][xC].type != 'w') {
				map[yC][xC].elevation = elev;
				if (elev <= 0) {
					map[yC][xC].type = 'w';
				}
			}
			do {
				dir = rand() % 4;
			} while (dir == lastDir);
			switch (dir) {
			case 0: yC++; break;
			case 1: xC++; break;
			case 2: yC--; break;
			case 3: xC--; break;
			}
			if (lastDir < 2) { lastDir = dir + 2; }
			else { lastDir = dir - 2; } //Asign Last Dir
			s--;
		}
	}
}

void elevSpread(std::vector< std::vector<MapTile >>& map) {
	for (int m = 4; m > 0; m--) {
		for (int h = 1; h >= 0; h--) {
			int i = h * (MAPSIZE - 1);
			while (i < MAPSIZE && i >= 0) {
				int j = h * (MAPSIZE - 1);
				while (j < MAPSIZE && j >= 0) {
					int Min = 100;
					int Max = -100;
					float avg = 0;
					for (int a = -1; a < 2; a++) {
						for (int b = -1; b < 2; b++) {
							if (a * b == 0 && a != b) {
								if (map[safeC(i + a)][safeC(j + b)].elevation > Max) { Max = map[safeC(i + a)][safeC(j + b)].elevation; }
								if (map[safeC(i + a)][safeC(j + b)].elevation < Min) { Min = fmax(0, map[safeC(i + a)][safeC(j + b)].elevation); }
								if (map[i][j].elevation > 0) {
									avg += fmax(0, map[safeC(i + a)][safeC(j + b)].elevation);
								}
								else {
									avg += fmin(0, (int)map[safeC(i + a)][safeC(j + b)].elevation);
								}
							}
						}
					}

					if (map[i][j].elevation > 0) {
						avg /= 4.0;
						if (map[i][j].elevation < Max) {
							if (map[i][j].elevation < 30) { // LOW
								map[i][j].elevation = (map[i][j].elevation * 14 + avg) / 15.0;
							}
							else if (map[i][j].elevation <= 50) { // FLAT
								if (map[i][j].elevation > avg) {
									map[i][j].elevation = (map[i][j].elevation * 3 + avg) / 4.0;
								}
								else {
									map[i][j].elevation = (map[i][j].elevation + avg) / 2.0;
								}
							}
							else if (map[i][j].elevation <= 70) { //HIGH
								map[i][j].elevation = (map[i][j].elevation * 4 + avg) / 5.0;
							}
							else if (map[i][j].elevation <= 85) { //HILL
								map[i][j].elevation = (map[i][j].elevation * 4 + avg) / 5.0;
							}
							else { //MOUNTAIN
								map[i][j].elevation = (map[i][j].elevation + avg) / 2.0;
							}
						}
						else {
							map[i][j].elevation = (map[i][j].elevation * 19 + avg) / 20.0;
						}
					}
					else {
						avg += 2 * min(Max, 0);
						avg /= 6.0;
						map[i][j].elevation = avg;
					}
					if (h == 0) { j++; }
					else { j--; }
				}
				if (h == 0) { i++; }
				else { i--; }
			}
		}
	}
}


void forestSpread(std::vector< std::vector<MapTile >>& map) {
	for (int m = 4; m > 0; m--) {
		for (int h = 1; h >= 0; h--) {
			int i = h * (MAPSIZE - 1);
			while (i < MAPSIZE && i >= 0) {
				int j = h * (MAPSIZE - 1);
				while (j < MAPSIZE && j >= 0) {
					float avg = 0;
					float num = 0;
					if (map[i][j].elevation > 0 && map[i][j].elevation < 75) {
						for (int a = -1; a < 2; a++) {
							for (int b = -1; b < 2; b++) {
								if (map[safeC(i + a)][safeC(j + b)].elevation > 0 && map[safeC(i + a)][safeC(j + b)].elevation < 75) {
									avg += map[safeC(i + a)][safeC(j + b)].forestChance;
									num++;
									if (a == 0 && b == 0) {
										avg += map[safeC(i + a)][safeC(j + b)].forestChance;
										num++;
									}
								}
							}
						}
						avg /= num;
						map[i][j].forestChance = (avg * (.99 + ((((double)rand() / RAND_MAX)) * .02)));
					}
					else {
						map[i][j].forestChance = 75;
					}
					if (h == 0) { j++; }
					else { j--; }
				}
				if (h == 0) { i++; }
				else { i--; }
			}
		}
	}
}

void currents(std::vector<std::vector<MapTile>>& map, bool fir, int id) {
	float ticks = 0;
	auto makeCurrent = [&](int yC, int xC) {
		int limit = 40;
		float water = 20 + latTemp(yC) / 2;
		int y = safeC(yC);
		int x = safeC(xC);
		int y1;
		int x1;
		int dir = 0;
		float temp = fmax(0, map[y][x].heat);
		int lastDir = 0;
		int bannedDir = -1;
		float cS = 0;
		MapTile* t;
		if (!fir) {
			water *= 3;
		}
		while (cS < limit && water > 0) {
			ticks++;
			y = safeC(y);
			x = safeC(x);
			MapTile til = map[y][x];
			if ((til.type == 'w' || til.type == 'a' || til.type == 'r')) {
				water += 1 + til.heat / 35;
				cS++;
			}
			for (int a = -2; a < 2; a++) {
				for (int b = -2; b < 2; b++) {
					if (a != 0 || b == 0) {
						y1 = safeC(y + a);
						x1 = safeC(x + b);
						if (map[y1][x1].elevation > 0) {
							switch (dir) {
							case 0: map[y1][x1].ways[0]++; break;
							case 1: map[y1][x1].ways[1]++; break;
							case 2: map[y1][x1].ways[2]++; break;
							case 3: map[y1][x1].ways[3]++; break;
							}
							t = &map[y1][x1];
							if (t->heat > temp) { //Colder current than land. Decrease land temp
								t->heat -= (100 - temp) / 1200;
								t->forestChance += .01;
							}
							else if (t->heat < temp) {  //Hot current over cold land. increase land temp
								t->heat += .01;
								t->rain += .02;
							}

							float rainfall = 0;
							if (t->rain < 195) {
								rainfall = 1;
							}
							else if (t->rain < (200 + latTemp(yC))) {
								rainfall = .5;
							}
							else {
								rainfall = .3;
							}
							if (!fir) {
								rainfall = fmax(.1, 2 * rainfall);
							}
							t->rain += rainfall;
							water -= t->heat / 4000.0;
							if (t->heat > 2) {
								t->heat -= t->elevation / 15000.0;
								if (t->elevation > 50) {
									water -= t->elevation / 4000.0;
								}
							}
						}
					}
				}
			}
			int maxY = -1;
			int maxX = 0;
			//use something like simulated annealing
			//if cS is low relative to limit, move randomly
			//if ur near death play it safe
			bool stayCourse = (rand() % limit < cS);

			if (map[y][x].elevation <= 0 && stayCourse) {
				for (int a = -1; a < 2; a++) {
					for (int b = -1; b < 2; b++) {
						if (a * b == 0 && a != b) {
							if (maxY == -1 || map[safeC(y + a)][safeC(x + b)].elevation > map[maxY][maxX].elevation) {
								maxY = safeC(y + a);
								maxX = safeC(x + b);
							}
						}
					}
				}
				if (map[maxY][maxX].elevation > 0) {
					if (maxX > x) {
						bannedDir = 3;
					}
					if (maxY > y) {
						bannedDir = 2;
					}
					if (bannedDir == -1 || rand() % 2 == 0) {
						if (maxX < x) {
							bannedDir = 1;
						}
						if (maxY < y) {
							bannedDir = 0;
						}
					}
				}
				y = maxY;
				x = maxX;
			}
			else {
				do {
					dir = rand() % 4;
				} while (dir == lastDir || dir == bannedDir);
				switch (dir) {
				case 0: y++; break;
				case 1: x++; break;
				case 2: y--; break;
				case 3: x--; break;
				}
				if (dir < 2) { lastDir = dir + 2; }
				else { lastDir = dir - 2; }
			}
		}
	};
	int count = 0;
	float avgLen = 0;
	for (int i = id; i < MAPSIZE; i += NUM_THREADS) {
		for (int j = id; j < MAPSIZE; j += NUM_THREADS) {
			switch (map[i][j].type) {
			case 'w': if (fir) {
				makeCurrent(i, j);
				makeCurrent(i, j);
			} break;
			case 'a': case 'r': if (!fir) { makeCurrent(i, j); } break;
			}
		}
	}
}

void rivers(std::vector< std::vector<MapTile >>& map) {
	auto makeRiver = [&](int yC, int xC) {
		std::vector<Spot> open = {Spot(xC, yC)};
		std::vector<Spot> closed = {};
		Spot current;
		int rS = 0;
		int min = 0;
		int MAX_SIZE = 1000;
		bool found;
		bool stop = false;
		while (open.size() > 0 && rS++ < MAX_SIZE && !stop) {
			min = 0;
			for (int i = 1; i < open.size(); i++) {
				if (open[i].g + open[i].h < open[min].g + open[min].h) {
					min = i;
				}
			}
			current = open[min];
			open.erase(open.begin() + min);

			for (int a = -1; a < 2; a++) {
				for (int b = -1; b < 2; b++) {
					if (a != b && (a == 0 || b == 0)) {
						if (map[safeC(current.y + a)][safeC(current.x + b)].elevation <= 0) {
							stop = true;
						}
					}
				}
			}

			if (!stop) {
				for (int a = -1; a < 2; a++) {
					for (int b = -1; b < 2; b++) {
						if (a != b && (a == 0 || b == 0)) {
							found = false;
							Spot consider = Spot(safeC(current.x + b), safeC(current.y + a), closed.size());
							consider.h = map[consider.y][consider.x].elevation;

							if (consider.h < 40 || rand() % 4 == 0) {
								for (Spot s : closed) {
									if (s == consider) {
										found = true;
										break;
									}
								}
								if (!found) {
									for (Spot s : open) {
										if (s == consider) {
											found = true;
											break;
										}
									}
									if (!found) {

										consider.g = (consider.h + current.g) / 5.0;
										open.push_back(consider);
									}
								}
							}
						}
					}
				}

				closed.push_back(current);
			}
		}
		if (stop && rS > 15) {
			while (current.parent != -1) {
				map[current.y][current.x].type = 'r';
				map[current.y][current.x].elevation = -100;
				current = closed[current.parent];
			} 
			map[current.y][current.x].elevation = -100;
			map[current.y][current.x].type = 'r';
		}
	};
	for (int i = 0; i < MAPSIZE; i += 1) {
		//std::cout << i << std::endl;
		for (int j = 0; j < MAPSIZE; j += 1) {
			if (map[i][j].elevation > 10 && map[i][j].elevation < 65) {
				int chance = 950;
				if ((rand() % 1000) > chance) {
					makeRiver(i, j);
				}
			}
		}
	}
}

void lakes(std::vector< std::vector<MapTile >>& map) {
	int id = 0;
	auto makeLake = [&](int yC, int xC) {
		std::vector<Spot> open = { Spot(xC, yC) };
		open[0].h = map[yC][xC].elevation;
		std::vector<Spot> closed = {};
		Spot current;
		int rS = 0;
		int min = 0;
		int MAX_SIZE = 200;
		bool found;
		bool stop = false;

		while (closed.size() < MAX_SIZE && open.size() > 0 && !stop) {
			min = 0;
			for (int i = 1; i < open.size(); i++) {
				if (open[i].g + open[i].h < open[min].g + open[min].h) {
					min = i;
				}
			}
			current = open[min];
			open.erase(open.begin() + min);

			for (int a = -1; a < 2; a++) {
				for (int b = -1; b < 2; b++) {
					if (a != b && (a == 0 || b == 0)) {
						if (map[safeC(current.y + a)][safeC(current.x + b)].type == 'w') {
							stop = true;
						}
					}
				}
			}

			if (!stop) {
				for (int a = -1; a < 2; a++) {
					for (int b = -1; b < 2; b++) {
						if (a != b && (a == 0 || b == 0)) {
							found = false;
							Spot consider = Spot(safeC(current.x + b), safeC(current.y + a), closed.size());
							consider.h = map[consider.y][consider.x].elevation;

							if (consider.h < current.h * .97) {
								for (Spot s : closed) {
									if (s == consider) {
										found = true;
										break;
									}
								}
								if (!found) {
									for (Spot s : open) {
										if (s == consider) {
											found = true;
											break;
										}
									}
									if (!found) {
										consider.g = (consider.h + current.g);
										open.push_back(consider);
									}
								}
							}
						}
					}
				}
				closed.push_back(current);
			}
		}
		if (closed.size() < MAX_SIZE && closed.size() > 20 && open.size() == 0 && !stop) {
			for (Spot c : closed) {
				map[c.y][c.x].type = 'a';
				map[c.y][c.x].elevation = -100;
			}
		}
	};
	for (int i = 0; i < MAPSIZE; i++) {
		for (int j = 0; j < MAPSIZE; j++) {
			if (map[i][j].elevation > 15 && map[i][j].elevation < 55) {
				int chance = 900;
				if ((rand() % 1000) > chance) {
					makeLake(i, j);
				}
			}
		}
	}
}

void slap(std::vector< std::vector<MapTile >>& map, int minSize, int maxSize, char typeDetect, char typeMake, double elevChange, int nind) {
	int ind = 0;
	int counter = 0;
	bool checking = true;
	std::vector<std::vector<int>> stack;
	auto checkValidity = [&](int y, int x, bool checking) {
		MapTile a = map[safeC(y)][safeC(x)];
		//std::cout << a.checked << std::endl;
		return (a.checked != nind || !checking) && a.type == typeDetect;
	};
	auto slapInner = [&](int yC, int xC, bool checking) {
		stack.push_back({ yC, xC });
		while (stack.size() > 0) {
			counter++;
			int y = safeC(stack.back()[0]);
			int x = safeC(stack.back()[1]);

			stack.pop_back();

			if (counter % 100 == 0) {
				//std::cout << typeDetect << std::endl;
			}
			if (checking) { map[y][x].checked = nind; }
			else {
				map[y][x].type = typeMake;
				if (elevChange != 2) {
					map[y][x].elevation = elevChange;
				}
			}

			if (checkValidity(y + 1, x, checking)) {
				stack.push_back({ y + 1, x });
			}
			if (checkValidity(y, x + 1, checking)) {
				stack.push_back({ y, x + 1 });
			}
			if (checkValidity(y - 1, x, checking)) {
				stack.push_back({ y - 1, x });
			}
			if (checkValidity(y, x - 1, checking)) {
				stack.push_back({ y, x - 1 });
			}
		}
	};
	int i = MAPSIZE;
	int j = MAPSIZE;
	while (--i >= 0) {

		while (--j >= 0) {

			if (checkValidity(i, j, true)) {
				counter = 0;
				slapInner(i, j, true);
				if (counter <= maxSize && counter >= minSize) {
					slapInner(i, j, false);
				}
			}
		}
		j = MAPSIZE;
	}
}

void thiccify_forests(std::vector< std::vector<MapTile >>& map) {
	int treeChance = 0;
	for (int m = 4; m > 0; m--) {
		for (int h = 1; h >= 0; h--) {
			int i = h * (MAPSIZE - 1);
			while (i < MAPSIZE && i >= 0) {
				int j = h * (MAPSIZE - 1);
				while (j < MAPSIZE && j >= 0) {
					if (map[i][j].elevation > 0 && map[i][j].forest == 'e' && map[i][j].elevation < 75) {
						treeChance = 0;
						for (int a = -1; a < 2; a++) {
							for (int b = -1; b < 2; b++) {
								if (a * b == 0 && a != b) {
									if (map[safeC(i + a)][safeC(j + b)].type == 'a' || map[safeC(i + a)][safeC(j + b)].type == 'r') {
										treeChance += 3;
										map[i][j].forestChance += 4;
									}
									if (map[safeC(i + a)][safeC(j + b)].forest != 'e') {
										treeChance += 3;
									}
								}
							}
						}
						if (map[i][j].elevation >= 50) {
							treeChance -= (map[i][j].elevation - 50);
						}
						if (rand() % 100 < treeChance) {
							if (map[i][j].type == 'm') {
								map[i][j].forest = 'j';
							}
							else if (map[i][j].type == '5' || map[i][j].type == '4' || map[i][j].type == 'p' || map[i][j].type == 'c' || map[i][j].type == '3') {
								if (map[i][j].elevation > 30) {
									map[i][j].forest = 'g';
								}
								else {
									map[i][j].forest = 't';
								}
							}
							else if (map[i][j].type == '1' || map[i][j].type == '2') {
								map[i][j].forest = 'f';
								if (map[i][j].elevation > 60) {
									map[i][j].forest = 't';
								}
							}
							if (rand() % 100 <= 1) {
								map[i][j].forest = 's';
							}
						}
					}
					if (h == 0) { j++; }
					else { j--; }
				}
				if (h == 0) { i++; }
				else { i--; }
			}
		}
	}
}

std::vector<std::vector<int>> nodes;
void finalize(std::vector< std::vector<MapTile >>& map) {
	MapTile t;
	bool stop = false;
	for (int i = 0; i < MAPSIZE; i++) {
		for (int j = 0; j < MAPSIZE; j++) {
			t = map[i][j];
			if (t.elevation > 0) {
				//Cold Biomes
				if (map[i][j].heat < 25) {
					if (map[i][j].rain < 225) {
						map[i][j].type = 'c';
					}
					else if (map[i][j].rain < 250) {
						map[i][j].type = 'b';
					}
					else if (map[i][j].rain < 500) {
						map[i][j].type = '1' + (5 - (map[i][j].rain - 250) / 50);
					}
					else {
						map[i][j].type = 'm';
					}
				}
				else if (map[i][j].heat < 60) {
					if (map[i][j].rain < 200) {
						map[i][j].type = 'd';
					}
					else if (map[i][j].rain < 225) {
						map[i][j].type = 'b';
					}
					else if (map[i][j].rain < 475) {
						map[i][j].type = '1' + (5 - (map[i][j].rain - 225) / 50);
					}
					else {
						map[i][j].type = 'm';
					}
				}
				else {
					if (map[i][j].rain < 175) {
						map[i][j].type = 'd';
					}
					else if (map[i][j].rain < 200) {
						map[i][j].type = 'b';
					}
					else if (map[i][j].rain < 325) {
						map[i][j].type = '1' + (5 - (map[i][j].rain - 200) / 25);
					}
					else {
						map[i][j].type = 'm';
					}
				}
				MapTile t = map[i][j];
				int treeChance = -100 + t.forestChance * max(.5, min(1.25, min(200.0 + pow(latTemp(i) / 50, 7), map[i][j].rain) / (200.0 + t.elevation / 2 + t.heat / 4)));
				map[i][j].forestChance = (t.rain / 4 + t.forestChance / 2 + t.heat / 8) * max(.1, fmin(300.0, map[i][j].rain) / (275.0 + t.elevation));
				if (t.rain > 300) {
					map[i][j].forestChance *= .9;
				}
				if (treeChance > 0) {
					treeChance = 95;
				}
				if (map[i][j].rain > 195) {
					treeChance = max(treeChance, 4);
				}
				if (rand() % 100 < treeChance) {
					if (map[i][j].type == 'm') {
						map[i][j].forest = 'j';
					}
					else if (map[i][j].type == '5' || map[i][j].type == '4' || map[i][j].type == 'p' || map[i][j].type == 'c' || map[i][j].type == '3') {
						if (map[i][j].elevation > 30) {
							map[i][j].forest = 'g';
						}
						else {
							map[i][j].forest = 't';
						}
					}
					else if (map[i][j].type == '1' || map[i][j].type == '2') {
						map[i][j].forest = 'f';
						if (map[i][j].elevation > 60) {
							map[i][j].forest = 't';
						}
					}
				}
			}
			else if (map[i][j].type == 'w') {
				bool stop = false;
				for (int a = -5; a <= 5; a++) {
					for (int b = -5; b <= 5; b++) {
						if (a != 0 || b != 0) {
							if (map[safeC(i + a)][safeC(j + b)].elevation > 0) {
								if (fDist(i, j, safeC(i + a), safeC(j + b)) <= 4) {
									stop = true;
									break;
								}
							}
						}
					}
				}
				if (!stop) {
					map[i][j].type = 'e';
				}
			}
		}
	}
}

std::vector<std::vector<Tile>> clearMap(Tile t) {
	std::vector<std::vector<Tile>> map;
	map.resize(MAPSIZE);
	for (int i = 0; i < MAPSIZE; i++) {
		map[i].resize(MAPSIZE);
	}
	for (int i = 0; i < MAPSIZE; i++) {
		for (int j = 0; j < MAPSIZE; j++) {
			map[i][j] = t;
		}
	}
	return map;
}

void rainSpread(std::vector< std::vector<MapTile>>& map) {
	for (int m = 4; m > 0; m--) {
		for (int h = 1; h >= 0; h--) {
			int i = h * (MAPSIZE - 1);
			while (i < MAPSIZE && i >= 0) {
				int j = h * (MAPSIZE - 1);
				while (j < MAPSIZE && j >= 0) {
					float avg = 0;
					float avgH = 0;
					int num = 0;
					if (map[i][j].elevation > 0) {
						for (int a = -1; a < 2; a++) {
							for (int b = -1; b < 2; b++) {
								if (map[safeC(i + a)][safeC(j + b)].elevation > 0) {
									avg += map[safeC(i + a)][safeC(j + b)].rain;
									avgH += map[safeC(i + a)][safeC(j + b)].heat;
									num++;
								}
								else if (map[safeC(i + a)][safeC(j + b)].type == 'a' || map[safeC(i + a)][safeC(j + b)].type == 'r') {
									map[i][j].rain += 15;
								}
								else if (map[safeC(i + a)][safeC(j + b)].type == 'w') {
									map[i][j].rain += 3;
								}
							}
						}
						avg /= num;
						avgH /= num;
						map[i][j].rain = (map[i][j].rain + avg) / 2.0;
						map[i][j].heat = (map[i][j].heat * 4 + avgH) / 5.0;
					}
					if (h == 0) { j++; }
					else { j--; }
				}
				if (h == 0) { i++; }
				else { i--; }
			}
		}
	}
}




std::vector<std::vector<Tile>> createMap(std::vector<std::vector<MapTile>>& base) {
	srand(time(NULL));
	std::vector<std::vector<MapTile>> map;
	map.resize(MAPSIZE);
	for (int i = 0; i < MAPSIZE; i++) {
		map[i].resize(MAPSIZE);
	}
	//Create The Oceans and the Land
	elev(map, MODI * MAPSIZE / 10 * MAPSIZE / 10 * MAPSIZE / 10, 40 * MODI, 40);
	elev(map, MODI * MAPSIZE / 10 * MAPSIZE / 10 * MAPSIZE / 10, 28 * MODI, -100);
	slap(map, 0, 400, 'g', 'w', -100, 0);
	slap(map, 500, 9000, 'g', 'w', -100, 1);
	std::cout << "Oceans Generated" << std::endl;

	for (int i = 0; i < MAPSIZE; i++) {
		for (int j = 0; j < MAPSIZE; j++) {
			map[i][j].heat = 25 + latTemp(i)/2;
			map[i][j].rain = latTemp(i);
			if (map[i][j].type == 'g') {
				map[i][j].elevation += (rand() % 20);
			}
			map[i][j].forestChance = 50 + rand() % 50;
			if (rand() % 10 == 0) {
				map[i][j].forestChance = 500;
			}
			if (rand() % 10 == 0) {
				map[i][j].forestChance = -100;
			}
		}
	}

	//Spread Elevation Across the Land
	elev(map, (int)(400.0 * MODI), (int)round(200.0 * MODI), 100);
	elev(map, (int)(1600.0 * MODI), (int)round(80.0 * MODI), 100);
	elev(map, (int)(2000.0 * MODI), (int)round(140.0 * MODI), 100);

	elevSpread(map);
	elevSpread(map);
	elevSpread(map);
	elevSpread(map);
	elevSpread(map);

	for (int i = 0; i < MAPSIZE; i++) {
		for (int j = 0; j < MAPSIZE; j++) {
			map[i][j].rain += (100 - map[i][j].elevation);
			if (map[i][j].heat > map[i][j].elevation / 4) {
				map[i][j].heat -= (map[i][j].elevation / 4);
			}
		}
	}

	//Spread Currents Across the Land

	//Create Rivers and Lakes

	//2nd Current Pass

	//Biomes and Forests
	forestSpread(map);
	//std::cout << "Elevation Generated" << std::endl;
	std::thread ape0(currents, std::ref(map), true, 0);
	std::thread ape1(currents, std::ref(map), true, 1);
	std::thread ape2(currents, std::ref(map), true, 2);
	std::thread ape3(currents, std::ref(map), true, 3);

	ape0.join();
	ape1.join();
	ape2.join();
	ape3.join();

	std::cout << "Currents Generated" << std::endl;
	lakes(map);
	//rivers(map);
	//std::cout << "Rivers/Lakes Generated" << std::endl;
	//elevSpread(map);
	//elevSpread(map);
	//elevSpread(map);
	std::thread ape4(currents, std::ref(map), false, 0);
	std::thread ape5(currents, std::ref(map), false, 1);
	std::thread ape6(currents, std::ref(map), false, 2);
	std::thread ape7(currents, std::ref(map), false, 3);

	ape4.join();
	ape5.join();
	ape6.join();
	ape7.join();
	std::cout << "2nd Currents Generated" << std::endl;
	rainSpread(map);
	rainSpread(map);
	rainSpread(map);
	finalize(map);
	thiccify_forests(map);

	base = map;

	std::vector<std::vector<Tile>> gameMap;
	gameMap.resize(MAPSIZE);
	for (int i = 0; i < MAPSIZE; i++) {
		gameMap[i].resize(MAPSIZE);
	}
	for (int i = 0; i < MAPSIZE; i++) {
		for (int j = 0; j < MAPSIZE; j++) {
			if (map[i][j].elevation < 1 && !(map[i][j].type == 'w' || map[i][j].type == 'a' || map[i][j].type == 'r')) {
				map[i][j].elevation = 1;
			}

			gameMap[i][j] = Terrain_Convert(map[i][j]);
		}
	}
	return gameMap;
}