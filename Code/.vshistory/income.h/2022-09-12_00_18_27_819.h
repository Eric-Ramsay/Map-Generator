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

void BuildingIncome(int x, int y, float (&income)[WATER + 1], float (&m)[WATER + 1]) {
	int type = map[y][x].building.type;

	//HOUSE, FARM, WATER_NET, MILL, LUMBER_CAMP, MINE

	if (type == HOUSE) {
		m[WORKERS] += 6;
		m[SPECIALISTS] += 2;
	}
	else if (type == FARM) {
		bool adjMill = false;
		income[FOOD] += max(0, 16 - map[y][x].type);
		for (int a = -1; a < 2; a++) {
			for (int b = -1; b < 2; b++) {
				if (!adjMill && map[safeC(y + a)][safeC(x + b)].building.type == MILL) {
					income[FOOD] += max(0, 16 - map[y][x].type);
					adjMill = true;
				}
				if (map[safeC(y + a)][safeC(x + b)].building.type == FARM) {
					income[FOOD]++;
				}
			}
		}
	}
	else if (type == WATER_NET) {
		income[WATER] += 9 + map[y][x].elev;
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
		income[STONE] += .5 + map[y][x].elev /10.0;
		m[STONE] += 10;
	}
	else if (type == STOREHOUSE) {
		m[STONE] += 30;
		m[WOOD] += 30;
		m[FOOD] += 500;
		m[WATER] += 150;
	}
	else if (type == WELL) {
		m[WATER] += 500;
		income[WATER] += 20;
	}
	else if (type == ADMIN_OFFICE) {
		income[WATER] += 15;
	}
}


void Income(Player& p) {
	if (p.land.size() == 0) {
		return;
	}
	std::vector<C> new_land = {};

	//Gatherer, Worker, Specialist, and Total incomes/max resources
	float gIncome[WATER + 1];
	float wIncome[WATER + 1];
	float sIncome[WATER + 1];
	float tIncome[WATER + 1];
	float wMax[WATER + 1];
	float sMax[WATER + 1];
	for (int i = 0; i < WATER + 1; i++) {
		gIncome[i] = 0;
		wIncome[i] = 0;
		sIncome[i] = 0;
		tIncome[i] = 0;
		wMax[i] = 0;
		sMax[i] = 0;
	}
	float tMax[WATER + 1] = {250, 50, 0, -1, -1, -1, 100, 25, 25, -1, 1000, 1000 };

	//Go through each tile and calculate income from that tile
	for (int i = 0; i < 2; i++) {
		for (C c : p.land) {
			if (i == 0) {
				if (map[c.y][c.x].owner == p.turn) {
					new_land.push_back(c);
				}
				if (map[c.y][c.x].building.type != -1) {
					if (buildings[map[c.y][c.x].building.type].costs[4] > 0) {
						BuildingIncome(c.x, c.y, sIncome, sMax);
					}
					else {
						BuildingIncome(c.x, c.y, wIncome, wMax);
					}
					wIncome[W_JOBS] += buildings[map[c.y][c.x].building.type].costs[3];
					sIncome[S_JOBS] += buildings[map[c.y][c.x].building.type].costs[4];
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
						gIncome[STONE] += .0025 * map[c.y][c.x].elev;
						switch (map[c.y][c.x].type) {
						case LAKE: case RIVER: gIncome[WATER] += 2; gIncome[FOOD] += .25; break;
						case OCEAN: case SHALLOW: gIncome[FOOD] += .25;
						case MEADOW: gIncome[WATER] += .4; gIncome[FOOD] += .5; break;
						case GRASS_1: gIncome[WATER] += .4; gIncome[FOOD] += .5; break;
						case GRASS_2: gIncome[WATER] += .4; gIncome[FOOD] += .4; break;
						case GRASS_3: gIncome[WATER] += .3; gIncome[FOOD] += .3; break;
						case GRASS_4: gIncome[WATER] += .2; gIncome[FOOD] += .2; break;
						case GRASS_5: gIncome[WATER] += .2; gIncome[FOOD] += .1; break;
						default: gIncome[STONE] += .005; break;
						}
					}
				}
			}
		}
		if (i == 1) {
			gIncome[G_JOBS] = (gIncome[WATER] + gIncome[FOOD] + gIncome[WOOD] * 3) / 3.0;
		}
	}

	//Calculate Efficiency for Each Work Group
	float sEff = 1;
	float wEff = 1;
	float gEff = 1;

	//Specialist Efficiency
	if (sIncome[S_JOBS] > 0) {
		sEff = min(1, p.res[SPECIALISTS] / sIncome[S_JOBS]);
	}
	//Worker Efficiency
	if (wIncome[W_JOBS] > 0) {
		wEff = min(1, p.res[WORKERS] / wIncome[W_JOBS]);
	}
	if (gIncome[G_JOBS] > 0 && p.res[WORKERS] > wIncome[W_JOBS]) {
		gEff = min(1, (p.res[WORKERS] - wIncome[W_JOBS]) / gIncome[G_JOBS]);
	}

	std::cout << sEff << std::endl << wEff << std::endl << gEff << std::endl;
	std::cout << "------------" << std::endl;

	tIncome[POWER] += .1;

	//Combine resources and max, appropriately adjusted for worker efficiency
	for (int i = 0; i < WATER + 1; i++) {
		tIncome[i] += wIncome[i] * wEff;
		tIncome[i] += gIncome[i] * gEff;
		tIncome[i] += sIncome[i] * sEff;
		tMax[i] += wMax[i] * wEff;
		tMax[i] += sMax[i] * sEff;
	}

	//Reductions from pop management. IE - food/water, births, deaths
	tIncome[FOOD] -= (p.res[WORKERS] + p.res[SPECIALISTS]) * 2;
	tIncome[WATER] -= (p.res[WORKERS] + p.res[SPECIALISTS]) * 2;

	float deathRate = .066;
	
	if (p.res[FOOD] > 0 && p.res[WATER] > 0) {
		tIncome[WORKERS] = p.res[WORKERS] * growthRate(p.res[WORKERS], tMax[WORKERS]);
		tIncome[SPECIALISTS] = p.res[SPECIALISTS] * (growthRate(p.res[SPECIALISTS], tMax[SPECIALISTS])/2);

		tIncome[WELLBEING] += .1;
	}
	else {
		if (p.res[FOOD] <= 0) {
			deathRate *= 1.2;
			tIncome[WELLBEING] -= 1;
		}
		if (p.res[WATER] <= 0) {
			deathRate *= 1.2;
			tIncome[WELLBEING] -= 1;
		}
	}

	tIncome[WORKERS] +=  p.res[WORKERS] * -deathRate;
	tIncome[SPECIALISTS] += p.res[SPECIALISTS] * -deathRate/2.0;

	tIncome[GOLD] += .0075 * p.res[WORKERS];
	tIncome[GOLD] += .03 * p.res[SPECIALISTS];

	tIncome[SPECIALISTS] += .001 * p.res[WORKERS];
	tIncome[WORKERS] -= .001 * p.res[WORKERS];

	std::cout << "Max Workers: " << tMax[WORKERS] << std::endl;

	for (int i = 0; i <= WATER; i++) {
		p.res[i] = max(0, p.res[i] + tIncome[i]);
		if (tMax[i] > -1 && p.res[i] > tMax[i]) {
			p.res[i] = tMax[i];
		}
		p.max[i] = tMax[i];
	}

	p.res[S_JOBS] = sIncome[S_JOBS];
	p.res[W_JOBS] = wIncome[W_JOBS];
	p.res[G_JOBS] = gIncome[G_JOBS];

}