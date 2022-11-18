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
	float num = (pMax - pop) / (pMax);
	return .06 * (1 + (num / 1.5));
}

void BuildingIncome(int x, int y, float(&wIncome)[WATER + 1], float(&wMax)[WATER + 1], float (&sIncome)[WATER + 1], float (&sMax)[WATER + 1]) {
	int type = map[y][x].building.type;
	//HOUSE, FARM, WATER_NET, MILL, LUMBER_CAMP, MINE

	float income[WATER + 1];
	float m[WATER + 1];
	for (int i = 0; i < WATER + 1; i++) {
		income[i] = 0;
		m[i] = 0;
	}

	if (type == HOUSE) {
		m[WORKERS] += 8;
		m[SPECIALISTS] += 2;
	}
	else if (type == FARM) {
		bool adjMill = false;
		income[FOOD] += 20 - map[y][x].type;
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
		income[WATER] += 20 + 3*map[y][x].elev;
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
					if (num_trees == 4) {
						a = 2; b = 2;
					}
				}
			}
		}
		income[WOOD] += num_trees * .1;
	}
	else if (type == MINE) {
		income[STONE] += .4 + map[y][x].elev/20.0;
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
		income[WATER] += 50;
	}
	else if (type == ADMIN_OFFICE) {
		income[POWER] += .25;
		income[W_WELLBEING] += .05;
		income[GOLD] -= .1;
	}
	else if (type == WOOD_SHOP) {
		income[WOOD] -= 1;
		income[GOLD] += .5;
	}

	float workers = buildings[map[y][x].building.type].costs[3];
	float specialists = 2 * buildings[map[y][x].building.type].costs[4];

	if (workers + specialists <= 0) {
		workers = 1.0;
		specialists = 0.0;
	}
	else {
		float total_pop = workers + specialists;
		workers /= total_pop;
		specialists /= total_pop;
	}

	for (int i = 0; i < WATER + 1; i++) {
		wIncome[i] += income[i] * workers;
		wMax[i] += m[i] * workers;
		sIncome[i] += income[i] * specialists;
		sMax[i] += m[i] * specialists;
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

	tIncome[GOLD] = .1;
	tIncome[WORKERS] = .25;
	tIncome[WOOD] = .1;
	tIncome[STONE] = .05;

	//Go through each tile and calculate income from that tile
	for (int i = 0; i < 2; i++) {
		for (C c : p.land) {
			if (i == 0) {
				if (map[c.y][c.x].owner == p.turn) {
					bool found = false;
					for (C check : new_land) {
						if (check == c) {
							found = true;
							break;
						}
					}
					if (!found) {
						new_land.push_back(c);
					}
				}
				if (map[c.y][c.x].building.type != -1) {
					if (buildings[map[c.y][c.x].building.type].costs[3] == 0 && buildings[map[c.y][c.x].building.type].costs[4] == 0) {
						BuildingIncome(c.x, c.y, tIncome, tMax, sIncome, sMax);
					}
					else {
						BuildingIncome(c.x, c.y, wIncome, tMax, sIncome, sMax);
						wIncome[W_JOBS] += buildings[map[c.y][c.x].building.type].costs[3];
						sIncome[S_JOBS] += buildings[map[c.y][c.x].building.type].costs[4];
					}
				}
			}
			else {
				if (map[c.y][c.x].building.type == -1) {
					if (map[c.y][c.x].forest != NONE) {
						if (map[c.y][c.x].forest == FORAGE) {
							gIncome[FOOD] += 10;
							gIncome[WATER] += 10;
						}
						else {
							gIncome[WOOD] += .03;
							gIncome[FOOD] += .3;
						}
					}
					else {
						gIncome[STONE] += .002 * map[c.y][c.x].elev;
						switch (map[c.y][c.x].type) {
						case LAKE: case RIVER: gIncome[WATER] += 2; gIncome[FOOD] += 1; break;
						case OCEAN: case SHALLOW: gIncome[FOOD] += 1; gIncome[GOLD] += .0075; break;
						case MEADOW: gIncome[WATER] += .75; gIncome[FOOD] += .75; gIncome[WORKERS] += .009; break;
						case GRASS_1: gIncome[WATER] += .6; gIncome[FOOD] += .6; gIncome[WORKERS] += .008; break;
						case GRASS_2: gIncome[WATER] += .5; gIncome[FOOD] += .5; gIncome[WORKERS] += .007; break;
						case GRASS_3: gIncome[WATER] += .3; gIncome[FOOD] += .4; gIncome[WORKERS] += .006; break;
						case GRASS_4: gIncome[WATER] += .3; gIncome[FOOD] += .3; gIncome[WORKERS] += .005; break;
						case GRASS_5: gIncome[WATER] += .3; gIncome[FOOD] += .2; break;
						default: gIncome[STONE] += .005; break;
						}
					}
				}
			}
		}
		if (i == 1) {
			gIncome[G_JOBS] = (gIncome[STONE] + gIncome[GOLD] + gIncome[WOOD]) * 2.5 + (gIncome[FOOD] + gIncome[WATER]) * .2;
		}
	}

	tMax[WORKERS] += p.land.size() * .1;
	tMax[SPECIALISTS] += p.land.size() * .01;

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

	tIncome[POWER] += .25;

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

	float deathRate = .061;
	
	if (p.res[FOOD] > 0 && p.res[WATER] > 0) {
		tIncome[WORKERS] = p.res[WORKERS] * growthRate(p.res[WORKERS], tMax[WORKERS]);
		tIncome[SPECIALISTS] = p.res[SPECIALISTS] * (growthRate(p.res[SPECIALISTS], tMax[SPECIALISTS])/3.0);

		tIncome[W_WELLBEING] += .1;
	}
	else {
		if (p.res[FOOD] <= 0) {
			deathRate *= 1.2;
			tIncome[W_WELLBEING] -= 3;
		}
		if (p.res[WATER] <= 0) {
			deathRate *= 1.2;
			tIncome[W_WELLBEING] -= 3;
		}
	}

	tIncome[WORKERS] +=  p.res[WORKERS] * -deathRate;
	tIncome[SPECIALISTS] += p.res[SPECIALISTS] * -deathRate/2.0;

	tIncome[GOLD] += .005 * p.res[WORKERS];
	tIncome[GOLD] += .02 * p.res[SPECIALISTS];

	tIncome[SPECIALISTS] += .002 * p.res[WORKERS];
	tIncome[WORKERS] -= .002 * p.res[WORKERS];

	std::cout << "Max Workers: " << tMax[WORKERS] << std::endl;

	for (int i = 0; i <= WATER; i++) {
		p.res[i] = max(0, p.res[i] + tIncome[i]);
		if (tMax[i] > -1 && p.res[i] > tMax[i]) {
			p.res[i] = tMax[i];
		}
		p.max[i] = tMax[i];
		p.income = tIncome;
	}

	

	p.res[S_JOBS] = sIncome[S_JOBS];
	p.res[W_JOBS] = wIncome[W_JOBS];
	p.res[G_JOBS] = gIncome[G_JOBS];

	p.land = new_land;
}