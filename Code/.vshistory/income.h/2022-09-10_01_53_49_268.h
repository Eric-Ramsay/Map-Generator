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


void Income(Player& p) {
	float income[HORSES + 1] = { 5, 5, 5, 25, 0, 0, 0, 500, 500, 0, 0, 0, 0, 0, 0, 0 };
	for (C c : p.land) {
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
	income[FOOD] -= (p.resources[WORKERS] + p.resources[SPECIALISTS]);
	income[WATER] -= (p.resources[WORKERS] + p.resources[SPECIALISTS]);

	if (p.resources[FOOD] > 0 && p.resources[WATER] > 0) {
		income[WORKERS] = p.resources[WORKERS] * growthRate(p.resources[WORKERS], p.max[WORKERS]);
		income[SPECIALISTS] = p.resources[SPECIALISTS] * (growthRate(p.resources[SPECIALISTS], p.max[SPECIALISTS])/2);
	}

	float deathRate = .033;

	income[WORKERS] +=  -deathRate;
	income[SPECIALISTS] += p.resources[SPECIALISTS] * -deathRate/2.0;
}