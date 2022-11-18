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
	//Spread Currents Across the Land

	//Create Rivers and Lakes

	//2nd Current Pass

	//Biomes and Forests

}