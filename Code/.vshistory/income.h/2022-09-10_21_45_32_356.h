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
	return .03 * min(2, pMax / max(1, pop));
}

void BuildingIncome(int x, int y, float& income[HORSES + 1], float& max[HORSES + 1]) {
	int type = map[y][x].building.type;

	//HOUSE, FARM, WATER_NET, MILL, LUMBER_CAMP, MINE

	if (type == HOUSE) {
		max[WORKERS] += 5;
		max[SPECIALISTS] += 5;
	}
	else if (type == FARM) {
		income[FOOD] += 10;
		income[WATER] += 2;
		for (int a = -1; a < 2; a++) {
			for (int b = -1; b < 2; b++) {
				if (map[safeC(y + a)][safeC(x + b)].building.type == MILL) {
					income[FOOD] += 10;
				}
			}
		}
	}
	else if (type == WATER_NET) {
		income[WATER] += 10;
	}
	else if (type == MILL) {
		max[FOOD] += 250;
	}
	else if (type == LUMBER_CAMP) {
		max[WOOD] += 3;
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
		max[STONE] += 3;
	}
	else if (type == STOREHOUSE) {
		max[STONE] += 25;
		max[WOOD] += 25;
		max[FOOD] += 500;
	}
	else if (type == WELL) {
		max[WATER] += 500;
		income[WATER] += 1;
	}

}


void Income(Player& p) {
	if (p.land.size() == 0) {
		return;
	}
	std::vector<C> new_land = {};
	//WOOD, STONE, GOLD, WORKERS, W_JOBS, SPECIALISTS, S_JOBS, FOOD, WATER, LUXURIES, COPPER, TIN, BRONZE, IRON, CATTLE, HORSES
	float income[HORSES + 1] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	float max[HORSES + 1] = { 25, 25, -1, 50, -1, 0, -1, 1000, 1000, 50, 50, 50, 50, 50, 10, 10, };
	//Reset Jobs to recalculate unemployment and what not
	p.resources[W_JOBS] = 0;
	p.resources[S_JOBS] = 0;

	for (C c : p.land) {
		if (map[c.y][c.x].owner == p.turn) {
			new_land.push_back(c);
		}
		if (map[c.y][c.x].building.type == -1) {
			if (map[c.y][c.x].forest != NONE) {
				if (map[c.y][c.x].forest == FORAGE) {
					income[FOOD] += 15;
					income[WATER] += 5;
				}
				else {
					income[WOOD] += .03;
					income[FOOD] += .1;
				}
			}
			else {
				switch (map[c.y][c.x].type) {
				case LAKE: case RIVER: income[WATER] += 1; income[FOOD] += .25; break;
				case OCEAN: case SHALLOW: income[FOOD] += .25;
				case MEADOW: income[WATER] += .4; income[FOOD] += .5; break;
				case GRASS_1: income[WATER] += .4; income[FOOD] += .5; break;
				case GRASS_2: income[WATER] += .4; income[FOOD] += .4; break;
				case GRASS_3: income[WATER] += .3; income[FOOD] += .3; break;
				case GRASS_4: income[WATER] += .2; income[FOOD] += .2; break;
				case GRASS_5: income[WATER] += .2; income[FOOD] += .1; break;
				}
			}
		}
	}

	p.resources[W_JOBS] = (income[WATER] + income[FOOD] + income[WOOD] * 3) / 3.0;

	for (C c : p.land) {
		if (map[c.y][c.x].building.type != -1) {

		}
	}

	//Reductions from pop management. IE - food/water, births, deaths
	income[FOOD] -= (p.resources[WORKERS] + p.resources[SPECIALISTS]);
	income[WATER] -= (p.resources[WORKERS] + p.resources[SPECIALISTS]);

	if (p.resources[FOOD] > 0 && p.resources[WATER] > 0) {
		income[WORKERS] = p.resources[WORKERS] * growthRate(p.resources[WORKERS], p.max[WORKERS]);
		income[SPECIALISTS] = p.resources[SPECIALISTS] * (growthRate(p.resources[SPECIALISTS], p.max[SPECIALISTS])/2);
	}

	float deathRate = .033;

	income[WORKERS] +=  p.resources[WORKERS] * -deathRate;
	income[SPECIALISTS] += p.resources[SPECIALISTS] * -deathRate/2.0;

	for (int i = 0; i <= HORSES; i++) {
		p.resources[i] = max(0, p.resources[i] + income[i]);
		if (p.max[i] > -1 && p.resources[i] > p.max[i]) {
			p.resources[i] = p.max[i];
		}
	}
}