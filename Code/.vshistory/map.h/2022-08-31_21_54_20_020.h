#pragma once

void elev(std::vector< std::vector<MapTile>>& map, int size, int num, int elev) {
	int yC = (int)floor((((double)rand() / RAND_MAX)) * MAPSIZE);
	int xC = (int)floor((((double)rand() / RAND_MAX)) * MAPSIZE);
	int rS = 0;
	int rN = 0;
	int dir = 0;
	int lastDir = -1;
	while (rN < num) {
		if (rS == size || rN == 0) {
			rS = 0;
			rN++;
			yC = (int)floor((((double)rand() / RAND_MAX)) * MAPSIZE);
			xC = (int)floor((((double)rand() / RAND_MAX)) * MAPSIZE);
		}
		do {
			dir = (int)floor((((double)rand() / RAND_MAX)) * 8);
		} while (dir == lastDir);
		switch (dir) {
		case 0: yC--; xC--; break;
		case 1: yC--; break;
		case 2: yC--; xC++; break;
		case 3: xC++; break;
		case 4: yC++; xC++; break;
		case 5: yC++; break;
		case 6: yC++; xC--; break;
		case 7: xC--; break;
		}
		yC = safeC(yC);
		xC = safeC(xC);
		if (map[yC][xC].elevation > 0) {
			map[yC][xC].elevation = elev;
			if (elev < 0) {
				map[yC][xC].type = 'w';
			}
			if (dir < 4) {
				lastDir = dir + 4;
			}
			else {
				lastDir = dir - 4;
			}
		}
		else { rS = size - 1; }
		rS++;
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
						if (Min < 20) {
							map[i][j].elevation = rand() % 5 + (Max + map[i][j].elevation + Min) / 3;
						}
						else if (map[i][j].elevation < Max) {
							//map[i][j].elevation = rand() % 3 + ((max/2+avg/2) * (.96));
							if (avg >= 50) {
								map[i][j].elevation = (rand() % 3 + ((Max / 2 + avg / 2) * (.91))) * (100 + pow(MODI, 2)) / 100.0;
							}
							else {
								map[i][j].elevation = rand() % 3 + (Max * (.75 + ((((double)rand() / RAND_MAX)) * .2))) * (100 + 2 * MODI) / 100.0;
							}
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

void currents(std::vector<std::vector<MapTile>>& map, bool fir, int dry_air, int id) {
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
								temp += .01;
								t->rain += .02;
								t->forestChance += .01;
							}
							else if (t->heat < temp) {  //Hot current over cold land. increase land temp
								t->heat += .04 + temp / 1000;
								temp -= .005;
								water -= .01;
							}

							float rainfall = 0;
							if (bannedDir == dry_air) { //Dry Air Current
								rainfall = -.2;
								water -= .01;
								t->heat += .04;
								t->forestChance -= .025;
							}
							else if (t->rain < 195) {
								rainfall = 2;
							}
							else if (t->rain < (250 + latTemp(yC))) {
								rainfall = .2;
							}
							else {
								rainfall = .1;
							}
							if (!fir) {
								rainfall = fmax(.1, 2 * rainfall);
							}
							t->rain += rainfall;
							water -= t->heat / 8000.0;
							if (t->heat > 2) {
								t->heat *= 1 - t->elevation / 40000.0;
							}
							if (t->elevation >= 55) { //Make mountains colder, and lose water when traversing mountains
								water -= (t->elevation / (700.0));
								if (t->heat > 2) {
									t->heat *= 1 - t->elevation / 80000.0;
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
	int id = 0;
	int temp = 0;
	auto makeRiver = [&](int yC, int xC) {
		int interval = 80.0 * MODI + 1;
		std::vector<std::vector<int>> stack;
		int stop = false;
		stack.push_back({ yC, xC });
		int lastDir = -1;
		int lastlast = -1;
		int bannedDir = -1;
		int rS = 0;
		std::deque<std::deque<int>> river;
		int minDir = -1;
		int y = -1; int x = -1;
		int min = 100;
		while (stack.size() > 0 && !stop) {
			y = safeC(stack.back()[0]);
			x = safeC(stack.back()[1]);
			stack.pop_back();
			river.push_back({ y, x, rS });
			if (map[y][x].elevation > 0) {
				min = map[y][x].elevation + 5;
				map[y][x].id[0] = id;
				map[y][x].id[1] = rS;
				minDir = -1;
				if (map[safeC(y - 1)][x].elevation <= min && lastDir != 0 && lastlast != 0 && bannedDir != 0 && map[safeC(y - 1)][x].id[0] != id) {
					min = map[safeC(y - 1)][x].elevation;
					minDir = 0;
				}
				if (map[y][safeC(1 + x)].elevation <= min && lastDir != 1 && lastlast != 1 && bannedDir != 1 && map[y][safeC(1 + x)].id[0] != id) {
					min = map[y][safeC(1 + x)].elevation;
					minDir = 1;
				}
				if (map[safeC(y + 1)][x].elevation <= min && lastDir != 2 && lastlast != 2 && bannedDir != 2 && map[safeC(y + 1)][x].id[0] != id) {
					min = map[safeC(y + 1)][x].elevation;

					minDir = 2;
				}
				if (map[y][safeC(x - 1)].elevation <= min && lastDir != 3 && lastlast != 3 && bannedDir != 3 && map[y][safeC(x - 1)].id[0] != id) {
					min = map[y][safeC(x - 1)].elevation;
					minDir = 3;
				}
				if (minDir > -1) {
					lastlast = lastDir;
					if (minDir == 0) { stack.push_back({ y - 1, x }); }
					else if (minDir == 1) { stack.push_back({ y, x + 1 }); }
					else if (minDir == 2) { stack.push_back({ y + 1, x }); }
					else if (minDir == 3) { stack.push_back({ y, x - 1 }); }
					if (minDir < 2) { lastDir = minDir + 2; }
					else { lastDir = minDir - 2; }

					if (rS % interval == 0) {
						bannedDir = rand() % 4;
					}
				}
			}
			else { stop = true; }
			rS++;
		}
		if (stop && rS > 3) {
			std::vector<std::vector<int>> keep;
			int y = safeC(river.front()[0]);
			int x = safeC(river.front()[1]);
			river.pop_front();
			int cur = 0;
			int dir = -1;
			keep.push_back({ y, x });
			while (cur < rS) {
				dir = -1;
				if ((map[safeC(y - 1)][x].id[0] > -1 && map[safeC(y - 1)][x].id[0] < map[safeC(y - 1)][x].id[0]) || (map[safeC(y - 1)][x].id[0] == id && map[safeC(y - 1)][x].id[1] >= cur)) {
					dir = 0;
					cur = map[safeC(y - 1)][x].id[1];
				}
				if ((map[y][safeC(x + 1)].id[0] > -1 && map[y][safeC(x + 1)].id[0] < map[y][safeC(x + 1)].id[0]) || (map[y][safeC(x + 1)].id[0] == id && map[y][safeC(x + 1)].id[1] >= cur)) {
					dir = 1;
					cur = map[y][safeC(x + 1)].id[1];
				}
				if ((map[safeC(y + 1)][x].id[0] > -1 && map[safeC(y + 1)][x].id[0] < map[safeC(y + 1)][x].id[0]) || (map[safeC(y + 1)][x].id[0] == id && map[safeC(y + 1)][x].id[1] >= cur)) {
					dir = 2;
					cur = map[safeC(y + 1)][x].id[1];
				}
				if ((map[y][safeC(x - 1)].id[0] > -1 && map[y][safeC(x - 1)].id[0] < map[y][safeC(x - 1)].id[0]) || (map[y][safeC(x - 1)].id[0] == id && map[y][safeC(x - 1)].id[1] >= cur)) {
					dir = 3;
					cur = map[y][safeC(x - 1)].id[1];
				}
				switch (dir) {
				case 0: keep.push_back({ y - 1, x }); y = safeC(y - 1); break;
				case 1: keep.push_back({ y, x + 1 }); x = safeC(x + 1); break;
				case 2: keep.push_back({ y + 1, x }); y = safeC(y + 1); break;
				case 3: keep.push_back({ y, x - 1 }); x = safeC(x - 1); break;
				}
				cur++;
			}
			int kepL = keep.size();
			for (int a = 0; a < kepL; a++) {
				int y = safeC(keep[a][0]);
				int x = safeC(keep[a][1]);
				map[y][x].type = 'r';
				map[y][x].elevation = -100;
				map[y][x].rain = 260;
				map[y][x].id[2] = map[y][x].id[0];
			}
		}
	};
	for (int i = 0; i < MAPSIZE; i++) {
		//std::cout << i << std::endl;
		for (int j = 0; j < MAPSIZE; j++) {
			if (map[i][j].elevation > 25 && map[i][j].elevation < 75) {
				int chance = 990 - min(map[i][j].rain / 15, 20);
				if ((rand() % 1000) > chance) {
					makeRiver(i, j);
					id++;
				}
			}
		}
	}

}
void lakes(std::vector< std::vector<MapTile >>& map) {
	int id = 0;
	auto makeLake = [&](int yC, int xC) {
		std::deque<std::deque<int>> stack;
		int stop = false;
		stack.push_back({ yC, xC });
		double min = 0;
		while (stack.size() > 0) {
			int y = safeC(stack.front()[0]);
			int x = safeC(stack.front()[1]);
			stack.pop_front();
			if (map[y][x].elevation > 0) {
				min = map[y][x].elevation + 1;
				if (min > 15) {
					map[y][x].type = 'a';
					map[y][x].rain = 260;
					map[y][x].id[0] = id;
					map[y][x].id[2] = id;
					map[y][x].elevation = -100;
					if (map[safeC(y - 1)][x].elevation < min && map[safeC(y - 1)][x].id[0] != id) {
						stack.push_back({ y - 1, x });
						map[safeC(y - 1)][x].id[0] = id;
					}
					if (map[y][safeC(1 + x)].elevation < min && map[y][safeC(1 + x)].id[0] != id) {
						stack.push_back({ y, x + 1 });
						map[y][safeC(x + 1)].id[0] = id;
					}
					if (map[safeC(y + 1)][x].elevation < min && map[safeC(y + 1)][x].id[0] != id) {
						stack.push_back({ y + 1, x });
						map[safeC(y + 1)][x].id[0] = id;
					}
					if (map[y][safeC(x - 1)].elevation < min && map[y][safeC(x - 1)].id[0] != id) {
						stack.push_back({ y, x - 1 });
						map[y][safeC(x - 1)].id[0] = id;
					}
				}
			}
		}
	};
	for (int i = 0; i < MAPSIZE; i++) {
		for (int j = 0; j < MAPSIZE; j++) {
			if (map[i][j].elevation > 35 && map[i][j].elevation < 40) {
				double chance = 1.002 - fmin(map[i][j].rain / 60000, .005);
				if (((double)rand() / RAND_MAX) > chance) {
					makeLake(i, j);
					id++;
				}
			}
		}
	}
}
std::vector<int> spill(std::vector< std::vector<MapTile >>& map, int y, int x, int r) { //finds distance to water currently
	std::vector<std::vector<int>> stack;
	stack.push_back({ y, x });
	int stop = false;
	int y1;
	int x1;
	int counter = 0;

	while (stack.size() > 0 && stack.size() < 20 * MAPSIZE) {
		counter++;
		y1 = safeC(stack.front()[0]);
		x1 = safeC(stack.front()[1]);
		//std::cout << x1 << std::endl;
		stack.pop_back();
		map[y1][x1].it = r;
		if (!stop) {
			for (int i = -1; i < 2; i++) {
				for (int j = -1; j < 2; j++) {
					if ((i == 0) != (j == 0)) {
						if (map[safeC(y1 + i)][safeC(x1 + j)].it != r) {
							if (map[safeC(y1 + i)][safeC(x1 + j)].type == 'w' || map[safeC(y1 + i)][safeC(x1 + j)].type == 'a') {
								stop = true;
								i = 2;
								j = 2;
							}
							stack.push_back({ y1 + i, x1 + j });
						}
					}
				}
			}
			if (stop) {
				int yDist = stack.back()[0] - y;
				int xDist = stack.back()[1] - x;
				if (abs(yDist) > MAPSIZE) {
					int modifier = yDist / abs(yDist);
					yDist = safeC(yDist) * modifier;
				}
				if (abs(xDist) > MAPSIZE) {
					int modifier = xDist / abs(xDist);
					xDist = safeC(xDist) * modifier;
				}
				return { yDist, xDist };
			}
		}
	}

	return { -1, -1 };
}

std::vector<std::vector<Tile>> createMap(std::vector<std::vector<MapTile>> m = {}) {
	//Create The Oceans and the Land
	elev(map, MODI * MAPSIZE / 10 * MAPSIZE / 10 * MAPSIZE / 10, 28 * MODI);
	slap(map, 0, 400, 'g', 'w', -100, 0);
	slap(map, 500, 9000, 'g', 'w', -100, 1);
	//Spread Elevation Across the Land
	elev(map, 20, (int)round(40.0 * MODI), 90);
	elev(map, 10, (int)round(80.0 * MODI), 90);
	elev(map, (int)(40.0 * MODI), (int)round(20.0 * MODI), 90);
	elev(map, (int)(320.0 * MODI), (int)round(20.0 * MODI), 90);
	elev(map, (int)(400.0 * MODI), (int)round(35.0 * MODI), 90);
	elev(map, 200, (int)round(20.0 * MODI), 60);
	elev(map, 100, (int)round(150.0 * MODI), 60);
	elev(map, 60, (int)round(150.0 * MODI), 60);
	elev(map, 35, (int)round(150.0 * MODI), 60);
	elev(map, 20, (int)round(150.0 * MODI), 60);
	elevSpread(map);
	//Spread Currents Across the Land

	//Create Rivers and Lakes

	//2nd Current Pass

	//Biomes and Forests

}