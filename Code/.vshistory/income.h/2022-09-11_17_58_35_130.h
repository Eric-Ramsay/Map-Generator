#pragma once

int tilePower(Player& p) {
	float total = 0;
	float mult = 2.5;
	Tile t;
	Tile t1;
	for (int i = p.land.size() - 1; i >= 0; i--) {
		t = map[p.land[i].y][p.land[i].x];
		if (t.owner != p.turn) {
			p.land.erase(p.land.begin() + i);
		}
		else {
			mult = 2.5;
			for (int a = -1; a < 2; a++) {
				for (int b = -1; b < 2; b++) {
					if (a * b == 0 && a != b) {
						t1 = map[safeC(p.land[i].y + a)][safeC(p.land[i].x + b)];
						if (t1.owner == t.owner) {
							mult -= .5;
						}
					}
				}
			}
			total += tileCost(t) * mult;
		}
	}
	return (int)total;
}

float growthRate(float pop, float pMax) {
	return .06 * min(2, pMax / max(1, pop));
}

void BuildingIncome(int x, int y, float (&income)[HORSES + 1], float (&m)[HORSES + 1]) {
	int type = map[y][x].building.type;

	//HOUSE, FARM, WATER_NET, MILL, LUMBER_CAMP, MINE

	if (type == HOUSE) {
		m[WORKERS] += 6;
		m[SPECIALISTS] += 2;
	}
	else if (type == FARM) {
		income[FOOD] += max(0, 8 - map[y][x].type);
		for (int a = -1; a < 2; a++) {
			for (int b = -1; b < 2; b++) {
				if (map[safeC(y + a)][safeC(x + b)].building.type == MILL) {
					income[FOOD] += max(0, 8 - map[y][x].type);
				}
				if (map[safeC(y + a)][safeC(x + b)].building.type == FARM) {
					income[FOOD]++;
				}
			}
		}
	}
	else if (type == WATER_NET) {
		income[WATER] += 10;
	}
	else if (type == MILL) {
		m[FOOD] += 250;
	}
	else if (type == LUMBER_CAMP) {
		m[WOOD] += 3;
		int num_trees = 0;
		for (int a = -1; a < 2; a++) {
			for (int b = -1; b < 2; b++) {
				if (map[safeC(y + a)][safeC(x + b)].forest != NONE) {
					num_trees++;
					if (num_trees == 3) {
						a = 2; b = 2;
					}
				}
			}
		}
		income[WOOD] += num_trees / 3.0;
	}
	else if (type == MINE) {
		income[STONE] += 1;
		m[STONE] += 3;
	}
	else if (type == STOREHOUSE) {
		m[STONE] += 30;
		m[WOOD] += 30;
		m[FOOD] += 500;
		m[WATER] += 150;
	}
	else if (type == WELL) {
		m[WATER] += 500;
		income[WATER] += 10;
	}

}


