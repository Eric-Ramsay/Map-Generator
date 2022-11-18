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

void Income(Player& p) {
	for (C c : p.land) {
		if (map[c.y][c.x].forest != NONE) {
			if (map[c.y][c.x].forest == FORAGE) {
				p.resources[FOOD] += 15;
				p.resources[WATER] += 5;
			}
			else {
				p.resources[WOOD] += .03;
				p.resources[FOOD] += .1;
			}
		}
		else {
			switch (map[c.y][c.x].type) {
			case LAKE: case RIVER: p.resources[WATER] += 1; p.resources[FOOD] += .25; break;
			case OCEAN: case SHALLOW: p.resources[FOOD] += .25;
			case MEADOW: p.resources[WATER] += .05; p.resources[FOOD] += .25; break;
			case GRASS_1: p.resources[WATER] += .05; p.resources[FOOD] += .125; break;
			case GRASS_2: p.resources[WATER] += .05; p.resources[FOOD] += .05; break;
			case GRASS_3: p.resources[WATER] += .05; p.resources[FOOD] += .025; break;
			}
		}
	}
	p.resources[FOOD] -= (p.resources[WORKERS] + p.resources[SPECIALISTS]);
	p.resources[WATER] -= (p.resources[WORKERS] + p.resources[SPECIALISTS]);


	pop = pop * (1 + .03 * min(2, housing / pop));
	pop *= .957;
	if (p.resources[FOOD] > 0 && p.resources[WATER] > 0) {
		p.resources[WORKERS] *= (1 + .03 * min(2, housing / pop));
		p.resources[SPECIALISTS] *= 1.005;
	}
	p.resources[WORKERS] *= .999;
	p.resources[SPECIALISTS] *= .999;
}