void Income(Player& p) {
	if (p.land.size() == 0) {
		return;
	}
	std::vector<C> new_land = {};

	//Gatherer, Worker, and Specialist Income
	float gIncome[HORSES + 1];
	float wIncome[HORSES + 1];
	float sIncome[HORSES + 1];
	float wMax[HORSES + 1];
	float sMax[HORSES + 1];
	for (int i = 0; i < HORSES + 1) {
		gIncome[i] = 0;
		wIncome[i] = 0;
		sIncome[i] = 0;
		wMax[i] = 0;
		sMax[i] = 0;
	}
	float base_max[HORSES + 1] = { 100, 25, 25, -1, 50, -1, -1, 0, -1, 1000, 1000, 50, 50, 50, 50, 50, 10, 10 };
	
	//Reset Jobs to recalculate unemployment and what not
	p.resources[G_JOBS] = 0;
	p.resources[W_JOBS] = 0;
	p.resources[S_JOBS] = 0;

	for (int i = 0; i < 2; i++) {
		for (C c : p.land) {
			if (i == 0) {
				if (map[c.y][c.x].owner == p.turn) {
					new_land.push_back(c);
				}
				if (map[c.y][c.x].building.type != -1) {
					if (buildings[map[c.y][c.x].building.type].specialists > 0) {
						BuildingIncome(c.x, c.y, sIncome, sMax);
					}
					else {
						BuildingIncome(c.x, c.y, wIncome, wMax);
					}
					wIncome[W_JOBS] += buildings[map[c.y][c.x].building.type].workers;
					sIncome[S_jobs] += buildings[map[c.y][c.x].building.type].specialists
				}
			}
			else {
				if (map[c.y][c.x].building.type == -1) {
					if (map[c.y][c.x].forest != NONE) {
						if (map[c.y][c.x].forest == FORAGE) {
							gIncome[FOOD] += 15;
							gIncome[WATER] += 5;
						}
						else {
							gIncome[WOOD] += .03;
							gIncome[FOOD] += .1;
						}
					}
					else {
						gIncome[STONE] += .01 * map[c.y][c.x].elev;
						switch (map[c.y][c.x].type) {
						case LAKE: case RIVER: gIncome[WATER] += 1; gIncome[FOOD] += .25; break;
						case OCEAN: case SHALLOW: gIncome[FOOD] += .25;
						case MEADOW: gIncome[WATER] += .4; gIncome[FOOD] += .5; break;
						case GRASS_1: gIncome[WATER] += .4; gIncome[FOOD] += .5; break;
						case GRASS_2: gIncome[WATER] += .4; gIncome[FOOD] += .4; break;
						case GRASS_3: gIncome[WATER] += .3; gIncome[FOOD] += .3; break;
						case GRASS_4: gIncome[WATER] += .2; gIncome[FOOD] += .2; break;
						case GRASS_5: gIncome[WATER] += .2; gIncome[FOOD] += .1; break;
						default: gIncome[STONE] += .025; break;
						}
					}
				}
			}
		}
		if (i == 1) {
			gIncome[G_JOBS] = (gIncome[WATER] + gIncome[FOOD] + gIncome[WOOD] * 3) / 3.0;
		}
	}

	for (C c : p.land) {
		
	}

	//Reductions from pop management. IE - food/water, births, deaths
	income[FOOD] -= (p.resources[WORKERS] + p.resources[SPECIALISTS]) * 2;
	income[WATER] -= (p.resources[WORKERS] + p.resources[SPECIALISTS]) * 2;

	float deathRate = .066;
	
	if (p.resources[FOOD] > 0 && p.resources[WATER] > 0) {
		income[WORKERS] = p.resources[WORKERS] * growthRate(p.resources[WORKERS], p.max[WORKERS]);
		income[SPECIALISTS] = p.resources[SPECIALISTS] * (growthRate(p.resources[SPECIALISTS], p.max[SPECIALISTS])/2);

		income[WELLBEING] += .1;
	}
	else {
		if (p.resources[FOOD] <= 0) {
			deathRate *= 1.2;
			income[WELLBEING] -= 1;
		}
		if (p.resources[WATER] <= 0) {
			deathRate *= 1.2;
			income[WELLBEING] -= 1;
		}
	}

	income[WORKERS] +=  p.resources[WORKERS] * -deathRate;
	income[SPECIALISTS] += p.resources[SPECIALISTS] * -deathRate/2.0;

	income[GOLD] += .0075 * p.resources[WORKERS];
	income[GOLD] += .03 * p.resources[SPECIALISTS];

	income[SPECIALISTS] += .001 * p.resources[WORKERS];
	income[WORKERS] -= .001 * p.resources[WORKERS];

	p.power += .1;

	for (int i = 0; i <= HORSES; i++) {
		p.resources[i] = max(0, p.resources[i] + income[i]);
		if (m[i] > -1 && p.resources[i] > m[i]) {
			p.resources[i] = m[i];
		}
		p.max[i] = m[i];
	}
}